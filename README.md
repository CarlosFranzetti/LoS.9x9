# RB-338 Renewed (Phase 1: TR-909 Module)

Standalone JUCE macOS app with a 909-style step sequencer, per-instrument levels/pan, and a simple delay send. Default sounds are synthesized placeholders and can be replaced per instrument by loading WAV/AIFF samples.

## Build

Prereqs:
- CMake 3.22+
- Xcode command line tools

```sh
cmake -S . -B build
cmake --build build
```

## Run

The app binary will be in `build/` after compilation. Launch it from Finder or the build output path.

## Notes

- This phase focuses on sequencing + playback + mixer strip.
- Sample loading supports `.wav`, `.aif`, `.aiff`.
