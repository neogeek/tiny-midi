using System;
using System.Runtime.InteropServices;

namespace TinyMidi
{

    public enum MidiEventStatus
    {

        NoteOn = 144,

        NoteOff = 128,

        ControlChange = 176

    };

    [StructLayout(LayoutKind.Sequential)]
    public struct MidiEvent
    {

        public int controllerNumber;

        public MidiEventStatus status;

        public int value;

        public int channel;

        public int deviceIndex;

    }

    public delegate void MidiEventListener(MidiEvent midiEvent);

    internal static class InputInternal
    {

#if WINDOWS_BUILD || UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
        [DllImport("libTinyMidi.dll", CallingConvention = CallingConvention.Cdecl)]
#elif MACOS_BUILD || UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX
        [DllImport("libTinyMidi.dylib", CallingConvention = CallingConvention.Cdecl)]
#endif
        public static extern void Setup();

#if WINDOWS_BUILD || UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
        [DllImport("libTinyMidi.dll", CallingConvention = CallingConvention.Cdecl)]
#elif MACOS_BUILD || UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX
        [DllImport("libTinyMidi.dylib", CallingConvention = CallingConvention.Cdecl)]
#endif
        public static extern void AddEventListener(IntPtr midiEventListener);

#if WINDOWS_BUILD || UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
        [DllImport("libTinyMidi.dll", CallingConvention = CallingConvention.Cdecl)]
#elif MACOS_BUILD || UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX
        [DllImport("libTinyMidi.dylib", CallingConvention = CallingConvention.Cdecl)]
#endif
        public static extern void Start();

#if WINDOWS_BUILD || UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
        [DllImport("libTinyMidi.dll", CallingConvention = CallingConvention.Cdecl)]
#elif MACOS_BUILD || UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX
        [DllImport("libTinyMidi.dylib", CallingConvention = CallingConvention.Cdecl)]
#endif
        public static extern void Stop();

    }

    public static class Input
    {

        public static void Setup()
        {
            InputInternal.Setup();
        }

        public static void AddEventListener(MidiEventListener midiEventListener)
        {
            var listenerPtr = Marshal.GetFunctionPointerForDelegate(midiEventListener);
            InputInternal.AddEventListener(listenerPtr);
        }

        public static void Start()
        {
            InputInternal.Start();
        }

        public static void Stop()
        {
            InputInternal.Stop();
        }

    }

}
