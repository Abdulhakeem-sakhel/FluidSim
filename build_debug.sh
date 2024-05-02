#!/bin/bash

# Create a build directory for debug mode
mkdir -p build_debug
cd build_debug

# Configure CMake for debug mode
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Build the project
cmake --build .