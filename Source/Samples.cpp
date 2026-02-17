#include "Samples.h"
#include <array>
#include <cmath>
#include <vector>

namespace rb338
{
    static float fastExpDecay(float t, float decay)
    {
        return std::exp(-t * decay);
    }

    static float triangle(float phase)
    {
        float normalized = std::fmod(phase, juce::MathConstants<float>::twoPi);
        normalized /= juce::MathConstants<float>::twoPi;
        return (normalized < 0.5f) ? (4.0f * normalized - 1.0f) : (3.0f - 4.0f * normalized);
    }

    static float square(float phase)
    {
        return (std::fmod(phase, juce::MathConstants<float>::twoPi) < juce::MathConstants<float>::pi) ? 1.0f : -1.0f;
    }

    static float quantizeToBits(float in, int bits)
    {
        const int maxLevel = (1 << bits) - 1;
        float normalized = (juce::jlimit(-1.0f, 1.0f, in) + 1.0f) * 0.5f;
        int q = (int)std::round(normalized * (float)maxLevel);
        q = juce::jlimit(0, maxLevel, q);
        return ((float)q / (float)maxLevel) * 2.0f - 1.0f;
    }

    static float highPassFilter(float input, float& state, float cutoff)
    {
        const float output = input - state;
        state += output * cutoff;
        return output;
    }

    static float lowPassFilter(float input, float& state, float cutoff)
    {
        state += (input - state) * cutoff;
        return state;
    }

    static float bandPassFilter(float input, float& stateLow, float& stateHigh, float lowCutoff, float highCutoff)
    {
        float low = stateLow + (input - stateLow) * lowCutoff;
        stateLow = low;
        float high = input - stateHigh;
        stateHigh += high * highCutoff;
        return high * 0.55f;
    }

    static float softClip(float x, float drive)
    {
        float driven = x * drive;
        float absX = std::abs(driven);
        if (absX > 3.0f)
            return (driven > 0.0f) ? 1.0f : -1.0f;
        float x2 = driven * driven;
        return driven * (27.0f + x2) / (27.0f + 9.0f * x2);
    }

    // Build deterministic 6-bit PCM source that acts like the TR-909 cymbal/hat ROM.
    static std::vector<float> buildMetalRom(double sampleRate, float tune, int seed)
    {
        const int length = (int)(sampleRate * 0.5);
        std::vector<float> rom((size_t)length, 0.0f);
        juce::Random rng(seed);

        std::array<float, 6> baseFreqs = { 3020.0f, 4110.0f, 5230.0f, 6310.0f, 7410.0f, 9200.0f };
        std::array<float, 6> phases = { 0, 0, 0, 0, 0, 0 };
        std::array<float, 6> detune = { -0.018f, -0.007f, 0.0f, 0.009f, 0.014f, 0.021f };

        const float tuneMul = 0.8f + tune * 0.45f;
        float hpState = 0.0f;
        float bpLow = 0.0f, bpHigh = 0.0f;

        for (int i = 0; i < length; ++i)
        {
            float src = 0.0f;
            for (int o = 0; o < 6; ++o)
            {
                float f = baseFreqs[(size_t)o] * tuneMul * (1.0f + detune[(size_t)o]);
                phases[(size_t)o] += juce::MathConstants<float>::twoPi * f / (float)sampleRate;
                src += square(phases[(size_t)o]) * (0.14f + 0.03f * (float)o);
            }

            // Burst noise from the original analog path feeding the converter.
            src += (rng.nextFloat() * 2.0f - 1.0f) * 0.22f;
            src = highPassFilter(src, hpState, 0.18f);
            src = bandPassFilter(src, bpLow, bpHigh, 0.13f, 0.2f);
            src = softClip(src, 1.4f);

            // TR-909 cymbal/hat source is 6-bit.
            rom[(size_t)i] = quantizeToBits(src, 6);
        }

        return rom;
    }

    static float readRom(const std::vector<float>& rom, float& pos, float speed)
    {
        if (rom.empty())
            return 0.0f;

        int i0 = juce::jlimit(0, (int)rom.size() - 1, (int)pos);
        int i1 = juce::jlimit(0, (int)rom.size() - 1, i0 + 1);
        float frac = pos - (float)i0;
        float out = juce::jmap(frac, rom[(size_t)i0], rom[(size_t)i1]);
        pos += speed;
        if (pos >= (float)rom.size())
            pos -= (float)rom.size();
        return out;
    }

