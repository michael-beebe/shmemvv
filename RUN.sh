#!/bin/bash

# --- Set bin directory
BIN_DIR=$(pwd)/build/bin

# ----------- Atomics -----------
# --- C
oshrun -np 2 $BIN_DIR/c_shmem_atomic_fetch