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
  -DDEBUG=ON \
  ../

make -j 40

export SHMEM_SYMMETRIC_HEAP_SIZE=1G
# export SHMEM_DEBUG=3
# export UCX_TLS=sm,self,tcp
# export UCX_NET_DEVICES=mlx5_0:1,mlx5_2:1
# export UCX_LOG_LEVEL=DEBUG
# export SHMEM_OFI_USE_PROVIDER=ofi+tcp

echo ; echo
cd ../

# echo "Verifying network devices:"
# ibv_devices
# ibv_devinfo

# Verify OFI provider
# echo "Verifying OFI provider:"
# fi_info -p ofi+tcp

exe=build/bin/shmemvv

if [ ! -f "$exe" ]; then
  echo "Executable $exe not found. Please build the project first."
  exit 1
fi

oshrun_flags="--bind-to core --map-by core"
oshrun $oshrun_flags -np 2 $exe --test_pt2pt_synch
