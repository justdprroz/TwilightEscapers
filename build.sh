#!/bin/bash
[ ! -d build ] && mkdir build
[ ! -d tmp ] && mkdir tmp
cd tmp
if [ $# -eq 0 ]; then
    cmake -DCMAKE_BUILD_TYPE=Release ..
else
    if [ "$1" == "Debug" ] || [ "$1" == "D" ] || [ "$1" == "debug" ] || [ "$1" == "d" ]; then
        cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
    fi
fi
make
