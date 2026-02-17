#include "Engine.h"

namespace rb338
{
    void Engine::prepare(double newSampleRate, int samplesPerBlock, int numOutputs)
    {
        juce::ignoreUnused(samplesPerBlock, numOutputs);
        sampleRate = newSampleRate;
        sampleLibrary.prepare(sampleRate);
        sequencer.prepare(sampleRate);
        setupDelay(sampleRate);

        for (int inst = 0; inst < (int)Instrument::Count; ++inst)
            updateInstrumentSound((Instrument)inst);
    }

    void Engine::render(juce::AudioBuffer<float>& buffer, int numSamples)
    {
        buffer.clear();
        juce::Array<StepEvent> events;
        juce::Array<StepEvent> pending;

        {
            const juce::SpinLock::ScopedLockType sl(pendingTriggerLock);
            pending.swapWith(pendingTriggers);
        }

        for (const auto& event : pending)
            triggerVoice(event);

        for (int i = 0; i < numSamples; ++i)
        {
            if (sequencer.nextSample(events))
            {
                for (auto& event : events)
                {
                    applyAutomationForStep(event.instrument, event.stepIndex);
                    triggerVoice(event);
                }
            }

            float left = 0.0f;
            float right = 0.0f;
            float delaySend = 0.0f;

            for (int inst = 0; inst < (int)Instrument::Count; ++inst)
            {
                auto instrument = (Instrument)inst;
                float sample = renderInstrument(instrument);
                if (sample == 0.0f)
                    continue;

                auto& channel = channels[inst];
                float level = channel.level;
                float pan = juce::jlimit(-1.0f, 1.0f, channel.pan);
                float panLeft = std::cos((pan + 1.0f) * juce::MathConstants<float>::halfPi * 0.5f);
                float panRight = std::sin((pan + 1.0f) * juce::MathConstants<float>::halfPi * 0.5f);

                float mono = sample * level;
                left += mono * panLeft;
                right += mono * panRight;
                delaySend += mono * channel.delaySend;
            }

            int readPos = (delayWritePos + delayBuffer.getNumSamples() - delaySamples) % delayBuffer.getNumSamples();
            float delayedL = delayBuffer.getSample(0, readPos);
            float delayedR = delayBuffer.getSample(1, readPos);

            delayBuffer.setSample(0, delayWritePos, left + delayedL * delayFeedback + delaySend);
            delayBuffer.setSample(1, delayWritePos, right + delayedR * delayFeedback + delaySend);

            left += delayedL * delayMix;
            right += delayedR * delayMix;

            // Accent-dependent low-end thump reinforcement for kick accents.
            if (kickThumpEnv > 0.0001f)
            {
                float thump = std::sin(kickThumpPhase) * kickThumpEnv * 0.12f;
                kickThumpPhase += juce::MathConstants<float>::twoPi * 48.0f / (float)sampleRate;
                if (kickThumpPhase > juce::MathConstants<float>::twoPi)
                    kickThumpPhase -= juce::MathConstants<float>::twoPi;
                kickThumpEnv *= 0.9982f;

                left += thump;
                right += thump;
            }

            // Soft protection keeps accents powerful but avoids harsh clipping.
            left = safeSaturate(left, 1.08f);
            right = safeSaturate(right, 1.08f);
            const float peak = juce::jmax(std::abs(left), std::abs(right));
            if (peak > 0.98f)
            {
                const float trim = 0.98f / peak;
                left *= trim;
                right *= trim;
            }

            delayWritePos = (delayWritePos + 1) % delayBuffer.getNumSamples();

            buffer.setSample(0, i, left);
            buffer.setSample(1, i, right);
        }
    }

    void Engine::triggerInstrument(Instrument instrument, float velocity)
    {
        StepEvent event;
        event.instrument = instrument;
        event.velocity = juce::jlimit(0.0f, 1.0f, velocity);
        event.flam = false;
        event.stepIndex = -1;

        const juce::SpinLock::ScopedLockType sl(pendingTriggerLock);
        pendingTriggers.add(event);
    }

    void Engine::setBpm(float bpm)
    {
        sequencer.setBpm(bpm);
    }

    void Engine::setRunning(bool running)
    {
        sequencer.setRunning(running);
    }

    bool Engine::isRunning() const
    {
        return sequencer.isRunning();
    }

    void Engine::setAccentLevel(float level)
    {
        accentLevel = juce::jlimit(0.0f, 1.0f, level);
    }

    float Engine::getAccentLevel() const
    {
        return accentLevel;
    }

