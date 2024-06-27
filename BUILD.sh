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

make -j 40
# ctest

export SHMEM_SYMMETRIC_HEAP_SIZE=128M
export SHMEM_OFI_USE_PROVIDER=sockets
export SHMEM_DEBUG=verbose

echo ; echo
cd ../
./TEST.sh all

