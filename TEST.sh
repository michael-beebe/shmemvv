#!/bin/bash

# --- Clean build
./CLEAN.sh

# --- Compile testing library
./BUILD.sh

# -- Set launcher
OSHRUN=/root/lanl/openshmem/osss/osss-ucx_testing/build/build/bin/oshrun

# --- Run all tests
export PMIX_MCA_pcompress_base_silence_warning=1
./shmemvv.sh          \
  --enable_c          \
  --enable_c11        \
  --test_all          \
  --launcher $OSHRUN  \
  --launcher_args "--allow-run-as-root" \
  --np 4 \
  --exclude_signaling --exclude_remote
