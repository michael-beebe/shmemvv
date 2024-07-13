#!/bin/bash

# --- Clean build
rm -rf build
mkdir -p build
cd build

# --- Create install prefix
PREFIX=$SWHOME/shmemvv
mkdir -p $PREFIX

# --- Configure build
cmake \
  -DCMAKE_INCLUDE_PATH=$SOS_INCLUDE     \
  -DCMAKE_LIBRARY_PATH=$SOS_LIB         \
  -DCMAKE_C_LINKER_FLAGS="-lpmi_simple -lsma" \
  -DCMAKE_INSTALL_PREFIX=$SWHOME/shmemvv \
  -DDEBUG=ON \
  ../

# --- Compile
make -j 50

# --- Set OpenSHMEM/UCX/OFI env vars
export SHMEM_SYMMETRIC_HEAP_SIZE=1G
# export SHMEM_DEBUG=3
# export UCX_TLS=sm,self,tcp
# export UCX_NET_DEVICES=mlx5_0:1,mlx5_2:1
# export UCX_LOG_LEVEL=DEBUG
# export SHMEM_OFI_USE_PROVIDER=ofi+tcp

# --- Print info about network stuff
# echo "Verifying network devices:"
# ibv_devices
# ibv_devinfo

# Verify OFI provider
# echo "Verifying OFI provider:"
# fi_info -p ofi+tcp

# --- Move back to project root
echo ; echo
cd ../

# --- Set path to shmemvvv binary
exe=build/bin/shmemvv
if [ ! -f "$exe" ]; then
  echo "Executable $exe not found. Please build the project first."
  exit 1
fi

# --- Run shmemvv
######################################################################
# Usage: shmemvv [options]
# Options:
#   --test_setup         Run setup tests
#   --test_threads       Run thread support tests
#   --test_mem           Run memory management tests
#   --test_teams         Run team management tests
#   --test_ctx           Run communication management tests
#   --test_remote        Run remote memory access tests
#   --test_atomics       Run atomic memory operations tests
#   --test_signaling     Run signaling operations tests
#   --test_collectives   Run collective operations tests
#   --test_pt2pt_synch   Run point-to-point synchronization tests
#   --test_mem_ordering  Run memory ordering tests
#   --test_locking       Run distributed locking tests
#   --all                (default) Run all tests
#   --help               Display help message
######################################################################

oshrun_flags="--bind-to core --map-by core"
oshrun $oshrun_flags -np 2 $exe --test_locking
