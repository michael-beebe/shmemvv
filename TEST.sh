#!/bin/bash

# --- Clean build
./CLEAN.sh

# --- Compile testing library
./BUILD.sh

# -- Set launcher
OSHRUN=/root/lanl/openshmem/osss/osss-ucx_testing/build/build/bin/oshrun

# ---------------------------------------------------------------------
# Remaining list of tests to make sure we pass, go through each
# option one by one. Delete the line once they pass.
#
# NOTE: RMA relies on signaling so do those together.
# ---------------------------------------------------------------------
# OpenSHMEM Verification and Validation Test Suite

# Usage: ./shmemvv.sh [options]

# Options:
#   --test_teams            Run team management tests
#   --test_remote           Run remote memory access tests
#   --test_signaling        Run signaling operations tests
#   --test_collectives      Run collective operations tests
#   --test_pt2pt_synch      Run point-to-point synchronization tests
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
#   --launcher <cmd>        (default=/root/sw/linuxkit-aarch64/sos_1.5.2/bin/oshrun) Path to oshrun launcher
#   --launcher_args <args>  Add custom arguments to launcher
#   --enable_c11            Enable C11 tests
#   --enable_c              Enable C/C++ tests
#   --verbose               Enable verbose output
#   --no-color              Disable colored output
#   --help                  Display this help message

# Note: You must enable at least one of --enable_c or --enable_c11
# ---------------------------------------------------------------------

# --- Suppress PMIX warnings for now
export PMIX_MCA_pcompress_base_silence_warning=1

# --- Set UCX transport
export UCX_TLS=self,sm,tcp,posix

# --- For containerized environments, sometimes memory registration needs to be disabled
export UCX_MEMTYPE_CACHE=n
export UCX_RCACHE_ENABLE=n
  
# --- Make TCP more robust in container settings
export UCX_TCP_CM_REUSEADDR=y
export UCX_TCP_TX_SEG_SIZE=16k
export UCX_TCP_RX_SEG_SIZE=16k
  
# --- Force TCP to be used for all traffic
#export UCX_NET_DEVICES=eth0
#export UCX_UNIFIED_MODE=y
  
# --- Reduce log level to see exactly what's failing
#export UCX_LOG_LEVEL=DEBUG

# --- Disable huge pages for now
export UCX_MM_HUGETLB=n
export UCX_SYSV_HUGETLB=n
export UCX_POSIX_HUGETLB=n

# --- Run tests
./shmemvv.sh \
  --enable_c                            \
  --launcher $OSHRUN                    \
  --launcher_args "--allow-run-as-root" \
  --np 2                                \
  --test_all # --test_signaling

