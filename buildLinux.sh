#!/bin/bash
# Generate directories

[ ! -d build ] && mkdir build
[ ! -d tmp ] && mkdir tmp

# Move assets to build directory
cp -r assets build/

# Go to directories
cd tmp

# Generate cmake files
cmake -DCMAKE_BUILD_TYPE=Release ..

# Build projects
make
