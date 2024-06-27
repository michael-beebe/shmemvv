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

# export UCX_TLS=all
# export UCX_NET_DEVICES=all
export UCX_TLS=sm,self,tcp
export UCX_NET_DEVICES=ibs11f0,ibs11f1,ibs5
export UCX_LOG_LEVEL=DEBUG


echo ; echo
cd ../

# ---- Run the program with the appropriate argument
exe=build/bin/shmemvv

# Check if the executable exists
if [ ! -f "$exe" ]; then
  echo "Executable $exe not found. Please build the project first."
  exit 1
fi

# Run the executable with the appropriate argument
oshrun_flags="--bind-to core --map-by core"
oshrun $oshrun_flags -np 2 $exe --all

