#pragma once

#include <thread>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef _WIN32
#include <mmsystem.h>
#define PACKAGE_API __declspec(dllexport)
#elif __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#include <CoreMIDI/CoreMIDI.h>
#define PACKAGE_API
#endif

namespace TinyMidi
{

struct MIDIContext
{
    int deviceIndex;
};

enum MidiEventStatus : uint8_t
{
    NoteOn = 144,
    NoteOff = 128,
    ControlChange = 176
};

struct MidiEvent
{
    int controllerNumber;
    MidiEventStatus status;
    int value;
    int channel;
    int deviceIndex;
};

using MidiEventListener = void (*)(MidiEvent);

#ifdef _WIN32
std::vector<HMIDIIN> hMidiInList;
#elif __APPLE__
MIDIClientRef client;
MIDIPortRef inputPort;
CFRunLoopRef runLoop;
#endif

std::vector<MidiEventListener> midiEventListeners;

std::thread running;

void TriggerEventListeners(MidiEvent midiEvent)
{
    for (auto midiEventListener : midiEventListeners)
    {
        midiEventListener(midiEvent);
    }
}

#ifdef _WIN32
void MIDIReadProc(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance,
                  DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
    if (wMsg == MIM_DATA)
    {
        auto context = reinterpret_cast<MIDIContext *>(dwInstance);

        DWORD midiMessage = dwParam1;

        auto status = static_cast<MidiEventStatus>(midiMessage & 0xF0);
        auto channel = static_cast<int>(midiMessage & 0x0F);
        auto controllerNumber = static_cast<int>((midiMessage >> 8) & 0xFF);
        auto value = static_cast<int>((midiMessage >> 16) & 0xFF);

        TriggerEventListeners(
            {controllerNumber, status, value, channel, context->deviceIndex});
    }
}
#elif __APPLE__
void MIDIReadProc(const MIDIPacketList *packetList, void *readProcRefCon,
                  void *srcConnRefCon)
{
    auto *context = static_cast<MIDIContext *>(srcConnRefCon);

    const auto *packet = &packetList->packet[0];

    for (unsigned int i = 0; i < packetList->numPackets; i++)
    {
        auto status = static_cast<MidiEventStatus>(packet->data[0] & 0xF0);
        auto channel = static_cast<int>(packet->data[0] & 0x0F);
        auto controllerNumber = static_cast<int>(packet->data[1]);
        auto value = static_cast<int>(packet->data[2]);

        TriggerEventListeners(
            {controllerNumber, status, value, channel, context->deviceIndex});
    }
}
#endif

extern "C"
{
    PACKAGE_API void Setup()
    {
#ifdef __APPLE__
        MIDIClientCreate(CFSTR("MIDI Client"), nullptr, nullptr, &client);
        MIDIInputPortCreate(client, CFSTR("Input Port"), MIDIReadProc, nullptr,
                            &inputPort);

        auto sourceCount = MIDIGetNumberOfSources();

        for (auto index = 0; index < sourceCount; index += 1)
        {
            auto source = MIDIGetSource(index);

            auto *context = new MIDIContext();
            context->deviceIndex = index;

            MIDIPortConnectSource(inputPort, source, context);
        }
#endif
    }

    PACKAGE_API void AddEventListener(MidiEventListener midiEventListener);

    PACKAGE_API void AddEventListener(MidiEventListener midiEventListener)
    {
        midiEventListeners.push_back(midiEventListener);
    }

    PACKAGE_API void Start()
    {
#ifdef _WIN32
        auto deviceCount = midiInGetNumDevs();

        for (auto index = 0; index < deviceCount; index += 1)
        {
            HMIDIIN hMidiIn;

            auto context = new MIDIContext();
            context->deviceIndex = index;

            auto result = midiInOpen(&hMidiIn, 0, (DWORD_PTR)MIDIReadProc,
                                     (DWORD_PTR)context, CALLBACK_FUNCTION);
            if (result == MMSYSERR_NOERROR)
            {
                midiInStart(hMidiIn);
            }

            hMidiInList.push_back(hMidiIn);
        }

#elif __APPLE__
        runLoop = CFRunLoopGetCurrent();

        CFRunLoopRun();
#endif
    }

    PACKAGE_API void Stop()
    {
#ifdef _WIN32
        for (auto &hMidiIn : hMidiInList)
        {
            midiInStop(hMidiIn);
            midiInClose(hMidiIn);
        }
#elif __APPLE__
        CFRunLoopStop(runLoop);
#endif
    }
}

auto StartThread() { running = std::thread(Start); }

auto StopThread()
{
    if (running.joinable())
    {
        running.join();
    }
}

} // namespace TinyMidi
