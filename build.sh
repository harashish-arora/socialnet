#!/bin/bash

# Build script for SocialNet Simulator
# Compiles main.cpp and produces executable 'socialnet'

g++ -std=c++11 -Wall -Wextra -O2 main.cpp -o socialnet

if [ $? -eq 0 ]; then
    echo "Compilation successful. Executable 'socialnet' created."
else
    echo "Compilation failed."
    exit 1
fi
