# AGI Prototype

This repository is a small C++20 prototype for exploring actions and a
Plutchik-inspired emotion model through a local web interface. It is not a
general-intelligence implementation.

## Requirements

- CMake 3.16 or newer
- A C++20 compiler
- [Crow](https://crowcpp.org/) with its CMake package
- RapidJSON with its CMake package

## Build and test

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## Run

```sh
./build/AGI
```

Then open <http://127.0.0.1:18080>. The executable loads and validates
`data/Soul/EmotionData.json` at startup. Resource paths are embedded by CMake,
so the executable can be launched from any working directory.

## Structure

- `src/AGI.cpp`: Crow server and HTTP routes
- `src/EmotionData.cpp`: validated JSON loading
- `include/Soul/Emotion`: emotion domain model
- `include/Body/Action/verbs.h`: action vocabulary
- `templates`: Mustache HTML templates
- `tests`: lightweight domain and data tests

The remaining Body, Soul, and Spirit headers are placeholders for future domain
work and are not part of the executable yet.
