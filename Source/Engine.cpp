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
    }

    void Engine::render(juce::AudioBuffer<float>& buffer, int numSamples)
    {
        buffer.clear();
        juce::Array<StepEvent> events;

        for (int i = 0; i < numSamples; ++i)
        {
            if (sequencer.nextSample(events))
            {
                for (auto& event : events)
                {
                    if (event.instrument == Instrument::ClosedHat)
                        clearVoices(Instrument::OpenHat);

                    VoiceInstance voice;
                    voice.sample = &sampleLibrary.get(event.instrument);
                    voice.position = 0;
                    voice.gain = event.velocity;
                    voices[(int)event.instrument].add(voice);
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

            delayWritePos = (delayWritePos + 1) % delayBuffer.getNumSamples();

            buffer.setSample(0, i, left);
            buffer.setSample(1, i, right);
        }
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
}
