#!/bin/bash

if command -v emcc &> /dev/null
then
    echo "emcc is installed."
else
    echo "emcc is not installed. Please install Emscripten."
fi
