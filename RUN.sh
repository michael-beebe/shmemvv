#!/bin/bash

# --- Set bin directory
BIN_DIR=$(pwd)/build/bin
ATOMIC_BIN=$BIN_DIR/atomics
COLLECTIVE_BIN=$BIN_DIR/collectives
RMA_BIN=$BIN_DIR/rma
CTX_BIN=$BIN_DIR/ctx

# ----------- Atomics ------------
# # --- C
# for executable in $ATOMIC_BIN/c_*; do
#   oshrun -np 2 $executable
# done

# # --- C11
# for executable in $ATOMIC_BIN/c11_*; do
#   oshrun -np 2 $executable
# done

# # --- CXX
# for executable in $ATOMIC_BIN/cxx_*; do
#   oshrun -np 2 $executable
# done

# --------- Collectives ----------
# # --- C
# for executable in $COLLECTIVE_BIN/c_*; do
#   oshrun -np 2 $executable
# done

# # --- C11
# for executable in $COLLECTIVE_BIN/c11_*; do
#   oshrun -np 2 $executable
# done

# ----------- Pt2pt RMA ----------
# # --- C11

# # --- C
# for executable in $RMA_BIN/c_*; do
#   oshrun -np 2 $executable
# done

# # --- C11
# for executable in $RMA_BIN/c11_*; do
#   oshrun -np 2 $executable
# done

# ----------- Contexts ------------
# --- C
for executable in $CTX_BIN/c_*; do
  oshrun -np 2 $executable
done
