#pragma once

#include <JuceHeader.h>
#include "Samples.h"

namespace rb338
{
    enum class AutomationParam
    {
        Level = 0,
        Tune,
        Decay,
        Tone,
        Snappy,
        Count
    };

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
        int stepIndex = 0;
    };

    class Sequencer
    {
    public:
        void prepare(double sampleRate);
        void setBpm(float bpm);
        void setShuffle(float amount); // 0.0 = no shuffle, 1.0 = max shuffle
        float getBpm() const;
        float getShuffle() const;
        void setRunning(bool shouldRun);
        bool isRunning() const;

        StepState getStep(Instrument instrument, int index) const;
        void setStep(Instrument instrument, int index, StepState state);
        void cycleStep(Instrument instrument, int index);
        void clear();

        void setLength(int steps);
        int getLength() const;

        int getCurrentStep() const;

        void setAutomationPoint(Instrument instrument, AutomationParam param, int step, float value);
        bool getAutomationPoint(Instrument instrument, AutomationParam param, int step, float& valueOut) const;
        bool hasAutomation(Instrument instrument) const;
        void clearAutomation(Instrument instrument);
        void clearAllAutomation();

        bool nextSample(juce::Array<StepEvent>& events);

    private:
        double sampleRate = 44100.0;
        float bpm = 125.0f;
        float shuffle = 0.0f; // 0-1 range
        bool running = false;
        int length = 16;
        int currentStep = 0;
        int samplesUntilNextStep = 0;
        float driftMemoryMs = 0.0f;
        juce::Random timingRng { 9099 };

        StepState grid[(int)Instrument::Count][16] = {};
        bool automationActive[(int)Instrument::Count][(int)AutomationParam::Count][16] = {};
        float automationValue[(int)Instrument::Count][(int)AutomationParam::Count][16] = {};

        int stepSamples() const;
        int getStepDelay(int step) const; // Returns shuffle delay for given step
        int getAnalogStepDrift(int step);
    };
}
