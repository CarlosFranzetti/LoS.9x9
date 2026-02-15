#pragma once

#include <JuceHeader.h>
#include "Samples.h"

namespace rb338
{
    enum class StepState
    {
        Off = 0,
        On = 1,
        Accent = 2
    };

    struct StepEvent
    {
        Instrument instrument;
        float velocity = 1.0f;
        bool flam = false;
    };

    class Sequencer
    {
    public:
        void prepare(double sampleRate);
        void setBpm(float bpm);
        void setShuffle(float amount); // 0.0 = no shuffle, 1.0 = max shuffle
        void setRunning(bool shouldRun);
        bool isRunning() const;

        StepState getStep(Instrument instrument, int index) const;
        void setStep(Instrument instrument, int index, StepState state);
        void cycleStep(Instrument instrument, int index);
        void clear();

        void setLength(int steps);
        int getLength() const;

        int getCurrentStep() const;

        bool nextSample(juce::Array<StepEvent>& events);

    private:
        double sampleRate = 44100.0;
        float bpm = 125.0f;
        float shuffle = 0.0f; // 0-1 range
        bool running = false;
        int length = 16;
        int currentStep = 0;
        int samplesUntilNextStep = 0;

        StepState grid[(int)Instrument::Count][16] = {};

        int stepSamples() const;
        int getStepDelay(int step) const; // Returns shuffle delay for given step
    };
}
