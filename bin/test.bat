@echo off

if not defined VCINSTALLDIR (
    echo Please run this script from x64 Native Tools Command Prompt for VS.
    exit /b 1
)

mkdir build

cl /EHsc /std:c++17 /Iincludes /Fe:build\tinymidi.exe src\*.cpp winmm.lib

del main.obj
