#!/bin/bash

mkdir -p build/

g++ -std=c++17 -o build/tinymidi src/*.cpp -Iinclude -framework CoreMIDI -framework CoreFoundation
