# 🥁 LoS.9x9 Rhythm Composer

<div align="center">

**A modern TR-909-style drum machine built with JUCE**

[![macOS](https://img.shields.io/badge/platform-macOS-lightgrey.svg?style=flat-square)](https://www.apple.com/macos/)
[![JUCE](https://img.shields.io/badge/framework-JUCE-orange.svg?style=flat-square)](https://juce.com/)
[![C++](https://img.shields.io/badge/language-C%2B%2B17-blue.svg?style=flat-square)](https://isocpp.org/)
[![License](https://img.shields.io/badge/license-MIT-green.svg?style=flat-square)](LICENSE)

*Authentic TR-909 analog synthesis • 16-step sequencer • Paint mode editing • Shuffle/swing timing*

[Features](#-features) • [Screenshots](#-screenshots) • [Installation](#-installation) • [Usage](#-usage) • [Technical Details](#-technical-details)

</div>

---

## 🎵 What is LoS.9x9?

**LoS.9x9 Rhythm Composer** is a standalone drum machine application inspired by the legendary **Roland TR-909** and the iconic **Propellerhead ReBirth RB-338**. Built from the ground up with modern C++ and JUCE, it recreates the authentic sound and workflow of classic drum machines while adding modern conveniences like paint mode editing, shuffle control, and accent level adjustment.

### 🎯 Design Philosophy

- **Authentic Sound** - Meticulously modeled TR-909 synthesis with filtered noise, 6-bit quantization, and analog-style character
- **Intuitive Workflow** - Paint mode sequencer, double-click to reset knobs, one-click accent programming
- **Visual Fidelity** - Faithful recreation of the classic aesthetic with metallic knobs, LED indicators, and retro LCD display
- **Modern Architecture** - Clean C++ codebase, JUCE framework, sample-accurate timing

---

## ✨ Features

### 🎛️ Sequencer & Playback

- **16-step pattern sequencer** with 11 classic TR-909 instruments
- **Paint mode editing** - drag across steps to fill/erase patterns instantly
- **Accent programming** - one-click accent hits with adjustable boost level
- **Accent level control** - TR-909 style accent knob (0-50% boost)
- **Shuffle/swing control** - 0-100% triplet feel for groove and timing variation
- **Green LCD display** - Classic green display (Red, Amber, Blue, Cyan, White also available)
- **Dark mode support** - Switch between light and dark panel themes
- **Expandable grid view** - see all 11 instruments × 16 steps simultaneously
- **BPM control** - 60-180 BPM with mouse wheel or drag adjustment

### 🥁 Instruments

Each instrument uses **authentic analog modeling techniques**:

| Instrument | Synthesis Method | Character |
|------------|------------------|-----------|
| **Bass Drum** (BD) | Dual-oscillator pitch sweep | Deep, punchy 909 kick |
| **Snare Drum** (SD) | Triangle oscillators + filtered noise | Crisp, snappy snare |
| **Low/Mid/Hi Tom** (LT/MT/HT) | Triangle oscillators with pitch envelope | Resonant, tonal toms |
| **Rim Shot** (RS) | Tonal click + filtered noise | Tight, woody rimshot |
| **Hand Clap** (CP) | Multi-strike filtered noise bursts | Classic 909 clap |
| **Closed Hi-Hat** (CH) | Metallic filtered noise, 6-bit quantized | Crisp, tight hi-hat |
| **Open Hi-Hat** (OH) | Extended decay filtered noise | Sustained, shimmering hat |
| **Crash Cymbal** (CR) | Wide-band filtered noise | Splashy, long decay |
| **Ride Cymbal** (RD) | Bell-like filtered noise | Defined, articulate ride |

### 🎚️ Mixer & Effects

- **Per-instrument mixer** with level, pan, and delay send controls
- **Tune/Tone knobs** for real-time sound shaping (kick, snare, toms, hi-hats)
- **Stereo delay effect** with 180ms delay time and subtle feedback
- **Sample loading** - replace any instrument with your own .wav/.aif/.aiff samples

### 💡 User Interface

- **Double-click knobs** to reset to default values (0.5 center)
- **Space bar** to start/stop playback
- **PANEL button** to toggle grid view (collapsed/expanded)
- **LCD display** with BPM, pattern info, and green display
- **Shuffle & Accent knobs** next to LCD for quick access
- **Retro aesthetic** with beige panels (or dark mode), orange accents, and LED indicators
- **Optimized spacing** - all knob labels clearly visible with proper padding

---

## 📸 Screenshots

### Collapsed View - Step Button Interface
<img src="DOCUMENTS/Stitch LoS9x9/los9x9_Main/screen.png" alt="Collapsed View" width="440">

*Compact view showing header knobs, step buttons 1-16, and transport controls*

### Expanded View - Full 11×16 Grid
<img src="DOCUMENTS/Stitch LoS9x9/los9x9_Extended/screen.png" alt="Expanded Grid View" width="440">

*Full sequencer grid with all instruments visible - perfect for pattern overview and paint mode editing*

---

## 🛠️ Installation

### Prerequisites

- **macOS** (10.13 or later recommended)
- **CMake** 3.22+
- **Xcode Command Line Tools**

```bash
# Install Xcode Command Line Tools (if not already installed)
xcode-select --install
```

### Build from Source

```bash
# 1. Clone the repository
git clone https://github.com/CarlosFranzetti/LoS.9x9.git
cd LoS.9x9

# 2. Initialize JUCE submodule
git submodule update --init --recursive

# 3. Build with CMake
cmake -S . -B build
cmake --build build

# 4. Launch the app
open "build/LoS9x9_artefacts/Debug/LoS9x9.app"
```

### Clean Build

```bash
# Remove build directory and rebuild from scratch
rm -rf build
cmake -S . -B build
cmake --build build
```

---

## 🎮 Usage

### Quick Start

1. **Launch** the application
2. **Click** step buttons to program a pattern (off → accent → off)
3. **Drag** across steps to paint/erase patterns quickly
4. **Press SPACE** or click START to begin playback
5. **Adjust knobs** to shape the sound of each instrument
6. **Turn the SHUFFLE knob** to add groove and swing

### Sequencer Controls

| Action | Method |
|--------|--------|
| **Program step** | Click step button |
| **Paint mode** | Drag across multiple steps |
| **Accent hit** | Click once (goes straight to accent) |
| **Clear step** | Click accent step to turn off |
| **Select instrument** | Click instrument label (BD, SD, etc.) |
| **Toggle grid view** | Click PANEL button |

### Mixing & Sound Design

| Parameter | Control | Range |
|-----------|---------|-------|
| **BPM** | Mouse wheel or drag LCD | 60-180 BPM |
| **Shuffle** | Rotary knob next to BPM | 0-100% (straight → triplet) |
| **Accent Level** | Rotary knob next to Shuffle | 0-50% volume boost for accents |
| **Tune** | Instrument knobs | Pitch adjustment |
| **Tone** | Instrument knobs | Brightness/filter |
| **Decay** | Instrument knobs | Sustain length |
| **Level** | Instrument knobs | Volume |
| **Pan** | Instrument knobs | Stereo position |
| **Send** | Instrument knobs | Delay amount |

### Keyboard Shortcuts

- **SPACE** - Start/Stop playback
- **Double-click knob** - Reset to default value

---

## 🔧 Technical Details

### Architecture

```
┌─────────────────────────────────────────────────┐
│              MainComponent (UI)                  │
│  ┌──────────────────────────────────────────┐  │
│  │ LCDDisplay │ Transport │ InstrumentKnobs │  │
│  │ StepButtons │ SequencerGrid │ StatusBar  │  │
│  └──────────────────────────────────────────┘  │
└────────────────┬────────────────────────────────┘
                 │
┌────────────────▼────────────────────────────────┐
│                  Engine                          │
│  ┌──────────────────────────────────────────┐  │
│  │  Sequencer  │  SampleLibrary  │  Mixer   │  │
│  │  VoicePool  │  DelayEffect    │          │  │
│  └──────────────────────────────────────────┘  │
└─────────────────────────────────────────────────┘
```

### Synthesis Techniques

#### Hi-Hat Synthesis (Authentic TR-909 Modeling)
```cpp
// Metallic filtered noise with 6-bit character
noise = whiteNoise();
filtered = bandPassFilter(noise, 6-10kHz);
shaped = highPassFilter(filtered, >1kHz);  // Remove low-end boom
quantized = 6bitQuantization(shaped);      // Lo-fi character
output = quantized * fastDecay(45Hz);      // Crisp, tight
```

#### Bass Drum Synthesis
```cpp
// Dual oscillator with pitch envelope
osc1 = sine(40-80Hz + pitchSweep);
osc2 = sine(osc1 * 2.0);
output = (osc1*0.7 + osc2*0.3 + click) * envelope;
```

### Audio Specifications

- **Sample Rate** - 44.1kHz (configurable)
- **Bit Depth** - 32-bit float internal, 6-bit emulation for cymbals
- **Latency** - Sample-accurate timing, ~10ms typical round-trip
- **Channels** - Stereo output
- **DSP** - Custom TR-909 analog modeling + delay effect

### File Support

- **Sample Loading** - WAV, AIF, AIFF (mono/stereo)
- **Project** - Pattern storage (future phase)
- **MIDI** - MIDI sync (future phase)

---

## 🎨 Design & Aesthetic

The UI faithfully recreates the **TR-909/ReBirth aesthetic** with:

- **Color Palette**
  - Beige panels: `#d8d8d2`
  - Main grey: `#c8c8c0`
  - Orange accents: `#ff6a00`
  - Dark details: `#4a4a4a`

- **Typography**
  - Headers: Orbitron (bold, geometric)
  - Body: Inter (clean, readable)
  - Code: JetBrains Mono (monospace)

- **Components**
  - 3D metallic rotary knobs with drop shadows
  - LED indicators (red #ff3300)
  - LCD display (dark #1a1a1a, red text)
  - Mechanical push buttons with gradients

---

## 📚 Project Structure

```
LoS.9x9/
├── Source/
│   ├── Main.cpp           # UI components, MainWindow, Application
│   ├── Engine.cpp/h       # Audio engine, mixer, voice management
│   ├── Sequencer.cpp/h    # 16-step pattern sequencer, timing
│   └── Samples.cpp/h      # TR-909 synthesis algorithms
├── DOCUMENTS/             # Historical references, mockups
├── external/JUCE/         # JUCE framework (submodule)
├── CMakeLists.txt         # Build configuration
├── CLAUDE.md              # Development guidelines
└── README.md              # This file
```

### Technical References

- **Roland TR-909** - Original analog drum machine (1983-1984)
- **JUCE Framework** - Modern C++ audio application framework
- **ReBirth RB-338** - Software emulation that inspired this project

### Development

- Built with ❤️ using **JUCE** and **C++17**
- TR-909 synthesis algorithms based on historical analysis and circuit modeling
- UI design inspired by original Roland hardware and Propellerhead software

---

## 🗺️ Roadmap

### Phase 1: TR-909 Module ✅ (Complete)
- [x] 16-step sequencer with 11 instruments
- [x] Authentic TR-909 synthesis with enhanced hi-hats
- [x] Paint mode editing (drag to fill/erase)
- [x] Shuffle/swing control (0-100%)
- [x] Accent level control (TR-909 style)
- [x] Per-instrument mixer with tune/tone controls
- [x] Sample loading support (.wav, .aif, .aiff)
- [x] Green LCD display with color options
- [x] Dark mode theme support
- [x] Double-click knob reset
- [x] Improved UI spacing and legibility

### Phase 2: TB-303 Module (Planned)
- [ ] Acid bassline sequencer
- [ ] Resonant filter with envelope
- [ ] Slide/accent programming
- [ ] Pattern chaining

### Phase 3: TR-808 Module (Planned)
- [ ] Additional 808 instruments
- [ ] Separate 808 sequencer
- [ ] Classic 808 synthesis

### Phase 4: Advanced Features (Future)
- [ ] Pattern save/load
- [ ] MIDI clock sync
- [ ] Audio export
- [ ] VST/AU plugin version

---

## 🤝 Contributing

This is a personal project, but suggestions and bug reports are welcome! Feel free to:
- Open an issue for bugs or feature requests
- Share your patterns and sounds
- Provide feedback on synthesis accuracy

---

## 📄 License

MIT License - See LICENSE file for details

---

## 🔗 Links

- **JUCE Framework** - [juce.com](https://juce.com/)
- **Roland TR-909** - [Wikipedia](https://en.wikipedia.org/wiki/Roland_TR-909)

---

<div align="center">

**Made with 🎶 by LoS • Built with JUCE**

*Bringing the classic drum machine sound to modern macOS*

</div>
