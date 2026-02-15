#include "Samples.h"
#include <cmath>

namespace rb338
{
    // Helper functions for TR-909 synthesis
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

    static float highPassFilter(float input, float& state, float cutoff)
    {
        float output = input - state;
        state += output * cutoff;
        return output;
    }

    static float bandPassFilter(float input, float& stateLow, float& stateHigh, float lowCutoff, float highCutoff)
    {
        // Simple 2-pole bandpass
        float low = stateLow + (input - stateLow) * lowCutoff;
        stateLow = low;
        float high = input - stateHigh;
        stateHigh += high * highCutoff;
        return high * 0.5f;
    }

    // State-variable resonant filter (Chamberlin)
    struct StateVariableFilter
    {
        float low = 0.0f;
        float band = 0.0f;
        float freq = 0.1f;
        float Q = 2.5f;

        void setParams(float frequency, float resonance)
        {
            freq = 2.0f * std::sin(juce::MathConstants<float>::pi * frequency);
            Q = resonance;
        }

        void process(float input, float& lpOut, float& bpOut, float& hpOut)
        {
            low += freq * band;
            float hp = input - low - Q * band;
            band += freq * hp;
            lpOut = low;
            bpOut = band;
            hpOut = hp;
        }
    };

    // Soft clipping waveshaper
    static float softClip(float x, float drive)
    {
        float driven = x * drive;
        float abs_x = std::abs(driven);
        if (abs_x > 3.0f)
            return (driven > 0.0f) ? 1.0f : -1.0f;
        float x2 = driven * driven;
        return driven * (27.0f + x2) / (27.0f + 9.0f * x2);
    }

    void SampleLibrary::prepare(double sampleRate)
    {
        generateDefaults(sampleRate);
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

        samples[(int)instrument] = std::move(loaded);
        return true;
    }

