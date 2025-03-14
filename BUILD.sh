#!/bin/bash

# --- Clean build
./CLEAN.sh

mkdir build ; cd build

OSHCC=/root/lanl/openshmem/osss/osss-ucx_testing/build/build/bin/oshcc

cmake \
    -DCMAKE_C_COMPILER=$OSHCC \
    ..

# --- Compile
make -j $(( $(nproc) - 1 ))

echo ; echo