    static bool shouldUseReferenceProcessing(Instrument instrument)
    {
        switch (instrument)
        {
            case Instrument::Clap:
            case Instrument::ClosedHat:
            case Instrument::OpenHat:
            case Instrument::Crash:
            case Instrument::Ride:
                return true;
            default:
                return false;
        }
    }

    static float readSampleLinear(const Sample& source, float position, bool wrap)
    {
        const int n = source.data.getNumSamples();
        if (n <= 0)
            return 0.0f;
        if (n == 1)
            return source.data.getSample(0, 0);

        if (wrap)
        {
            while (position < 0.0f)
                position += (float)n;
            while (position >= (float)n)
                position -= (float)n;
        }
        else
        {
            if (position <= 0.0f)
                return source.data.getSample(0, 0);
            if (position >= (float)(n - 1))
                return source.data.getSample(0, n - 1);
        }

        const int i0 = juce::jlimit(0, n - 1, (int)position);
        const int i1 = wrap ? (i0 + 1) % n : juce::jlimit(0, n - 1, i0 + 1);
        const float frac = position - (float)i0;
        return juce::jmap(frac, source.data.getSample(0, i0), source.data.getSample(0, i1));
    }

    void SampleLibrary::prepare(double sampleRate)
    {
        generateDefaults(sampleRate);
        tryLoadReferencePack(sampleRate);

        // Re-render from references at current sample-rate so knob behavior remains active.
        InstrumentParams defaults;
        for (int i = 0; i < (int)Instrument::Count; ++i)
            if (hasReferenceSamples[(size_t)i] && shouldUseReferenceProcessing((Instrument)i))
                regenerate((Instrument)i, sampleRate, defaults);
    }

    const Sample& SampleLibrary::get(Instrument instrument) const
    {
        return samples[(int)instrument];
    }

