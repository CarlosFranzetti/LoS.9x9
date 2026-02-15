# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

RB-338 Renewed is a standalone JUCE macOS application implementing a TR-909-style drum machine with:
- 16-step sequencer for 11 instruments (Kick, Snare, Clap, Rim, 3 Toms, Closed/Open Hat, Crash, Ride)
- Per-instrument mixer with level, pan, and delay send controls
- Synthesized placeholder sounds that can be replaced via sample loading (.wav, .aif, .aiff)
- Simple stereo delay effect

**Naming:** The application displays as "LoS.9x9 Rhythm Composer" in the UI, while the project/build artifacts use "RB-338 Renewed".

This is Phase 1, focused on sequencing, playback, and mixer functionality.

## Build & Run

**Prerequisites:**
- CMake 3.22+
- Xcode command line tools

**First time setup:**
```sh
# Initialize JUCE submodule
git submodule update --init --recursive
```

**Build:**
```sh
cmake -S . -B build
cmake --build build
```

**Run:**
The app binary is generated in `build/RB338Renewed_artefacts/Debug/` (or `Release/` depending on configuration). Launch from Finder or directly from the build output path.

**Clean build:**
```sh
rm -rf build
cmake -S . -B build
cmake --build build
```

## Architecture

### Core Components

The application follows a layered architecture with clear separation of concerns:

**Engine** (`Source/Engine.h/cpp`)
- Top-level audio engine coordinating all subsystems
- Owns and manages `Sequencer` and `SampleLibrary` instances
- Handles voice management per instrument (polyphonic playback)
- Implements mixer channels with level/pan/delay send processing
- Renders final stereo output with delay effect
- Delay implementation: circular buffer with configurable time (350ms), feedback (0.35), and mix (0.2)

**Sequencer** (`Source/Sequencer.h/cpp`)
- 16-step pattern grid for 11 instruments
- Three step states: Off, On (normal velocity 0.7), Accent (full velocity 1.0)
- BPM-based timing: converts tempo to sample counts for 16th-note steps
- Outputs `StepEvent` objects containing instrument and velocity when steps trigger
- Pattern playback wraps at configurable length (default 16 steps)

**SampleLibrary** (`Source/Samples.h/cpp`)
- Manages one `Sample` per instrument (contains `AudioBuffer<float>` and sample rate)
- Generates synthesized default sounds for all 11 instruments using JUCE DSP
- Supports loading user samples via `loadFromFile()` to replace defaults
- Sample playback is one-shot with no looping

**Main** (`Source/Main.cpp`)
- JUCE application entry point and GUI implementation
- `MainComponent`: audio callback host, owns the `Engine`, creates UI grid
- `StepButton`: custom button that cycles through Off → On → Accent → Off when clicked
- UI refreshes at 30Hz to show current step position with brightness modulation
- Closed hat automatically cuts open hat voices (choke group behavior)

### UI Components

The application features a custom TR-909-inspired interface with several specialized components:

**LookAndFeel909** (in `Main.cpp`)
- Custom JUCE look and feel implementing TR-909 visual styling
- Dark backgrounds, metallic sliders, custom knob rendering
- Color scheme matches classic Roland aesthetic

**LCDDisplay** (in `Main.cpp`)
- BPM display with interactive controls
- Mouse wheel or vertical drag to adjust tempo (60-180 BPM)
- LCD-style segmented font rendering

**InstrumentSection** (in `Main.cpp`)
- Knob groups for per-instrument parameters (tune, decay, level, pan, send)
- Live sound regeneration when synthesis parameters change
- Direct connection to `Engine` for real-time parameter updates

**SequencerGrid** (in `Main.cpp`)
- Expanded 11×16 grid view with full pattern visibility
- Paint-style editing: drag to paint/erase steps across instruments
- Shows/hides via PANEL button toggle

**StepButton909** (in `Main.cpp`)
- Individual step buttons with LED-style indicators
- Visual feedback for step state (Off/On/Accent)
- Current step highlighting with brightness modulation

**UI Refresh:**
- 30Hz timer drives visual updates
- Current step position synchronized with sequencer
- LED brightness reflects playback state

### Key Design Patterns

**Audio Thread Safety:**
- `Engine::render()` is called from the audio thread
- UI controls (sliders, buttons) directly modify engine state (acceptable for this simple app)
- No explicit locking currently used; JUCE handles basic thread safety for simple types

**Voice Management:**
- Each instrument has an array of `VoiceInstance` objects (struct with sample pointer, playback position, gain)
- When a step triggers, a new voice is added to the instrument's array
- Voices auto-remove when playback completes (position >= sample length)
- Simple polyphonic: multiple triggers can overlap

**Step Sequencer Timing:**
- Sample-accurate timing: `Sequencer::nextSample()` called per audio sample
- Counts down `samplesUntilNextStep` and emits events when reaching zero
- Formula: `stepSamples = (60.0 / bpm) / 4.0 * sampleRate` (16th notes at 4/4 time)

**Mixer Signal Flow:**
1. Render each instrument to mono
2. Apply level and stereo pan (equal-power panning using sin/cos)
3. Sum to stereo output
4. Route scaled signal to delay send
5. Mix delayed signal back with dry signal

## Namespace

All code lives in the `rb338` namespace.

## User Interaction Patterns

**Panel Toggle:**
- PANEL button shows/hides the full 11×16 sequencer grid
- Collapsed mode shows only currently selected instrument's 16 steps
- Expanded mode shows all instruments simultaneously for pattern overview

**Instrument Selection:**
- Click instrument label to switch active instrument for step buttons
- Selected instrument's parameters shown in right panel

**Step Editing:**
- Click step button: cycles Off → On → Accent → Off
- Grid paint mode: drag across steps to paint or erase
- Current step highlighted during playback

**BPM Control:**
- Mouse wheel on LCD display adjusts tempo
- Vertical drag on LCD also controls BPM
- Range: 60-180 BPM

**Transport:**
- Space bar toggles playback (when implemented)
- START/STOP buttons control sequencer
- Pattern loops at configured length (default 16 steps)

## Developer Resources

**Reference Materials:**
- `DOCUMENTS/` folder contains historical background on ReBirth RB-338
- `los9x9_Extended.html` and `los9x9_Main.html`: UI design mockups
- These files provide context for the TR-909 interface recreation

## Important Notes

- The application is macOS-only (uses `juce_add_gui_app`)
- JUCE is included as a git submodule at `external/JUCE/`
- Default synth sounds are simple waveform-based approximations (not TR-909 accurate)
- No preset saving/loading yet (Phase 1 scope limitation)
- No MIDI sync or external clock (internal BPM only)