    void SampleLibrary::regenerate(Instrument instrument, double sampleRate, const InstrumentParams& params)
    {
        switch (instrument)
        {
            case Instrument::Kick:      samples[(int)instrument] = generateKick(sampleRate, params); break;
            case Instrument::Snare:     samples[(int)instrument] = generateSnare(sampleRate, params); break;
            case Instrument::Clap:      samples[(int)instrument] = generateClap(sampleRate, params); break;
            case Instrument::Rim:       samples[(int)instrument] = generateRim(sampleRate, params); break;
            case Instrument::TomLow:    samples[(int)instrument] = generateTom(sampleRate, 65.0f, params); break;
            case Instrument::TomMid:    samples[(int)instrument] = generateTom(sampleRate, 110.0f, params); break;
            case Instrument::TomHigh:   samples[(int)instrument] = generateTom(sampleRate, 145.0f, params); break;
            case Instrument::ClosedHat: samples[(int)instrument] = generateHat(sampleRate, false, params); break;
            case Instrument::OpenHat:   samples[(int)instrument] = generateHat(sampleRate, true, params); break;
            case Instrument::Crash:     samples[(int)instrument] = generateCrash(sampleRate, params); break;
            case Instrument::Ride:      samples[(int)instrument] = generateRide(sampleRate, params); break;
            default: break;
        }
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

    Sample SampleLibrary::generateKick(double sampleRate, const InstrumentParams& params) const
    {
        // TR-909 Bass Drum: Dual oscillator with pitch envelope
        const float duration = 0.15f + params.decay * 0.35f; // 150-500ms
        const int length = (int)(sampleRate * duration);
        Sample sample;
        sample.sampleRate = sampleRate;
        sample.data.setSize(1, length);

        // Tune: 30-80Hz range
        const float basePitch = 40.0f + params.tune * 40.0f;

        float phase1 = 0.0f; // Low oscillator
        float phase2 = 0.0f; // High oscillator

        for (int i = 0; i < length; ++i)
        {
            float t = (float)i / (float)sampleRate;

            // Pitch envelope (sweep down)
            float pitchEnv = fastExpDecay(t, 25.0f);
            float freq1 = basePitch + pitchEnv * 25.0f;
            float freq2 = freq1 * 2.0f;

            // Amplitude envelope
            float env = fastExpDecay(t, 8.0f + params.decay * 12.0f);

            // Attack click transient
            float click = 0.0f;
            if (i < 50)
                click = 0.6f * fastExpDecay(t, 100.0f);

            // Dual sine oscillators (909 style)
            phase1 += juce::MathConstants<float>::twoPi * freq1 / (float)sampleRate;
            phase2 += juce::MathConstants<float>::twoPi * freq2 / (float)sampleRate;

            float osc1 = std::sin(phase1) * 0.7f;
            float osc2 = std::sin(phase2) * 0.3f;

            float value = (osc1 + osc2 + click) * env;
            sample.data.setSample(0, i, juce::jlimit(-1.0f, 1.0f, value * 0.9f));
        }

        return sample;
    }

    Sample SampleLibrary::generateSnare(double sampleRate, const InstrumentParams& params) const
    {
        // TR-909 Snare: Dual triangle oscillators + filtered noise
        const float duration = 0.15f + params.decay * 0.35f;
        const int length = (int)(sampleRate * duration);
        Sample sample;
        sample.sampleRate = sampleRate;
        sample.data.setSize(1, length);

        // Tune affects the triangle oscillators
        const float tuneOffset = (params.tune - 0.5f) * 100.0f;
        const float freq1 = 180.0f + tuneOffset;
        const float freq2 = 330.0f + tuneOffset;

        juce::Random rng;
        float phase1 = 0.0f;
        float phase2 = 0.0f;
        float hpState = 0.0f;
        float bpLow = 0.0f, bpHigh = 0.0f;

        for (int i = 0; i < length; ++i)
        {
            float t = (float)i / (float)sampleRate;

            // Separate envelopes for tone and noise
            float toneEnv = fastExpDecay(t, 12.0f + params.tone * 8.0f);
            float noiseEnv = fastExpDecay(t, 10.0f + params.snappy * 10.0f);

            // Triangle oscillators (909 characteristic)
            phase1 += juce::MathConstants<float>::twoPi * freq1 / (float)sampleRate;
            phase2 += juce::MathConstants<float>::twoPi * freq2 / (float)sampleRate;
            float tone = (triangle(phase1) + triangle(phase2)) * 0.4f * toneEnv;

            // Noise with band-pass filter
            float noise = rng.nextFloat() * 2.0f - 1.0f;
            noise = bandPassFilter(noise, bpLow, bpHigh, 0.08f, 0.12f);
            noise *= noiseEnv * (0.6f + params.snappy * 0.4f);

            float value = tone + noise;
            sample.data.setSample(0, i, juce::jlimit(-1.0f, 1.0f, value * 0.7f));
        }

        return sample;
    }

    Sample SampleLibrary::generateClap(double sampleRate, const InstrumentParams& params) const
    {
        // TR-909 Clap: Multiple delayed noise bursts
        const int length = (int)(sampleRate * 0.5);
        Sample sample;
        sample.sampleRate = sampleRate;
        sample.data.setSize(1, length);

        juce::Random rng;
        float hpState = 0.0f;

        // 3 clap strikes with delays
        const int strike1 = 0;
        const int strike2 = (int)(sampleRate * 0.025f);
        const int strike3 = (int)(sampleRate * 0.050f);

        for (int i = 0; i < length; ++i)
        {
            float t = (float)i / (float)sampleRate;
            float noise = rng.nextFloat() * 2.0f - 1.0f;

            // High-pass filter (909 claps are bright)
            noise = highPassFilter(noise, hpState, 0.01f);

            // Amplitude for each strike
            float amp = 0.0f;
            if (i >= strike1 && i < strike1 + 800)
                amp = fastExpDecay((i - strike1) / (float)sampleRate, 18.0f);
            if (i >= strike2 && i < strike2 + 800)
                amp += fastExpDecay((i - strike2) / (float)sampleRate, 18.0f) * 0.9f;
            if (i >= strike3 && i < strike3 + 800)
                amp += fastExpDecay((i - strike3) / (float)sampleRate, 18.0f) * 0.8f;

            float value = noise * amp * (0.5f + params.tone * 0.3f);
            sample.data.setSample(0, i, juce::jlimit(-1.0f, 1.0f, value * 0.6f));
        }

        return sample;
    }

    Sample SampleLibrary::generateRim(double sampleRate, const InstrumentParams& params) const
    {
        // TR-909 Rimshot: Tight click with tonal component
        const int length = (int)(sampleRate * 0.04f); // Shorter for tight click
        Sample sample;
        sample.sampleRate = sampleRate;
        sample.data.setSize(1, length);

        juce::Random rng;
        float bpLow = 0.0f, bpHigh = 0.0f;
        float phase = 0.0f;

        // Tonal component frequency (around 1kHz for woody click)
        const float clickFreq = 900.0f + params.tone * 400.0f;

        for (int i = 0; i < length; ++i)
        {
            float t = (float)i / (float)sampleRate;
            float env = fastExpDecay(t, 55.0f); // Very fast decay

            // Sine wave for tonal click
            phase += juce::MathConstants<float>::twoPi * clickFreq / (float)sampleRate;
            float tonal = std::sin(phase) * 0.4f;

            // Filtered noise for attack
            float noise = rng.nextFloat() * 2.0f - 1.0f;
            noise = bandPassFilter(noise, bpLow, bpHigh, 0.12f, 0.18f) * 0.6f;

            // Mix tonal and noise
            float output = (tonal + noise) * env;

            sample.data.setSample(0, i, juce::jlimit(-1.0f, 1.0f, output * 0.6f));
        }

        return sample;
    }

    Sample SampleLibrary::generateTom(double sampleRate, float baseFreq, const InstrumentParams& params) const
    {
        // TR-909 Tom: Dual triangle oscillators with pitch sweep
        const float duration = 0.2f + params.decay * 0.6f;
        const int length = (int)(sampleRate * duration);
        Sample sample;
        sample.sampleRate = sampleRate;
        sample.data.setSize(1, length);

        // Tune parameter affects base frequency
        const float tunedFreq = baseFreq * (0.7f + params.tune * 0.6f);

        float phase1 = 0.0f;
        float phase2 = 0.0f;

        for (int i = 0; i < length; ++i)
        {
            float t = (float)i / (float)sampleRate;

            // Pitch envelope (downward sweep)
            float pitchEnv = fastExpDecay(t, 18.0f);
            float freq1 = tunedFreq + pitchEnv * 20.0f;
            float freq2 = freq1 * 1.5f;

            // Amplitude envelope
            float env = fastExpDecay(t, 5.0f + params.decay * 8.0f);

            // Attack click
            float click = (i < 30) ? 0.3f * (1.0f - (float)i / 30.0f) : 0.0f;

            // Triangle oscillators
            phase1 += juce::MathConstants<float>::twoPi * freq1 / (float)sampleRate;
            phase2 += juce::MathConstants<float>::twoPi * freq2 / (float)sampleRate;

            float value = (triangle(phase1) * 0.6f + triangle(phase2) * 0.4f + click) * env;
            sample.data.setSample(0, i, juce::jlimit(-1.0f, 1.0f, value * 0.7f));
        }

        return sample;
    }

    Sample SampleLibrary::generateHat(double sampleRate, bool open, const InstrumentParams& params) const
    {
        // TR-909 Hi-Hat: Metallic filtered noise (like rim but sustained)
        const float baseDuration = open ? (0.3f + params.decay * 1.2f) : 0.05f;
        const int length = (int)(sampleRate * baseDuration);
        Sample sample;
        sample.sampleRate = sampleRate;
        sample.data.setSize(1, length);

        juce::Random rng(open ? 1234 : 5678);
        float bpLow = 0.0f, bpHigh = 0.0f;
        float hpState = 0.0f;

        // Tune affects filter frequency range
        const float lowCutoff = 0.18f + params.tune * 0.08f;   // 0.18-0.26
        const float highCutoff = 0.22f + params.tune * 0.08f;  // 0.22-0.30

        for (int i = 0; i < length; ++i)
        {
            float t = (float)i / (float)sampleRate;

            // Fast decay for metallic character
            float decayRate = open ? (8.0f + params.decay * 12.0f) : 45.0f;
            float env = fastExpDecay(t, decayRate);

            // White noise
            float noise = rng.nextFloat() * 2.0f - 1.0f;

            // Band-pass filter for metallic character (6-10kHz range)
            noise = bandPassFilter(noise, bpLow, bpHigh, lowCutoff, highCutoff);

            // High-pass filter to remove any low-frequency content
            noise = highPassFilter(noise, hpState, 0.15f);

            // Tone parameter controls brightness
            float brightness = 0.6f + params.tone * 0.4f;
            noise *= brightness;

            // 6-bit quantization for authentic TR-909 character
            const int bitDepth = 6;
            const int maxLevel = (1 << bitDepth) - 1;
            float normalized = (noise + 1.0f) * 0.5f;
            int quantized = (int)(normalized * maxLevel + 0.5f);
            quantized = juce::jlimit(0, maxLevel, quantized);
            float dequantized = ((float)quantized / (float)maxLevel) * 2.0f - 1.0f;

            // Apply envelope
            float output = dequantized * env * 0.5f;

            sample.data.setSample(0, i, juce::jlimit(-1.0f, 1.0f, output));
        }

        return sample;
    }

    Sample SampleLibrary::generateCrash(double sampleRate, const InstrumentParams& params) const
    {
        // TR-909 Crash: Filtered noise with long decay
        const float duration = 1.5f + params.decay * 2.5f;
        const int length = (int)(sampleRate * duration);
        Sample sample;
        sample.sampleRate = sampleRate;
        sample.data.setSize(1, length);

        juce::Random rng(9999);
        float hpState = 0.0f;
        float bpLow = 0.0f, bpHigh = 0.0f;

        for (int i = 0; i < length; ++i)
        {
            float t = (float)i / (float)sampleRate;

            // Slower decay for crash
            float env = fastExpDecay(t, 1.2f + params.decay * 1.8f);

            // White noise
            float noise = rng.nextFloat() * 2.0f - 1.0f;

            // High-pass to remove low frequencies
            noise = highPassFilter(noise, hpState, 0.08f);

            // Band-pass for splash character (tone parameter controls brightness)
            float lowCutoff = 0.08f + params.tone * 0.06f;
            float highCutoff = 0.14f + params.tone * 0.08f;
            noise = bandPassFilter(noise, bpLow, bpHigh, lowCutoff, highCutoff);

            sample.data.setSample(0, i, juce::jlimit(-1.0f, 1.0f, noise * env * 0.4f));
        }

        return sample;
    }

    Sample SampleLibrary::generateRide(double sampleRate, const InstrumentParams& params) const
    {
        // TR-909 Ride: Bell-like tone with filtered noise
        const float duration = 1.0f + params.decay * 2.0f;
        const int length = (int)(sampleRate * duration);
        Sample sample;
        sample.sampleRate = sampleRate;
        sample.data.setSize(1, length);

        juce::Random rng(7777);
        float hpState = 0.0f;
        float bpLow = 0.0f, bpHigh = 0.0f;

        for (int i = 0; i < length; ++i)
        {
            float t = (float)i / (float)sampleRate;

            // Medium decay
            float env = fastExpDecay(t, 2.0f + params.decay * 2.5f);

            // White noise
            float noise = rng.nextFloat() * 2.0f - 1.0f;

            // High-pass filter
            noise = highPassFilter(noise, hpState, 0.10f);

            // Tighter band-pass than crash (more defined, bell-like)
            float lowCutoff = 0.10f + params.tone * 0.05f;
            float highCutoff = 0.16f + params.tone * 0.06f;
            noise = bandPassFilter(noise, bpLow, bpHigh, lowCutoff, highCutoff);

            sample.data.setSample(0, i, juce::jlimit(-1.0f, 1.0f, noise * env * 0.35f));
        }

        return sample;
    }
}
