#!/bin/bash

# --- Set bin directory
BIN_DIR=$(pwd)/build/bin
ATOMIC_BIN=$BIN_DIR/atomics
COLLECTIVE_BIN=$BIN_DIR/collectives
RMA_BIN=$BIN_DIR/rma
CTX_BIN=$BIN_DIR/ctx
LOCKING_BIN=$BIN_DIR/locking
MEMORY_BIN=$BIN_DIR/memory
PT2PT_SYNC_BIN=$BIN_DIR/pt2pt_sync
SETUP_BIN=$BIN_DIR/setup

# ----------  C    ------------
# --- setup
for executable in $SETUP_BIN/c_*; do
  oshrun -np 2 $executable
done

# # --- atomics
# for executable in $ATOMIC_BIN/c_*; do
#   oshrun -np 2 $executable
# done

# # --- collectives
# for executable in $COLLECTIVE_BIN/c_*; do
#   oshrun -np 2 $executable
# done

# # --- rma
# for executable in $RMA_BIN/c_*; do
#   oshrun -np 2 $executable
# done

# # --- ctx
# for executable in $CTX_BIN/c_*; do
#   oshrun -np 2 $executable
# done  

# # --- locking
# for executable in $LOCKING_BIN/c_*; do
#   oshrun -np 2 $executable
# done

# # --- memory
# for executable in $MEMORY_BIN/c_*; do
#   oshrun -np 2 $executable
# done

# # --- pt2pt_sync
# for executable in $PT2PT_SYNC_BIN/c_*; do
#   oshrun -np 2 $executable
# done

# ----------  C11  ------------
# # --- atomics
# for executable in $ATOMIC_BIN/c11_*; do
#   oshrun -np 2 $executable
# done

# # --- collectives
# for executable in $COLLECTIVE_BIN/c11_*; do
#   oshrun -np 2 $executable
# done

# # --- rma
# for executable in $RMA_BIN/c11_*; do
#   oshrun -np 2 $executable
# done

# ----------  CXX  ------------
