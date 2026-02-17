#pragma once

#include <JuceHeader.h>
#include "Samples.h"
#include "Sequencer.h"

namespace rb338
{
    struct MixerChannel
    {
        float level = 0.9f;
        float pan = 0.0f; // -1..1
        float delaySend = 0.0f;
        InstrumentParams params; // TR-909 voice parameters
    };

    class Engine
    {
    public:
        void prepare(double sampleRate, int samplesPerBlock, int numOutputs);
        void render(juce::AudioBuffer<float>& buffer, int numSamples);
        void triggerInstrument(Instrument instrument, float velocity = 1.0f);

        void setBpm(float bpm);
        void setRunning(bool running);
        bool isRunning() const;
        void setAccentLevel(float level); // 0-1 range, controls accent volume boost
        float getAccentLevel() const;

        Sequencer& getSequencer();
        SampleLibrary& getSampleLibrary();

        MixerChannel& getChannel(Instrument instrument);
        void updateInstrumentSound(Instrument instrument);

    private:
        struct VoiceInstance
        {
            const Sample* sample = nullptr;
            int position = 0;
            float gain = 1.0f;
            bool accented = false;
        };

        double sampleRate = 44100.0;
        SampleLibrary sampleLibrary;
        Sequencer sequencer;
        float accentLevel = 0.5f; // TR-909 style accent control (0-1)

        juce::Array<VoiceInstance> voices[(int)Instrument::Count];
        MixerChannel channels[(int)Instrument::Count];

        juce::AudioBuffer<float> delayBuffer;
        int delayWritePos = 0;
        int delaySamples = 1;
        float delayFeedback = 0.15f;  // Reduced from 0.35 (less prominent)
        float delayMix = 0.08f;        // Reduced from 0.2 (subtle effect)
        float kickThumpEnv = 0.0f;
        float kickThumpPhase = 0.0f;
        juce::SpinLock pendingTriggerLock;
        juce::Array<StepEvent> pendingTriggers;

        float renderInstrument(Instrument instrument);
        void triggerVoice(const StepEvent& event);
        void applyAutomationForStep(Instrument instrument, int stepIndex);
        void clearVoices(Instrument instrument);
        void setupDelay(double sampleRate);
        float accentMultiplier(Instrument instrument, bool accented) const;
        float safeSaturate(float x, float drive) const;
    };
}
