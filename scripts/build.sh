#!/bin/bash

# --- Clean build
./scripts/clean.sh

mkdir build ; cd build

OSHCC=$OSSS_TESTING_BIN/oshcc

cmake \
    -DCMAKE_C_COMPILER=$OSHCC \
    ..

# --- Compile
make -j $(( $(nproc) - 1 ))

echo ; echo
