#!/bin/bash

conan profile detect
conan install . --build=missing -of=build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
