#!/bin/bash

# --- Clean build
./scripts/clean.sh

mkdir build ; cd build

export CC=$OSSS_TESTING_BIN/oshcc
export CXX=$OSSS_TESTING_BIN/oshc++

cmake ..

# --- Compile
make -j $(( $(nproc) - 1 ))

echo ; echo