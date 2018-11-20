#!/bin/bash

mkdir build_native
cd build_native
cmake ..
make
mv SliderCombat ../
cd ..
