#!/bin/bash

SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)

REDON=$(tput setaf 1)
GREENON=$(tput setaf 2)
COLOROFF=$(tput sgr0)

(

  mkdir -p build/

  cd "${SCRIPT_DIR}" || exit

  cd ..

  printf "Building macOS libraries ... "

  g++ -std=c++17 -framework CoreMIDI -framework CoreFoundation -shared -fPIC -arch arm64 -o build/libTinyMidi-arm64.dylib include/TinyMidi/TinyMidi.cpp
  g++ -std=c++17 -framework CoreMIDI -framework CoreFoundation -shared -fPIC -arch x86_64 -o build/libTinyMidi-x86_64.dylib include/TinyMidi/TinyMidi.cpp

  if [[ ! -f build/libTinyMidi-arm64.dylib || ! -f build/libTinyMidi-x86_64.dylib ]]; then
    exit 1
  fi

  lipo -create -output build/libTinyMidi.dylib build/libTinyMidi-arm64.dylib build/libTinyMidi-x86_64.dylib

  lipo -info build/libTinyMidi.dylib

  mkdir -p UnityPackage/Libs/macOS

  cp build/libTinyMidi.dylib UnityPackage/Libs/macOS

  printf "%sDONE%s\n" "${GREENON}" "${COLOROFF}"

)