    Sequencer& Engine::getSequencer()
    {
        return sequencer;
    }

    SampleLibrary& Engine::getSampleLibrary()
    {
        return sampleLibrary;
    }

    MixerChannel& Engine::getChannel(Instrument instrument)
    {
        return channels[(int)instrument];
    }

    void Engine::updateInstrumentSound(Instrument instrument)
    {
        sampleLibrary.regenerate(instrument, sampleRate, channels[(int)instrument].params);
    }

    float Engine::renderInstrument(Instrument instrument)
    {
        auto& list = voices[(int)instrument];
        if (list.isEmpty())
            return 0.0f;

        float sum = 0.0f;
        for (int i = list.size(); --i >= 0;)
        {
            auto& voice = list.getReference(i);
            if (!voice.sample || voice.position >= voice.sample->data.getNumSamples())
            {
                list.remove(i);
                continue;
            }

            sum += voice.sample->data.getSample(0, voice.position) * voice.gain;
            voice.position++;
        }

        return sum;
    }

    void Engine::triggerVoice(const StepEvent& event)
    {
        if (event.instrument == Instrument::ClosedHat)
            clearVoices(Instrument::OpenHat);

        VoiceInstance voice;
        voice.sample = &sampleLibrary.get(event.instrument);
        voice.position = 0;
        voice.accented = (event.velocity >= 0.95f);
        voice.gain = event.velocity * accentMultiplier(event.instrument, voice.accented);

        if (voice.accented && event.instrument == Instrument::Kick)
            kickThumpEnv = juce::jmax(kickThumpEnv, 0.55f + accentLevel * 0.65f);

        voices[(int)event.instrument].add(voice);
    }

    void Engine::applyAutomationForStep(Instrument instrument, int stepIndex)
    {
        if (stepIndex < 0 || stepIndex >= 16)
            return;

        auto& seq = getSequencer();
        auto& ch = channels[(int)instrument];

        float value = 0.0f;
        bool needsResynth = false;

        if (seq.getAutomationPoint(instrument, AutomationParam::Level, stepIndex, value))
            ch.level = value;
        if (seq.getAutomationPoint(instrument, AutomationParam::Tune, stepIndex, value))
        {
            ch.params.tune = value;
            needsResynth = true;
        }
        if (seq.getAutomationPoint(instrument, AutomationParam::Decay, stepIndex, value))
        {
            ch.params.decay = value;
            needsResynth = true;
        }
        if (seq.getAutomationPoint(instrument, AutomationParam::Tone, stepIndex, value))
        {
            ch.params.tone = value;
            needsResynth = true;
        }
        if (seq.getAutomationPoint(instrument, AutomationParam::Snappy, stepIndex, value))
        {
            ch.params.snappy = value;
            needsResynth = true;
        }

        if (needsResynth)
            updateInstrumentSound(instrument);
    }

    void Engine::clearVoices(Instrument instrument)
    {
        voices[(int)instrument].clearQuick();
    }

    void Engine::setupDelay(double newSampleRate)
    {
        // Reduced delay time: 180ms (was 350ms) for more subtle effect
        delaySamples = (int)(newSampleRate * 0.18);
        int bufferSize = (int)(newSampleRate * 1.0);
        delayBuffer.setSize(2, bufferSize);
        delayBuffer.clear();
        delayWritePos = 0;
    }

    float Engine::accentMultiplier(Instrument instrument, bool accented) const
    {
        if (!accented)
            return 1.0f;

        float boost = 1.0f + accentLevel * 0.35f;
        switch (instrument)
        {
            case Instrument::Kick:      boost = 1.0f + accentLevel * 0.72f; break;
            case Instrument::Snare:     boost = 1.0f + accentLevel * 0.42f; break;
            case Instrument::TomLow:
            case Instrument::TomMid:
            case Instrument::TomHigh:   boost = 1.0f + accentLevel * 0.38f; break;
            case Instrument::Clap:
            case Instrument::Rim:       boost = 1.0f + accentLevel * 0.32f; break;
            case Instrument::ClosedHat:
            case Instrument::OpenHat:
            case Instrument::Crash:
            case Instrument::Ride:      boost = 1.0f + accentLevel * 0.26f; break;
            default: break;
        }
        return boost;
    }

    float Engine::safeSaturate(float x, float drive) const
    {
        float y = x * drive;
        const float y2 = y * y;
        return y * (27.0f + y2) / (27.0f + 9.0f * y2);
    }
}
