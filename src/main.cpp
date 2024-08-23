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
