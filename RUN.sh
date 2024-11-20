#!/bin/bash

# --- Set log dir
rm -rf logs
export SHMEMVV_LOG_DIR="$(pwd)/logs/"
mkdir -p $SHMEMVV_LOG_DIR

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
SIGNALING_BIN=$BIN_DIR/signaling
TEAMS_BIN=$BIN_DIR/teams
THREADS_BIN=$BIN_DIR/threads

HLINE="================================================"

# ===============================================
#         ----------  C  ------------
# ===============================================
echo $HLINE
echo "    RUNNING C TESTS"
echo $HLINE

# --- setup
# for executable in $SETUP_BIN/c_*; do
#   oshrun -np 2 $executable
# done

# --- atomics
# for executable in $ATOMIC_BIN/c_*; do
#   oshrun -np 2 $executable
# done

# --- collectives
# for executable in $COLLECTIVE_BIN/c_*; do
#   oshrun -np 2 $executable
# done

# --- rma
# for executable in $RMA_BIN/c_*; do
#   oshrun -np 2 $executable
# done

# --- ctx
# for executable in $CTX_BIN/c_*; do
#   oshrun -np 2 $executable
# done  

# --- locking
# for executable in $LOCKING_BIN/c_*; do
#   oshrun -np 2 $executable
# done

# --- memory
# for executable in $MEMORY_BIN/c_*; do
#   oshrun -np 2 $executable
# done

# --- pt2pt_sync
# for executable in $PT2PT_SYNC_BIN/c_*; do
#   oshrun -np 2 $executable
# done

# --- signaling
# for executable in $SIGNALING_BIN/c_*; do
#   oshrun -np 2 $executable
# done

# --- teams
# for executable in $TEAMS_BIN/c_*; do
#   oshrun -np 2 $executable
# done

# --- threads
for executable in $THREADS_BIN/c_*; do
  oshrun -np 2 $executable
done

# ===============================================
#       ----------  C11  ------------
# ===============================================
echo $HLINE
echo "    RUNNING C11 TESTS"
echo $HLINE

# --- setup
# for executable in $SETUP_BIN/c11_*; do
#   oshrun -np 2 $executable
# done

# --- atomics
# for executable in $ATOMIC_BIN/c11_*; do
#   oshrun -np 2 $executable
# done

# --- collectives
# for executable in $COLLECTIVE_BIN/c11_*; do
#   oshrun -np 2 $executable
# done

# --- rma
# for executable in $RMA_BIN/c11_*; do
#   oshrun -np 2 $executable
# done

# --- ctx
# for executable in $CTX_BIN/c11_*; do
#   oshrun -np 2 $executable
# done  

# --- locking
# for executable in $LOCKING_BIN/c11_*; do
#   oshrun -np 2 $executable
# done

# --- memory
# for executable in $MEMORY_BIN/c11_*; do
#   oshrun -np 2 $executable
# done

# --- pt2pt_sync
# for executable in $PT2PT_SYNC_BIN/c11_*; do
#   oshrun -np 2 $executable
# done

# --- signaling
# for executable in $SIGNALING_BIN/c11_*; do
#   oshrun -np 2 $executable
# done

# --- teams
# for executable in $TEAMS_BIN/c11_*; do
#   oshrun -np 2 $executable
# done

# --- threads
for executable in $THREADS_BIN/c11_*; do
  oshrun -np 2 $executable
done

# ===============================================
#       ----------  CXX  ------------
# ===============================================
# echo $HLINE
# echo "    RUNNING CXX TESTS"
# echo $HLINE

# --- setup
# for executable in $SETUP_BIN/cxx_*; do
#   oshrun -np 2 $executable
# done

# --- atomics
# for executable in $ATOMIC_BIN/cxx_*; do
#   oshrun -np 2 $executable
# done

# --- collectives
# for executable in $COLLECTIVE_BIN/cxx_*; do
#   oshrun -np 2 $executable
# done

# --- rma
# for executable in $RMA_BIN/cxx_*; do
#   oshrun -np 2 $executable
# done

# --- ctx
# for executable in $CTX_BIN/cxx_*; do
#   oshrun -np 2 $executable
# done  

# --- locking
# for executable in $LOCKING_BIN/cxx_*; do
#   oshrun -np 2 $executable
# done

# --- memory
# for executable in $MEMORY_BIN/cxx_*; do
#   oshrun -np 2 $executable
# done

# --- pt2pt_sync
# for executable in $PT2PT_SYNC_BIN/cxx_*; do
#   oshrun -np 2 $executable
# done

# --- signaling
# for executable in $SIGNALING_BIN/cxx_*; do
#   oshrun -np 2 $executable
# done

# --- teams
# for executable in $TEAMS_BIN/cxx_*; do
#   oshrun -np 2 $executable
# done

# --- threads
# for executable in $THREADS_BIN/cxx_*; do
#   oshrun -np 2 $executable
# done


