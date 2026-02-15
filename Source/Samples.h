#pragma once

#include <JuceHeader.h>

namespace rb338
{
    enum class Instrument
    {
        Kick,
        Snare,
        Clap,
        Rim,
        TomLow,
        TomMid,
        TomHigh,
        ClosedHat,
        OpenHat,
        Crash,
        Ride,
        Count
    };

    struct Sample
    {
        juce::AudioBuffer<float> data;
        double sampleRate = 44100.0;
    };

    // Per-instrument parameters (TR-909 style)
    struct InstrumentParams
    {
        float tune = 0.5f;      // 0-1 range
        float decay = 0.5f;     // 0-1 range
        float snappy = 0.5f;    // 0-1 range (snare)
        float tone = 0.5f;      // 0-1 range
    };

    class SampleLibrary
    {
    public:
        void prepare(double sampleRate);
        const Sample& get(Instrument instrument) const;
        bool loadFromFile(Instrument instrument, const juce::File& file);
        void regenerate(Instrument instrument, double sampleRate, const InstrumentParams& params);

    private:
        Sample samples[(int)Instrument::Count];
        void generateDefaults(double sampleRate);

        Sample generateKick(double sampleRate, const InstrumentParams& params) const;
        Sample generateSnare(double sampleRate, const InstrumentParams& params) const;
        Sample generateClap(double sampleRate, const InstrumentParams& params) const;
        Sample generateRim(double sampleRate, const InstrumentParams& params) const;
        Sample generateTom(double sampleRate, float baseFreq, const InstrumentParams& params) const;
        Sample generateHat(double sampleRate, bool open, const InstrumentParams& params) const;
        Sample generateCrash(double sampleRate, const InstrumentParams& params) const;
        Sample generateRide(double sampleRate, const InstrumentParams& params) const;
    };
}
