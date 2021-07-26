#!/bin/bash
# Generate directories

[ ! -d build ] && mkdir build
[ ! -d tmp ] && mkdir tmp

# Go to directories

cd tmp

# Generate cmake files

cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..

# Build projects

make
