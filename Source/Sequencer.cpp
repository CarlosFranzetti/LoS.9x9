#include "Sequencer.h"

namespace rb338
{
    void Sequencer::prepare(double newSampleRate)
    {
        sampleRate = newSampleRate;
        samplesUntilNextStep = stepSamples();
    }

    void Sequencer::setBpm(float newBpm)
    {
        bpm = juce::jlimit(40.0f, 200.0f, newBpm);
        samplesUntilNextStep = stepSamples();
    }

    void Sequencer::setShuffle(float amount)
    {
        shuffle = juce::jlimit(0.0f, 1.0f, amount);
    }

    void Sequencer::setRunning(bool shouldRun)
    {
        running = shouldRun;
        if (running)
        {
            samplesUntilNextStep = stepSamples();
            currentStep = 0;
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
            state = StepState::Accent;  // First click goes to Accent
        else
            state = StepState::Off;      // Second click turns off
    }

    void Sequencer::clear()
    {
        for (int inst = 0; inst < (int)Instrument::Count; ++inst)
            for (int i = 0; i < 16; ++i)
                grid[inst][i] = StepState::Off;
    }

    void Sequencer::setLength(int steps)
    {
        length = juce::jlimit(1, 16, steps);
    }

    int Sequencer::getLength() const
    {
        return length;
    }

    int Sequencer::getCurrentStep() const
    {
        return currentStep;
    }

    bool Sequencer::nextSample(juce::Array<StepEvent>& events)
    {
        events.clear();

        if (!running)
            return false;

        samplesUntilNextStep--;
        if (samplesUntilNextStep > 0)
            return false;

        samplesUntilNextStep = stepSamples() + getStepDelay(currentStep);

        for (int inst = 0; inst < (int)Instrument::Count; ++inst)
        {
            auto state = grid[inst][currentStep];
            if (state != StepState::Off)
            {
                StepEvent event;
                event.instrument = (Instrument)inst;
                event.velocity = (state == StepState::Accent) ? 1.0f : 0.7f;
                event.flam = false;
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
}
