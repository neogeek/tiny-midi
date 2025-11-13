#include <iostream>
#include <thread>

#include "TinyMidi/Input.hpp"

auto main() -> int
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
                          << " Device Index: " << midiEvent.deviceIndex << "\n";
                break;
            case TinyMidi::MidiEventStatus::NoteOff:
                std::cout << "Note Up: " << midiEvent.controllerNumber
                          << " Velocity: " << midiEvent.value
                          << " Device Index: " << midiEvent.deviceIndex << "\n";
                break;
            case TinyMidi::MidiEventStatus::ControlChange:
                std::cout << "Control Changed: " << midiEvent.controllerNumber
                          << " Value: " << midiEvent.value
                          << " Device Index: " << midiEvent.deviceIndex << "\n";
                break;
            }
        });

    TinyMidi::StartThread();

    while (true)
    {
        std::this_thread::yield();
    }

    TinyMidi::StopThread();

    return 0;
}
