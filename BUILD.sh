#!/bin/bash

# --- Clean build
rm -rf build
mkdir -p build
cd build

# --- Create install prefix
PREFIX=$SWHOME/shmemvv
mkdir -p $PREFIX

# --- Configure build with SOS
export CC=oshcc ; export CXX=oshc++
cmake ../

# --- Compile
make -j $(( $(nproc) - 1 ))

# # --- Move back to project root
# echo ; echo
# cd ../

# # --- Set path to shmemvv binary
# exe=build/bin/shmemvv
# if [ ! -f "$exe" ]; then
#   echo "Executable $exe not found. Please build the project first."
#   exit 1
# fi

# --- Run shmemvv with CPU pinning
# flags="--bind-to core --map-by core"

# if [ "$(which oshcc)" == "$HOME/sw/el9-x86_64/ompi/bin/oshcc" ]; then
#   flags+=" --mca btl ^openib"
# fi

# oshrun $flags -np 6 $exe --test_all
