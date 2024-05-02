#!/bin/bash

# Create a build directory for release mode
mkdir -p build_release
cd build_release

# Configure CMake for release mode
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_FLAGS_RELEASE="-O2" -DCMAKE_CXX_FLAGS_RELEASE="-O2" ..

# Build the project
cmake --build .
