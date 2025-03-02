#!/bin/bash

# --- Clean build
./CLEAN.sh

mkdir build ; cd build

# --- Configure build based on option
if [ "$1" = "osss" ]; then
    # --- Configure build with OSSS
    OSSS_BIN=/root/lanl/openshmem/osss/osss-ucx_testing/build/build/bin
    OSHCC="$OSSS_BIN/oshcc"
    OSHCXX="$OSSS_BIN/oshcxx"
elif [ "$1" = "sos" ]; then
    # --- Configure build with SOS
    SOS_BIN=/root/sw/linuxkit-aarch64/sos_1.5.2/bin/
    OSHCC="$SOS_BIN/oshcc"
    OSHCXX="$SOS_BIN/oshc++"
else
    echo "Please specify either 'osss' or 'sos' as an argument"
    exit 1
fi

export CC=$OSHCC ; export CXX=$OSHCXX

echo "----------------------------------------"
echo "CC: $CC"
echo "CXX: $CXX"
echo "----------------------------------------"

cmake ../

# --- Compile
make -j $(( $(nproc) - 1 ))

# --- Move back to project root
echo ; echo
cd ../
echo ; echo

# --- Run tests
./RUN.sh $1
ls -l logs
