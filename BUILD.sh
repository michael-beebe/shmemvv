#!/bin/bash

# --- Clean build
./CLEAN.sh

mkdir build ; cd build

cmake \
    -DCMAKE_C_COMPILER=$(which oshcc) \
    ..

# --- Compile
make -j $(( $(nproc) - 1 ))

echo ; echo
