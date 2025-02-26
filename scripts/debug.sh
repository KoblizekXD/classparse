#!/bin/bash

if [ ! -f Makefile ]; then
    echo "Error: Makefile not found! Make sure you're executing from the root of the project!"
    exit 1
fi

make clean-all && make debug && gdb -ex "break FreeClassFile" --args ./examples/build/main.out ./examples/Main.class
