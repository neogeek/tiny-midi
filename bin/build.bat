@echo off

if not defined VCINSTALLDIR (
    echo Please run this script from x64 Native Tools Command Prompt for VS.
    exit /b 1
)

mkdir build

cl /EHsc /std:c++17 /c include/TinyMidi/TinyMidi.cpp /Fo:build\TinyMidi.obj

link /DLL /MACHINE:X64 /OUT:build\libTinyMidi.dll /IMPLIB:build\libTinyMidi.lib build\TinyMidi.obj winmm.lib

del build\TinyMidi.obj

mkdir UnityPackage\Libs\Windows

copy build\libTinyMidi.dll UnityPackage\Libs\Windows
