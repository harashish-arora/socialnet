#!/bin/bash


MSG="COL106 dassi dedo pls"

ANIMALS=(tux dragon stegosaurus vader turkey skeleton moose eyes ghostbusters)

if command -v cowsay >/dev/null 2>&1; then
    idx=$((RANDOM % ${#ANIMALS[@]}))
    cowfile=${ANIMALS[$idx]}
    cowsay -f "$cowfile" "$MSG"
elif command -v cowthink >/dev/null 2>&1; then
    idx=$((RANDOM % ${#ANIMALS[@]}))
    cowfile=${ANIMALS[$idx]}
    cowthink -f "$cowfile" "$MSG"
else
    echo "+-----------------------------------------------+"
    printf "| %-45s |\n" "$MSG"
    echo "+-----------------------------------------------+"
fi

g++ -std=c++11 -Wall -Wextra -O2 main.cpp -o socialnet

if [ $? -eq 0 ]; then
    echo "Compilation successful. Executable 'socialnet' created."
else
    echo "Compilation failed."
    exit 1
fi
