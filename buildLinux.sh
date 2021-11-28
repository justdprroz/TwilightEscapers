#!/bin/bash
# Generate directories

[ ! -d build ] && mkdir build
[ ! -d tmp ] && mkdir tmp

# Go to directories

cd tmp

# Generate cmake files

cmake ..

# Build projects

make
