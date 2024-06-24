#!/bin/bash

./CLEAN.sh

mkdir -p build
cd build

PREFIX=$SWHOME/shmemvv
mkdir -p $PREFIX

cmake \
  -DCMAKE_INCLUDE_PATH=$SOS_INCLUDE     \
  -DCMAKE_LIBRARY_PATH=$SOS_LIB         \
  -DCMAKE_C_LINKER_FLAGS="-lpmi_simple -lsma" \
  -DCMAKE_INSTALL_PREFIX=$SWHOME/shmemvv \
  ../

make
# ctest

echo ; echo
cd ../
./TEST.sh setup

