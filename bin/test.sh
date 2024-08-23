#!/bin/bash

mkdir -p build/

g++ -std=c++17 -o build/tinymidi src/*.cpp -Iincludes -framework CoreMIDI -framework CoreFoundation
