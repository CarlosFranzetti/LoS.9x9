#include "Sequencer.h"

namespace rb338
{
    void Sequencer::prepare(double newSampleRate)
    {
        sampleRate = newSampleRate;
        samplesUntilNextStep = stepSamples();
        driftMemoryMs = 0.0f;
    }

    void Sequencer::setBpm(float newBpm)
    {
        const float oldBpm = bpm;
        bpm = juce::jlimit(40.0f, 200.0f, newBpm);

        // Keep playback continuous when changing tempo mid-run by scaling the remaining
        // time to the next step instead of hard-resetting the transport phase.
        if (running && oldBpm > 0.0f)
        {
            const double oldStepSamps = (60.0 / oldBpm) / 4.0 * sampleRate;
            const double newStepSamps = (60.0 / bpm) / 4.0 * sampleRate;
            if (oldStepSamps > 0.0)
                samplesUntilNextStep = juce::jmax(1, (int)std::round(samplesUntilNextStep * (newStepSamps / oldStepSamps)));
        }
        else
        {
            samplesUntilNextStep = stepSamples();
        }
    }

    void Sequencer::setShuffle(float amount)
    {
        shuffle = juce::jlimit(0.0f, 1.0f, amount);
    }

    float Sequencer::getBpm() const
    {
        return bpm;
    }

    float Sequencer::getShuffle() const
    {
        return shuffle;
    }

    void Sequencer::setRunning(bool shouldRun)
    {
        running = shouldRun;
        if (running)
        {
            samplesUntilNextStep = stepSamples();
            currentStep = 0;
            driftMemoryMs = 0.0f;
        }
    }

    bool Sequencer::isRunning() const
    {
        return running;
    }

    StepState Sequencer::getStep(Instrument instrument, int index) const
    {
        if (index < 0 || index >= 16)
            return StepState::Off;
        return grid[(int)instrument][index];
    }

    void Sequencer::setStep(Instrument instrument, int index, StepState state)
    {
        if (index < 0 || index >= 16)
            return;
        grid[(int)instrument][index] = state;
    }

    void Sequencer::cycleStep(Instrument instrument, int index)
    {
        if (index < 0 || index >= 16)
            return;

        auto& state = grid[(int)instrument][index];
        if (state == StepState::Off)
            state = StepState::On;
        else if (state == StepState::On)
            state = StepState::Accent;
        else
            state = StepState::Off;
    }

    void Sequencer::clear()
    {
        for (int inst = 0; inst < (int)Instrument::Count; ++inst)
            for (int i = 0; i < 16; ++i)
                grid[inst][i] = StepState::Off;
    }

    void Sequencer::setLength(int steps)
    {
        juce::ignoreUnused(steps);
        length = 16;
    }

    int Sequencer::getLength() const
    {
        return length;
    }

    int Sequencer::getCurrentStep() const
    {
        return currentStep;
    }

    void Sequencer::setAutomationPoint(Instrument instrument, AutomationParam param, int step, float value)
    {
        if (step < 0 || step >= 16)
            return;

        auto instIndex = (int)instrument;
        auto paramIndex = (int)param;
        if (instIndex < 0 || instIndex >= (int)Instrument::Count
            || paramIndex < 0 || paramIndex >= (int)AutomationParam::Count)
            return;

        automationActive[instIndex][paramIndex][step] = true;
        automationValue[instIndex][paramIndex][step] = juce::jlimit(0.0f, 1.0f, value);
    }

    bool Sequencer::getAutomationPoint(Instrument instrument, AutomationParam param, int step, float& valueOut) const
    {
        if (step < 0 || step >= 16)
            return false;

        auto instIndex = (int)instrument;
        auto paramIndex = (int)param;
        if (instIndex < 0 || instIndex >= (int)Instrument::Count
            || paramIndex < 0 || paramIndex >= (int)AutomationParam::Count)
            return false;

        if (!automationActive[instIndex][paramIndex][step])
            return false;

        valueOut = automationValue[instIndex][paramIndex][step];
        return true;
    }

    bool Sequencer::hasAutomation(Instrument instrument) const
    {
        auto instIndex = (int)instrument;
        if (instIndex < 0 || instIndex >= (int)Instrument::Count)
            return false;

        for (int param = 0; param < (int)AutomationParam::Count; ++param)
            for (int step = 0; step < 16; ++step)
                if (automationActive[instIndex][param][step])
                    return true;

        return false;
    }

    void Sequencer::clearAutomation(Instrument instrument)
    {
        auto instIndex = (int)instrument;
        if (instIndex < 0 || instIndex >= (int)Instrument::Count)
            return;

        for (int param = 0; param < (int)AutomationParam::Count; ++param)
            for (int step = 0; step < 16; ++step)
            {
                automationActive[instIndex][param][step] = false;
                automationValue[instIndex][param][step] = 0.0f;
            }
    }

    void Sequencer::clearAllAutomation()
    {
        for (int inst = 0; inst < (int)Instrument::Count; ++inst)
            clearAutomation((Instrument)inst);
    }

    bool Sequencer::nextSample(juce::Array<StepEvent>& events)
    {
        events.clear();

        if (!running)
            return false;

        samplesUntilNextStep--;
        if (samplesUntilNextStep > 0)
            return false;

        const int baseStep = stepSamples();
        const int shuffleDelay = getStepDelay(currentStep);
        const int analogDrift = getAnalogStepDrift(currentStep);
        samplesUntilNextStep = juce::jmax(1, baseStep + shuffleDelay + analogDrift);

        for (int inst = 0; inst < (int)Instrument::Count; ++inst)
        {
            auto state = grid[inst][currentStep];
            if (state != StepState::Off)
            {
                StepEvent event;
                event.instrument = (Instrument)inst;
                event.velocity = (state == StepState::Accent) ? 1.0f : 0.78f;
                event.flam = false;
                event.stepIndex = currentStep;
                events.add(event);
            }
        }

        currentStep = (currentStep + 1) % length;
        return !events.isEmpty();
    }

    int Sequencer::stepSamples() const
    {
        double secondsPerBeat = 60.0 / bpm;
        double secondsPerStep = secondsPerBeat / 4.0;
        return (int)std::round(secondsPerStep * sampleRate);
    }

    int Sequencer::getStepDelay(int step) const
    {
        // Shuffle affects even-numbered steps (1, 3, 5, 7, etc. in 0-based indexing)
        // by delaying them slightly
        if (shuffle == 0.0f || step % 2 == 0)
            return 0;

        // Maximum shuffle delay is 33% of step length (triplet feel at max)
        int baseStep = stepSamples();
        return (int)(baseStep * shuffle * 0.33f);
    }

    int Sequencer::getAnalogStepDrift(int step)
    {
        // Subtle analog-style clock wander: tiny and structured, only every 4th step.
        if (step % 4 != 0)
            return 0;

        const float targetMs = (timingRng.nextFloat() * 2.0f - 1.0f) * 1.0f; // +/- 1 ms max
        driftMemoryMs = driftMemoryMs * 0.65f + targetMs * 0.35f;
        const float clampedMs = juce::jlimit(-1.0f, 1.0f, driftMemoryMs);
        return (int)std::round(clampedMs * 0.001f * (float)sampleRate);
    }
}
