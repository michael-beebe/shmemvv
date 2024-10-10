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

# --- Move back to project root
echo ; echo
cd ../
echo ; echo

# --- Run tests
# ./RUN.sh

