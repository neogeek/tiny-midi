> [!CAUTION]
> This package is in early development and should not be used in production.

# TinyMidi

[![Build](https://github.com/neogeek/tiny-midi/actions/workflows/build.workflow.yml/badge.svg)](https://github.com/neogeek/tiny-midi/actions/workflows/build.workflow.yml)
[![Join the chat at https://discord.gg/nNtFsfd](https://img.shields.io/badge/discord-join%20chat-7289DA.svg)](https://discord.gg/nNtFsfd)

## Install

### Unity

Add package via git URL `https://github.com/neogeek/tiny-midi.git?path=/UnityPackage`.

### Unreal

Coming soon.

### Godot

Coming soon.

### HandcrankEngine

Coming soon.

## Usage

### C++

```cpp
#include <csignal>
#include <iostream>
#include <thread>

#include "TinyMidi/Input.hpp"

void signalHandler(int signum)
{
    TinyMidi::Stop();

    exit(signum);
}

int main()
{
    TinyMidi::Setup();

    TinyMidi::AddEventListener(
        [](TinyMidi::MidiEvent midiEvent)
        {
            switch (midiEvent.status)
            {
            case TinyMidi::MidiEventStatus::NoteOn:
                std::cout << "Note Down: " << midiEvent.controllerNumber
                          << " Velocity: " << midiEvent.value
                          << " Device Index: " << midiEvent.deviceIndex
                          << std::endl;
                break;
            case TinyMidi::MidiEventStatus::NoteOff:
                std::cout << "Note Up: " << midiEvent.controllerNumber
                          << " Velocity: " << midiEvent.value
                          << " Device Index: " << midiEvent.deviceIndex
                          << std::endl;
                break;
            case TinyMidi::MidiEventStatus::ControlChange:
                std::cout << "Control Changed: " << midiEvent.controllerNumber
                          << " Value: " << midiEvent.value
                          << " Device Index: " << midiEvent.deviceIndex
                          << std::endl;
                break;
            }
        });

    TinyMidi::Start();

    std::signal(SIGINT, signalHandler);

    while (true)
    {
        std::this_thread::yield();
    }

    return 0;
}
```

### Unity C#

```csharp
using UnityEngine;

public class TinyMidiTest : MonoBehaviour
{
    private void Start()
    {
        TinyMidi.Input.Setup();

        TinyMidi.Input.AddEventListener(midiEvent =>
        {
            switch (midiEvent.status)
            {
                case TinyMidi.MidiEventStatus.NoteOn:
                    Debug.Log(
                        $"Note Down: {midiEvent.controllerNumber}" +
                        $" Velocity: {midiEvent.value}" +
                        $" Device Index: {midiEvent.deviceIndex}");

                    break;
                case TinyMidi.MidiEventStatus.NoteOff:
                    Debug.Log(
                        $"Note Up: {midiEvent.controllerNumber}" +
                        $" Velocity: {midiEvent.value}" +
                        $" Device Index: {midiEvent.deviceIndex}");

                    break;
                case TinyMidi.MidiEventStatus.ControlChange:
                    Debug.Log(
                        $"Control Changed: {midiEvent.controllerNumber}" +
                        $" Value: {midiEvent.value}" +
                        $" Device Index: {midiEvent.deviceIndex}");

                    break;
            }
        });

        TinyMidi.Input.Start();
    }

    private void OnDestroy()
    {
        TinyMidi.Input.Stop();
    }
}
```

## Git Hooks

The git hooks that run are quick file checks to ensure the build files haven't changed.

```bash
$ git config --local core.hooksPath .githooks/
```

## Build

> [!WARNING]
> Do not commit any build changes to the repo. The build files are automatically generated via GitHub Actions.

### macOS

When developing on macOS, make sure that **Mac** is selected in the bottom right-hand corner of Visual Studio Code or C++ Intellisense will not work.

```bash
./bin/build.sh
```

### Windows

When developing on Windows, make sure that **Win32** is selected in the bottom right-hand corner of Visual Studio Code or C++ Intellisense will not work.

Run from **x64 Native Tools Command Prompt for VS**:

```cmd
call "./bin/build.bat"
```
