#!/bin/bash

# ---------------------------------------------------------------------
# OpenSHMEM Verification and Validation Test Suite

# Usage: ./shmemvv.sh [options]

# Options:
#   COMPLETE --test_setup            Run setup tests
#   COMPLETE --test_threads          Run thread support tests
#   COMPLETE --test_mem              Run memory management tests
#   COMPLETE --test_teams            Run team management tests
#   COMPLETE --test_ctx              Run communication/context management tests
#   COMPLETE --test_remote           Run remote memory access tests
#   COMPLETE --test_atomics          Run atomic memory operations tests
#   --test_signaling        Run signaling operations tests
#   COMPLETE --test_collectives      Run collective operations tests
#   --test_pt2pt_synch      Run point-to-point synchronization tests
#   --test_locking          Run distributed locking tests
#   --test_all              (default) Run all tests

#   --exclude_setup         Exclude setup tests
#   --exclude_threads       Exclude thread support tests
#   --exclude_mem           Exclude memory management tests
#   --exclude_teams         Exclude team management tests
#   --exclude_ctx           Exclude communication/context management tests
#   --exclude_remote        Exclude remote memory access tests
#   --exclude_atomics       Exclude atomic memory operations tests
#   --exclude_signaling     Exclude signaling operations tests
#   --exclude_collectives   Exclude collective operations tests
#   --exclude_pt2pt_synch   Exclude point-to-point synchronization tests
#   --exclude_locking       Exclude distributed locking tests

#   --np <N>                (default=varies by test) Override default PE count for all tests
#   --launcher <cmd>        (default=/mnt/DISCL/home/mibeebe/lanl/shmem/osss/osss-ucx_sanity/build/install/bin/oshrun) Path to oshrun launcher
#   --launcher_args <args>  Add custom arguments to launcher
#   --enable_c11            Enable C11 tests
#   --enable_c              Enable C/C++ tests
#   --verbose               Enable verbose output
#   --no-color              Disable colored output
#   --help                  Display this help message

# Note: You must enable at least one of --enable_c or --enable_c11
# ---------------------------------------------------------------------

hline="----------------------------------------------------"

# --- Get a list of unique hostnames from the srun command, sorted and concatenated with commas
hosts=$(srun hostname | sort | uniq | paste -sd, -)
# --- Count the number of unique hosts
num_hosts=$(echo "$hosts" | tr ',' '\n' | wc -l)

# --- Set the number of processes per host and the total number of processes
ppn=2
np=$(( $num_hosts * $ppn ))

# --- Create a hostfile with specified slots per host
create_hostfile() {
  local ppn=$1
  local hostfile="hostfile.txt"
  # --- Clear the hostfile if it exists
  > "$hostfile"
  # --- Write each host and its slots to the hostfile
  for host in ${hosts//,/ }; do
    echo "$host slots=$ppn" >> "$hostfile"
  done
  # --- Return the hostfile name
  echo "$hostfile"
}

# ---  Create hostfile with 2 slots per host
hostfile=$(create_hostfile $ppn)

# --- Set launcher
oshrun=$OSSS_TESTING_BIN/oshrun

# --- Print hostfile
echo $hline
echo "Running tests with $num_hosts hosts and $ppn processes per host"
echo "Hostfile:"
cat $hostfile
echo $hline

# --- Set some UCX vars
export UCX_LOG_LEVEL=error
export PMIX_MCA_gds=hash
export PMIX_DEBUG=1

# --- Set up log directory for all nodes
export SHMEMVV_LOG_DIR="$(pwd)/logs/"
mkdir -p $SHMEMVV_LOG_DIR

# --- Run tests
./shmemvv.sh \
  --enable_c \
  --launcher $oshrun \
  --launcher_args "--hostfile $hostfile -x SHMEMVV_LOG_DIR" \
  --np $np \
  --test_pt2pt_synch

# --- Clean up
rm $hostfile

