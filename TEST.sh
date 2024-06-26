#!/bin/bash
# ---- Check if an argument is provided
if [ $# -eq 0 ]; then
  echo "Usage: $0 <test_option>"
  echo "Available test options: setup, threads, mem, teams, comms, remote, atomics, signaling, collectives, pt2pt_synch, mem_ordering, locking, all, help"
  exit 1
fi

# ---- Get the test option from the command line argument
test_option=$1

# ---- Map the test option to the appropriate command line argument for your program
case $test_option in
  setup)
    arg="--test_setup"
    ;;
  threads)
    arg="--test_threads"
    ;;
  mem)
    arg="--test_mem"
    ;;
  teams)
    arg="--test_teams"
    ;;
  comms)
    arg="--test_comms"
    ;;
  remote)
    arg="--test_remote"
    ;;
  atomics)
    arg="--test_atomics"
    ;;
  signaling)
    arg="--test_signaling"
    ;;
  collectives)
    arg="--test_collectives"
    ;;
  pt2pt_synch)
    arg="--test_pt2pt_synch"
    ;;
  mem_ordering)
    arg="--test_mem_ordering"
    ;;
  locking)
    arg="--test_locking"
    ;;
  all)
    arg="--all"
    ;;
  help)
    arg="--help"
    ;;
  *)
    echo "Invalid test option: $test_option"
    echo "Available test options: setup, threads, mem, teams, comms, remote, atomics, signaling, collectives, pt2pt_synch, mem_ordering, locking, all, help"
    exit 1
    ;;
esac

# ---- Run the program with the appropriate argument
exe=build/bin/shmemvv

# Check if the executable exists
if [ ! -f "$exe" ]; then
  echo "Executable $exe not found. Please build the project first."
  exit 1
fi

# Run the executable with the appropriate argument
oshrun -np 2 $exe $arg