    bool SampleLibrary::loadFromFile(Instrument instrument, const juce::File& file)
    {
        juce::AudioFormatManager formatManager;
        formatManager.registerBasicFormats();

        std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));
        if (!reader)
            return false;

        Sample loaded;
        loaded.sampleRate = reader->sampleRate;
        loaded.data.setSize(1, (int)reader->lengthInSamples);
        reader->read(&loaded.data, 0, (int)reader->lengthInSamples, 0, true, false);

        referenceSamples[(size_t)instrument] = std::move(loaded);
        hasReferenceSamples[(size_t)instrument] = true;
        if (samples[(int)instrument].data.getNumSamples() <= 0)
            samples[(int)instrument] = referenceSamples[(size_t)instrument];
        return true;
    }

    void SampleLibrary::regenerate(Instrument instrument, double sampleRate, const InstrumentParams& params)
    {
        Sample analogPrimary;

        switch (instrument)
        {
            case Instrument::Kick:      analogPrimary = generateKick(sampleRate, params); break;
            case Instrument::Snare:     analogPrimary = generateSnare(sampleRate, params); break;
            case Instrument::Clap:      analogPrimary = generateClap(sampleRate, params); break;
            case Instrument::Rim:       analogPrimary = generateRim(sampleRate, params); break;
            case Instrument::TomLow:    analogPrimary = generateTom(sampleRate, 65.0f, params); break;
            case Instrument::TomMid:    analogPrimary = generateTom(sampleRate, 110.0f, params); break;
            case Instrument::TomHigh:   analogPrimary = generateTom(sampleRate, 145.0f, params); break;
            case Instrument::ClosedHat: analogPrimary = generateHat(sampleRate, false, params); break;
            case Instrument::OpenHat:   analogPrimary = generateHat(sampleRate, true, params); break;
            case Instrument::Crash:     analogPrimary = generateCrash(sampleRate, params); break;
            case Instrument::Ride:      analogPrimary = generateRide(sampleRate, params); break;
            default: break;
        }

        // Analog model is primary. External reference samples are strict fallback.
        if (analogPrimary.data.getNumSamples() > 0)
        {
            samples[(int)instrument] = std::move(analogPrimary);
            return;
        }

        if (hasReferenceSamples[(size_t)instrument] && shouldUseReferenceProcessing(instrument))
        {
            samples[(int)instrument] = processReferenceSample(instrument, sampleRate, params);
            return;
        }

        samples[(int)instrument].data.setSize(1, 1);
        samples[(int)instrument].data.clear();
        samples[(int)instrument].sampleRate = sampleRate;
    }

    void SampleLibrary::generateDefaults(double sampleRate)
    {
        InstrumentParams defaultParams;
        samples[(int)Instrument::Kick] = generateKick(sampleRate, defaultParams);
        samples[(int)Instrument::Snare] = generateSnare(sampleRate, defaultParams);
        samples[(int)Instrument::Clap] = generateClap(sampleRate, defaultParams);
        samples[(int)Instrument::Rim] = generateRim(sampleRate, defaultParams);
        samples[(int)Instrument::TomLow] = generateTom(sampleRate, 65.0f, defaultParams);
        samples[(int)Instrument::TomMid] = generateTom(sampleRate, 110.0f, defaultParams);
        samples[(int)Instrument::TomHigh] = generateTom(sampleRate, 145.0f, defaultParams);
        samples[(int)Instrument::ClosedHat] = generateHat(sampleRate, false, defaultParams);
        samples[(int)Instrument::OpenHat] = generateHat(sampleRate, true, defaultParams);
        samples[(int)Instrument::Crash] = generateCrash(sampleRate, defaultParams);
        samples[(int)Instrument::Ride] = generateRide(sampleRate, defaultParams);
    }

    void SampleLibrary::tryLoadReferencePack(double sampleRate)
    {
        juce::Array<juce::File> candidateDirs;
        auto addDirIfValid = [&candidateDirs](const juce::File& dir)
        {
            if (dir.exists() && dir.isDirectory())
                candidateDirs.addIfNotAlreadyThere(dir);
        };

        const auto envPath = juce::SystemStats::getEnvironmentVariable("LOS9X9_SAMPLE_PACK", {});
        if (envPath.isNotEmpty())
            addDirIfValid(juce::File(envPath));

        const auto cwd = juce::File::getCurrentWorkingDirectory();
        addDirIfValid(cwd.getChildFile("Samples/TR-909_JP"));
        addDirIfValid(cwd.getChildFile("TR-909_JP"));
        addDirIfValid(cwd.getParentDirectory().getChildFile("Samples/TR-909_JP"));
        addDirIfValid(cwd.getParentDirectory().getChildFile("TR-909_JP"));
        addDirIfValid(cwd.getParentDirectory().getParentDirectory().getChildFile("Samples/TR-909_JP"));

        const auto exe = juce::File::getSpecialLocation(juce::File::currentExecutableFile);
        const auto exeDir = exe.getParentDirectory();
        addDirIfValid(exeDir.getChildFile("Samples/TR-909_JP"));
        addDirIfValid(exeDir.getChildFile("TR-909_JP"));
        addDirIfValid(exeDir.getParentDirectory().getChildFile("Resources/Samples/TR-909_JP"));
        addDirIfValid(exeDir.getParentDirectory().getParentDirectory().getChildFile("Resources/Samples/TR-909_JP"));

        auto findByAliases = [](const juce::File& dir, std::initializer_list<const char*> aliases) -> juce::File
        {
            juce::Array<juce::File> files;
            dir.findChildFiles(files, juce::File::findFiles, true, "*");
            for (auto* alias : aliases)
            {
                juce::String wanted(alias);
                for (const auto& f : files)
                {
                    if (!f.hasFileExtension("wav;aif;aiff;flac"))
                        continue;

                    if (f.getFileName().equalsIgnoreCase(wanted)
                        || f.getFileNameWithoutExtension().equalsIgnoreCase(wanted))
                        return f;
                }
            }
            return {};
        };

        bool loadedAny = false;
        for (const auto& dir : candidateDirs)
        {
            if (!hasReferenceSamples[(size_t)Instrument::Clap])
            {
                auto f = findByAliases(dir, { "clap", "cp", "handclap", "tr909_clap", "909_clap" });
                if (f.existsAsFile() && loadFromFile(Instrument::Clap, f))
                    loadedAny = true;
            }
            if (!hasReferenceSamples[(size_t)Instrument::ClosedHat])
            {
                auto f = findByAliases(dir, { "ch", "closedhat", "closed_hat", "hh_closed", "hihat_closed", "hat_closed" });
                if (f.existsAsFile() && loadFromFile(Instrument::ClosedHat, f))
                    loadedAny = true;
            }
            if (!hasReferenceSamples[(size_t)Instrument::OpenHat])
            {
                auto f = findByAliases(dir, { "oh", "openhat", "open_hat", "hh_open", "hihat_open", "hat_open" });
                if (f.existsAsFile() && loadFromFile(Instrument::OpenHat, f))
                    loadedAny = true;
            }
            if (!hasReferenceSamples[(size_t)Instrument::Crash])
            {
                auto f = findByAliases(dir, { "crash", "cr", "crash_cymbal", "cym_crash" });
                if (f.existsAsFile() && loadFromFile(Instrument::Crash, f))
                    loadedAny = true;
            }
            if (!hasReferenceSamples[(size_t)Instrument::Ride])
            {
                auto f = findByAliases(dir, { "ride", "rd", "ride_cymbal", "cym_ride" });
                if (f.existsAsFile() && loadFromFile(Instrument::Ride, f))
                    loadedAny = true;
            }
        }

        if (loadedAny)
        {
            juce::Logger::writeToLog("LoS.9x9: Loaded external TR-909 reference sample pack.");

            InstrumentParams defaults;
            for (int i = 0; i < (int)Instrument::Count; ++i)
                if (hasReferenceSamples[(size_t)i] && shouldUseReferenceProcessing((Instrument)i))
                    samples[i] = processReferenceSample((Instrument)i, sampleRate, defaults);
        }
    }

    Sample SampleLibrary::processReferenceSample(Instrument instrument, double sampleRate, const InstrumentParams& params) const
    {
        const auto& source = referenceSamples[(size_t)instrument];
        const int inLen = source.data.getNumSamples();
        if (inLen <= 0)
            return {};

        const float srcStepBase = (float)(source.sampleRate / sampleRate);
        float playbackRate = 1.0f;
        float durationSeconds = (float)inLen / (float)source.sampleRate;
        bool wrap = false;

        switch (instrument)
        {
            case Instrument::Clap:
                playbackRate = 0.9f + params.tune * 0.35f;
                durationSeconds = juce::jlimit(0.18f, 1.1f, durationSeconds * (0.65f + params.decay * 0.8f));
                break;
            case Instrument::ClosedHat:
                playbackRate = 0.85f + params.tune * 0.5f;
                durationSeconds = 0.03f + params.decay * 0.09f;
                wrap = true;
                break;
            case Instrument::OpenHat:
                playbackRate = 0.85f + params.tune * 0.5f;
                durationSeconds = 0.22f + params.decay * 1.1f;
                wrap = true;
                break;
            case Instrument::Crash:
                playbackRate = 0.78f + params.tune * 0.42f;
                durationSeconds = juce::jlimit(0.8f, 4.5f, durationSeconds * (0.55f + params.decay * 1.25f));
                wrap = true;
                break;
            case Instrument::Ride:
                playbackRate = 0.72f + params.tune * 0.35f;
                durationSeconds = juce::jlimit(0.6f, 3.6f, durationSeconds * (0.55f + params.decay * 1.0f));
                wrap = true;
                break;
            default:
                break;
        }

        const int outLen = juce::jmax(1, (int)(sampleRate * durationSeconds));
        Sample out;
        out.sampleRate = sampleRate;
        out.data.setSize(1, outLen);

        const float step = srcStepBase * playbackRate;
        float pos = 0.0f;
        float hpState = 0.0f;
        float bpLow = 0.0f, bpHigh = 0.0f;
        float lpState = 0.0f;
        const float brightness = 0.75f + params.tone * 0.55f;

        for (int i = 0; i < outLen; ++i)
        {
            float t = (float)i / (float)sampleRate;
            float src = readSampleLinear(source, pos, wrap);
            pos += step;

            if (!wrap && pos >= (float)(inLen - 1))
            {
                // Keep tail smooth when source ends.
                src *= fastExpDecay((pos - (float)(inLen - 1)) / (float)sampleRate, 30.0f);
            }

            float env = 1.0f;
            switch (instrument)
            {
                case Instrument::Clap:
                    env = fastExpDecay(t, 4.8f + (1.0f - params.decay) * 5.0f);
                    src = quantizeToBits(src, 8);
                    src = highPassFilter(src, hpState, 0.03f + params.tone * 0.02f);
                    src = lowPassFilter(src, lpState, 0.45f + params.tone * 0.2f);
                    break;
                case Instrument::ClosedHat:
                case Instrument::OpenHat:
                {
                    float decayRate = (instrument == Instrument::OpenHat)
                        ? (5.8f + (1.0f - params.decay) * 3.0f)
                        : (34.0f + (1.0f - params.decay) * 25.0f);
                    env = fastExpDecay(t, decayRate);
                    float q = quantizeToBits(src, 6);
                    src = src * 0.65f + q * 0.35f;
                    src = bandPassFilter(src, bpLow, bpHigh, 0.14f + params.tone * 0.06f, 0.21f + params.tone * 0.07f);
                    src = highPassFilter(src, hpState, 0.11f + params.tone * 0.06f);
                    src = lowPassFilter(src, lpState, 0.64f + params.tone * 0.25f);
                    break;
                }
                case Instrument::Crash:
                    env = fastExpDecay(t, 0.95f + (1.0f - params.decay) * 0.9f);
                    src = bandPassFilter(src, bpLow, bpHigh, 0.09f + params.tone * 0.06f, 0.14f + params.tone * 0.07f);
                    src = highPassFilter(src, hpState, 0.06f);
                    break;
                case Instrument::Ride:
                {
                    env = fastExpDecay(t, 1.35f + (1.0f - params.decay) * 1.25f);
                    float bell = std::sin(juce::MathConstants<float>::twoPi * (560.0f + params.tune * 140.0f) * t)
                        * fastExpDecay(t, 2.6f) * 0.18f;
                    src = bandPassFilter(src, bpLow, bpHigh, 0.11f + params.tone * 0.05f, 0.17f + params.tone * 0.06f);
                    src = highPassFilter(src, hpState, 0.08f);
                    src += bell;
                    break;
                }
                default:
                    break;
            }

            float shaped = softClip(src * brightness * env, 1.08f + params.tone * 0.35f);
            out.data.setSample(0, i, juce::jlimit(-1.0f, 1.0f, shaped));
        }

        return out;
    }

    Sample SampleLibrary::generateKick(double sampleRate, const InstrumentParams& params) const
    {
        // 909-style kick: bridged-T body, fast pitch dive, and short attack click.
        const float duration = 0.24f + params.decay * 1.18f;
        const int length = (int)(sampleRate * duration);
        Sample sample;
        sample.sampleRate = sampleRate;
        sample.data.setSize(1, length);

        const float basePitch = 41.0f + params.tune * 52.0f;
        const float attack = juce::jlimit(0.0f, 1.0f, params.tone);
        const float ampDecay = 2.25f - params.decay * 1.75f;

        float phase = 0.0f;
        float phaseSub = 0.0f;
        float hpState = 0.0f;
        juce::Random rng(1978);

        for (int i = 0; i < length; ++i)
        {
            float t = (float)i / (float)sampleRate;

            const float pitchSweep = (112.0f + attack * 65.0f) * fastExpDecay(t, 38.0f + attack * 22.0f)
                + 20.0f * fastExpDecay(t, 6.5f + (1.0f - params.decay) * 3.6f);
            const float freq = basePitch + pitchSweep;
            phase += juce::MathConstants<float>::twoPi * freq / (float)sampleRate;
            phaseSub += juce::MathConstants<float>::twoPi * (basePitch * 0.5f) / (float)sampleRate;

            const float body = std::sin(phase) * 0.88f + std::sin(phase * 2.02f + 0.1f) * 0.19f;
            const float sub = std::sin(phaseSub) * 0.36f
                * fastExpDecay(t, 0.85f + (1.0f - params.decay) * 0.45f);
            const float ampEnv = fastExpDecay(t, ampDecay);

            const float clickEnv = fastExpDecay(t, 140.0f + attack * 170.0f);
            float click = (rng.nextFloat() * 2.0f - 1.0f) * clickEnv * (0.10f + attack * 0.35f);
            click += std::sin(juce::MathConstants<float>::twoPi * (1700.0f - t * 400.0f) * t)
                * clickEnv * (0.05f + attack * 0.22f);

            float out = (body + sub) * ampEnv + click;
            out = highPassFilter(out, hpState, 0.0012f);
            out = softClip(out, 1.55f + params.decay * 0.42f + attack * 0.35f);
            sample.data.setSample(0, i, juce::jlimit(-1.0f, 1.0f, out));
        }

        return sample;
    }

    Sample SampleLibrary::generateSnare(double sampleRate, const InstrumentParams& params) const
    {
        // Snare: tuned twin oscillators + snappy filtered noise burst.
        const float duration = 0.16f + params.decay * 0.33f;
        const int length = (int)(sampleRate * duration);
        Sample sample;
        sample.sampleRate = sampleRate;
        sample.data.setSize(1, length);

        const float tuneOffset = (params.tune - 0.5f) * 120.0f;
        const float tone1 = 185.0f + tuneOffset;
        const float tone2 = 332.0f + tuneOffset * 1.1f;
        const float toneBrightness = 0.65f + params.tone * 0.5f;

        float ph1 = 0.0f;
        float ph2 = 0.0f;
        float bpLow = 0.0f, bpHigh = 0.0f;
        float hpState = 0.0f;
        juce::Random rng(1983);

        for (int i = 0; i < length; ++i)
        {
            float t = (float)i / (float)sampleRate;

            float toneEnv = fastExpDecay(t, 12.0f + params.tone * 9.0f);
            float noiseEnv = fastExpDecay(t, 10.0f + params.snappy * 12.0f);

            ph1 += juce::MathConstants<float>::twoPi * tone1 / (float)sampleRate;
            ph2 += juce::MathConstants<float>::twoPi * tone2 / (float)sampleRate;
            float tonal = (triangle(ph1) * 0.58f + triangle(ph2) * 0.42f) * toneEnv;

            float noise = rng.nextFloat() * 2.0f - 1.0f;
            noise = bandPassFilter(noise, bpLow, bpHigh, 0.07f + params.tone * 0.04f, 0.12f + params.tone * 0.05f);
            noise = highPassFilter(noise, hpState, 0.08f);
            noise *= noiseEnv * (0.45f + params.snappy * 0.85f);

            float out = tonal * toneBrightness * 0.65f + noise;
            out = softClip(out, 1.3f);
            sample.data.setSample(0, i, juce::jlimit(-1.0f, 1.0f, out * 0.78f));
        }

        return sample;
    }

    Sample SampleLibrary::generateClap(double sampleRate, const InstrumentParams& params) const
    {
        // 909 clap: fixed PCM-like burst cluster + analog high-pass/tail shaping.
        const float duration = 0.42f + params.decay * 0.42f;
        const int length = (int)(sampleRate * duration);
        Sample sample;
        sample.sampleRate = sampleRate;
        sample.data.setSize(1, length);

        juce::Random rng(909);
        float hpState = 0.0f;
        float lpState = 0.0f;
        float bpLow = 0.0f, bpHigh = 0.0f;

        const float burstMs = 0.013f;
        const int burstLen = (int)(sampleRate * burstMs);
        const float spacingScale = 0.85f + params.tune * 0.30f;
        const int strike1 = 0;
        const int strike2 = (int)(sampleRate * 0.011f * spacingScale);
        const int strike3 = (int)(sampleRate * 0.023f * spacingScale);
        const int strike4 = (int)(sampleRate * 0.036f * spacingScale);

        for (int i = 0; i < length; ++i)
        {
            float t = (float)i / (float)sampleRate;
            float noise = rng.nextFloat() * 2.0f - 1.0f;

            // Create a deterministic bright source then apply ROM-like quantization.
            noise = bandPassFilter(noise, bpLow, bpHigh, 0.1f + params.tone * 0.06f + params.tune * 0.02f,
                                   0.18f + params.tone * 0.07f + params.tune * 0.02f);
            noise = quantizeToBits(noise, 8);

            auto strikeAmp = [sampleRate, burstLen](int idx, int start, float gain)
            {
                if (idx < start || idx >= start + burstLen)
                    return 0.0f;
                float dt = (float)(idx - start) / (float)sampleRate;
                return fastExpDecay(dt, 72.0f) * gain;
            };

            float burstEnv = 0.0f;
            burstEnv += strikeAmp(i, strike1, 1.00f);
            burstEnv += strikeAmp(i, strike2, 0.92f);
            burstEnv += strikeAmp(i, strike3, 0.82f);
            burstEnv += strikeAmp(i, strike4, 0.70f);

            float tail = 0.0f;
            if (i > strike3)
                tail = fastExpDecay(t - 0.03f, 8.0f + (1.0f - params.decay) * 4.0f);

            float out = noise * (burstEnv + tail * 0.22f);
            out = highPassFilter(out, hpState, 0.03f + params.tone * 0.02f + params.tune * 0.01f);
            out = lowPassFilter(out, lpState, 0.45f + params.tone * 0.2f);
            out = softClip(out, 1.15f);
            sample.data.setSample(0, i, juce::jlimit(-1.0f, 1.0f, out * 0.86f));
        }

        return sample;
    }

    Sample SampleLibrary::generateRim(double sampleRate, const InstrumentParams& params) const
    {
        // Short, woody rim click.
        const float duration = 0.045f + params.decay * 0.07f;
        const int length = (int)(sampleRate * duration);
        Sample sample;
        sample.sampleRate = sampleRate;
        sample.data.setSize(1, length);

        juce::Random rng(5050);
        float bpLow = 0.0f, bpHigh = 0.0f;
        float phase = 0.0f;
        const float clickFreq = 860.0f + params.tune * 760.0f + params.tone * 220.0f;

        for (int i = 0; i < length; ++i)
        {
            float t = (float)i / (float)sampleRate;
            float env = fastExpDecay(t, 56.0f + (1.0f - params.decay) * 32.0f);

            phase += juce::MathConstants<float>::twoPi * clickFreq / (float)sampleRate;
            float tonal = std::sin(phase) * 0.45f + std::sin(phase * 1.97f) * 0.18f;
            float noise = rng.nextFloat() * 2.0f - 1.0f;
            noise = bandPassFilter(noise, bpLow, bpHigh, 0.12f + params.tone * 0.03f, 0.19f + params.tone * 0.04f) * 0.52f;

            float out = (tonal + noise) * env;
            out = softClip(out, 1.22f);
            sample.data.setSample(0, i, juce::jlimit(-1.0f, 1.0f, out * 0.74f));
        }

        return sample;
    }

    Sample SampleLibrary::generateTom(double sampleRate, float baseFreq, const InstrumentParams& params) const
    {
        const float duration = 0.2f + params.decay * 0.75f;
        const int length = (int)(sampleRate * duration);
        Sample sample;
        sample.sampleRate = sampleRate;
        sample.data.setSize(1, length);

        const float tunedFreq = baseFreq * (0.62f + params.tune * 0.88f);
        float phase1 = 0.0f;
        float phase2 = 0.0f;
        float hpState = 0.0f;

        for (int i = 0; i < length; ++i)
        {
            float t = (float)i / (float)sampleRate;
            float pitchEnv = fastExpDecay(t, 16.0f);
            float freq1 = tunedFreq + pitchEnv * (22.0f + baseFreq * 0.03f);
            float freq2 = freq1 * 1.5f;
            float env = fastExpDecay(t, 3.7f + (1.0f - params.decay) * 7.0f);

            float click = (i < 30) ? (0.18f * (1.0f - (float)i / 30.0f)) : 0.0f;
            phase1 += juce::MathConstants<float>::twoPi * freq1 / (float)sampleRate;
            phase2 += juce::MathConstants<float>::twoPi * freq2 / (float)sampleRate;

            float out = (triangle(phase1) * 0.63f + triangle(phase2) * 0.34f + click) * env;
            out = highPassFilter(out, hpState, 0.002f);
            out = softClip(out, 1.2f);
            sample.data.setSample(0, i, juce::jlimit(-1.0f, 1.0f, out * 0.78f));
        }

        return sample;
    }

    Sample SampleLibrary::generateHat(double sampleRate, bool open, const InstrumentParams& params) const
    {
        // Hybrid 909 hat model: metallic square-osc bank + 6-bit ROM source + optional sample layer.
        const float duration = open ? (0.26f + params.decay * 1.18f) : (0.024f + params.decay * 0.085f);
        const int length = (int)(sampleRate * duration);
        Sample sample;
        sample.sampleRate = sampleRate;
        sample.data.setSize(1, length);

        const auto rom = buildMetalRom(sampleRate, params.tune, 31909);
        float romPos = open ? 47.0f : 7.0f;
        const float speed = 0.90f + params.tune * 0.44f;
        const float brightness = 0.75f + params.tone * 0.5f;

        std::array<float, 6> phases = { 0, 0, 0, 0, 0, 0 };
        constexpr std::array<float, 6> baseFreqs = { 3020.0f, 4110.0f, 5230.0f, 6310.0f, 7410.0f, 9200.0f };
        constexpr std::array<float, 6> detune = { -0.020f, -0.010f, -0.002f, 0.008f, 0.014f, 0.021f };

        const Instrument refInst = open ? Instrument::OpenHat : Instrument::ClosedHat;
        const Sample* refSource = hasReferenceSamples[(size_t)refInst] ? &referenceSamples[(size_t)refInst] : nullptr;
        float refPos = open ? 11.0f : 3.0f;
        const float refSpeed = refSource ? (float)(refSource->sampleRate / sampleRate) * (0.86f + params.tune * 0.35f) : 0.0f;

        float hpState = 0.0f;
        float bpLow = 0.0f, bpHigh = 0.0f;
        float lpState = 0.0f;

        for (int i = 0; i < length; ++i)
        {
            float t = (float)i / (float)sampleRate;
            float metallic = 0.0f;
            const float tuneMul = 0.9f + params.tune * 0.3f;
            for (size_t o = 0; o < baseFreqs.size(); ++o)
            {
                const float f = baseFreqs[o] * tuneMul * (1.0f + detune[o]);
                phases[o] += juce::MathConstants<float>::twoPi * f / (float)sampleRate;
                metallic += square(phases[o]) * (0.08f + 0.02f * (float)o);
            }

            float source = readRom(rom, romPos, speed) * 0.58f + metallic * 0.42f;
            if (refSource != nullptr)
            {
                const float refSample = readSampleLinear(*refSource, refPos, true);
                refPos += refSpeed;
                source = source * 0.72f + refSample * 0.56f;
            }

            // Preserve the characteristic 6-bit hat texture before analog-style filtering.
            const float q = quantizeToBits(source, 6);
            source = source * 0.38f + q * 0.62f;

            float shaped = bandPassFilter(source, bpLow, bpHigh, 0.16f + params.tone * 0.05f, 0.24f + params.tone * 0.06f);
            shaped = highPassFilter(shaped, hpState, 0.13f + params.tone * 0.05f);
            shaped = lowPassFilter(shaped, lpState, 0.63f + params.tone * 0.22f);

            float env = 0.0f;
            if (open)
            {
                const float fastEnv = fastExpDecay(t, 9.8f + (1.0f - params.decay) * 6.0f);
                const float slowEnv = fastExpDecay(t, 2.1f + (1.0f - params.decay) * 1.2f);
                env = fastEnv * 0.42f + slowEnv * 0.58f;
            }
            else
            {
                env = fastExpDecay(t, 40.0f + (1.0f - params.decay) * 31.0f);
            }

            const float attackShape = 1.0f - fastExpDecay(t, 1800.0f);
            env *= attackShape;
            float out = softClip(shaped * brightness * env, 1.18f + params.tone * 0.34f);

            sample.data.setSample(0, i, juce::jlimit(-1.0f, 1.0f, out * (open ? 0.70f : 0.78f)));
        }

        return sample;
    }

    Sample SampleLibrary::generateCrash(double sampleRate, const InstrumentParams& params) const
    {
        const float duration = 1.3f + params.decay * 3.0f;
        const int length = (int)(sampleRate * duration);
        Sample sample;
        sample.sampleRate = sampleRate;
        sample.data.setSize(1, length);

        const auto rom = buildMetalRom(sampleRate, params.tune * 0.8f + 0.1f, 44909);
        float romPos = 0.0f;
        const float speed = 0.78f + params.tune * 0.42f;
        float hpState = 0.0f;
        float bpLow = 0.0f, bpHigh = 0.0f;

        for (int i = 0; i < length; ++i)
        {
            float t = (float)i / (float)sampleRate;
            float src = readRom(rom, romPos, speed);
            src = bandPassFilter(src, bpLow, bpHigh, 0.09f + params.tone * 0.06f, 0.14f + params.tone * 0.07f);
            src = highPassFilter(src, hpState, 0.06f);

            float env = fastExpDecay(t, 0.95f + (1.0f - params.decay) * 0.9f);
            float out = softClip(src * env, 1.08f + params.tone * 0.25f);
            sample.data.setSample(0, i, juce::jlimit(-1.0f, 1.0f, out * 0.62f));
        }

        return sample;
    }

    Sample SampleLibrary::generateRide(double sampleRate, const InstrumentParams& params) const
    {
        const float duration = 1.0f + params.decay * 2.2f;
        const int length = (int)(sampleRate * duration);
        Sample sample;
        sample.sampleRate = sampleRate;
        sample.data.setSize(1, length);

        const auto rom = buildMetalRom(sampleRate, params.tune * 0.75f + 0.2f, 55909);
        float romPos = 91.0f;
        const float speed = 0.7f + params.tune * 0.35f;
        float hpState = 0.0f;
        float bpLow = 0.0f, bpHigh = 0.0f;

        for (int i = 0; i < length; ++i)
        {
            float t = (float)i / (float)sampleRate;
            float src = readRom(rom, romPos, speed);
            float bell = std::sin(juce::MathConstants<float>::twoPi * (560.0f + params.tune * 140.0f) * t)
                * fastExpDecay(t, 2.6f) * 0.23f;

            src = bandPassFilter(src, bpLow, bpHigh, 0.11f + params.tone * 0.05f, 0.17f + params.tone * 0.06f);
            src = highPassFilter(src, hpState, 0.08f);

            float env = fastExpDecay(t, 1.35f + (1.0f - params.decay) * 1.25f);
            float out = softClip(src * env + bell, 1.05f + params.tone * 0.2f);
            sample.data.setSample(0, i, juce::jlimit(-1.0f, 1.0f, out * 0.56f));
        }

        return sample;
    }
}
