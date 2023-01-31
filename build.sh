#!/bin/bash

cmake -S . -B tmp;
cmake --build tmp -- -j;
./bin/twe
