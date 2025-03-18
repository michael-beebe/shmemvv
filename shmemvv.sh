#!/bin/bash

#
# OpenSHMEM Verification and Validation Test Suite Runner
#

# --- Set default values
DEFAULT_NP=4
VERBOSE=0
COLOR=1
RUN_ALL=1
LAUNCHER="oshrun"
LAUNCHER_ARGS=""
HLINE="================================================"
ENABLE_C11=0 # By default, don't run C11 tests
ENABLE_C=0   # By default, don't run C/C++ tests

# --- PE counts for different test categories
SETUP_NP=2
THREADS_NP=2
MEMORY_NP=2
TEAMS_NP=2
CTX_NP=2
REMOTE_NP=2
ATOMICS_NP=2
SIGNALING_NP=2
COLLECTIVES_NP=2 
PT2PT_SYNC_NP=2
LOCKING_NP=2

# --- Test selection flags
RUN_SETUP=0
RUN_THREADS=0
RUN_MEM=0
RUN_TEAMS=0
RUN_CTX=0
RUN_REMOTE=0
RUN_ATOMICS=0
RUN_SIGNALING=0
RUN_COLLECTIVES=0
RUN_PT2PT_SYNCH=0
RUN_LOCKING=0

# --- Test exclusion flags
EXCLUDE_SETUP=0
EXCLUDE_THREADS=0
EXCLUDE_MEM=0
EXCLUDE_TEAMS=0
EXCLUDE_CTX=0
EXCLUDE_REMOTE=0
EXCLUDE_ATOMICS=0
EXCLUDE_SIGNALING=0
EXCLUDE_COLLECTIVES=0
EXCLUDE_PT2PT_SYNCH=0
EXCLUDE_LOCKING=0

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

# --- Terminal colors
if [ "$COLOR" -eq 1 ]; then
  RED='\033[0;31m'
  GREEN='\033[0;32m'
  YELLOW='\033[0;33m'
  BLUE='\033[0;34m'
  BOLD='\033[1m'
  NC='\033[0m' # No Color
else
  RED=''
  GREEN=''
  YELLOW=''
  BLUE=''
  BOLD=''
  NC=''
fi

# --- Statistics counters
TESTS_TOTAL=0
TESTS_PASSED=0
TESTS_FAILED=0

# --- Help message
display_usage() {
  echo -e "\n${BOLD}OpenSHMEM Verification and Validation Test Suite${NC}\n"
  echo -e "Usage: ./shmemvv.sh [options]\n"
  echo "Options:"
  echo "  --test_setup            Run setup tests"
  echo "  --test_threads          Run thread support tests"
  echo "  --test_mem              Run memory management tests"
  echo "  --test_teams            Run team management tests"
  echo "  --test_ctx              Run communication/context management tests"
  echo "  --test_remote           Run remote memory access tests"
  echo "  --test_atomics          Run atomic memory operations tests"
  echo "  --test_signaling        Run signaling operations tests"
  echo "  --test_collectives      Run collective operations tests"
  echo "  --test_pt2pt_synch      Run point-to-point synchronization tests"
  echo "  --test_locking          Run distributed locking tests"
  echo "  --test_all              (default) Run all tests"
  echo ""
  echo "  --exclude_setup         Exclude setup tests"
  echo "  --exclude_threads       Exclude thread support tests"
  echo "  --exclude_mem           Exclude memory management tests"
  echo "  --exclude_teams         Exclude team management tests"
  echo "  --exclude_ctx           Exclude communication/context management tests"
  echo "  --exclude_remote        Exclude remote memory access tests"
  echo "  --exclude_atomics       Exclude atomic memory operations tests"
  echo "  --exclude_signaling     Exclude signaling operations tests"
  echo "  --exclude_collectives   Exclude collective operations tests"
  echo "  --exclude_pt2pt_synch   Exclude point-to-point synchronization tests"
  echo "  --exclude_locking       Exclude distributed locking tests"
  echo ""
  echo "  --np <N>                (default=varies by test) Override default PE count for all tests"
  echo "  --launcher <cmd>        (default=$(which oshrun)) Path to oshrun launcher"
  echo "  --launcher_args <args>  Add custom arguments to launcher"
  echo "  --enable_c11            Enable C11 tests"
  echo "  --enable_c              Enable C/C++ tests"
  echo "  --verbose               Enable verbose output"
  echo "  --no-color              Disable colored output"
  echo "  --help                  Display this help message"
  echo ""
  echo "Note: You must enable at least one of --enable_c or --enable_c11"
  echo ""
}

# --- Setup environment check
setup_environment() {
  # --- Set up log directory
  rm -rf logs
  export SHMEMVV_LOG_DIR="$(pwd)/logs/"
  mkdir -p $SHMEMVV_LOG_DIR

  # --- Check for launcher
  if ! command -v $LAUNCHER &>/dev/null; then
    echo -e "${RED}Error: $LAUNCHER not found in PATH.${NC}"
    echo "Please ensure OpenSHMEM is properly installed and the launcher is in your PATH."
    exit 1
  fi

  # --- Check if binary directories exist
  if [ ! -d "$BIN_DIR" ]; then
    echo -e "${RED}Error: Binary directory $BIN_DIR not found.${NC}"
    echo "Please build the test suite before running this script."
    exit 1
  fi

  # --- Check for test executables in at least some of the test directories
  local test_count=0
  local required_dirs=("$SETUP_BIN" "$MEMORY_BIN" "$RMA_BIN")

  for dir in "${required_dirs[@]}"; do
    if [ -d "$dir" ]; then
      # Check for C test executables (they should always exist)
      local c_execs=$(find "$dir" -name "c_*" -type f -executable 2>/dev/null | wc -l)
      if [ "$c_execs" -gt 0 ]; then
        test_count=$((test_count + c_execs))
      fi
    fi
  done

  if [ "$test_count" -eq 0 ]; then
    echo -e "${RED}Error: No test executables found in the test directories.${NC}"
    echo -e "The test suite doesn't appear to be built correctly."
    echo -e "Please build the test suite with 'make' before running this script."
    exit 1
  fi

  # echo -e "${GREEN}Found $test_count test executables. Test suite appears to be built correctly.${NC}"
}

# --- Run a single test
run_test() {
  local test_path=$1
  local test_name=$(basename $test_path)
  local np=$2

  TESTS_TOTAL=$((TESTS_TOTAL + 1))

  if [ $VERBOSE -eq 1 ]; then
    echo -e "${BLUE}Running test:${NC} $test_name with $np PEs"
    echo "Command: $LAUNCHER $LAUNCHER_ARGS -np $np $test_path"
  fi

  # --- Run the test and capture output
  local output=$($LAUNCHER $LAUNCHER_ARGS -np $np $test_path 2>&1)
  local status=$?

  # --- Display the test output
  echo "$output"
}

# --- Run all tests in a directory
run_test_category() {
  local test_dir=$1
  local category_name=$2
  local np=$3
  local pattern="${4:-c_*}"

  if [ ! -d "$test_dir" ]; then
    echo -e "${YELLOW}Warning: Test directory $test_dir not found, skipping $category_name tests.${NC}"
    return
  fi

  echo "$HLINE"
  echo -e "${BOLD}Running $category_name tests (${np} PEs)...${NC}"
  echo "$HLINE"

  # --- Check if any executables matching the pattern exist
  local found_tests=0
  for executable in $test_dir/$pattern; do
    if [ -x "$executable" ]; then
      found_tests=1
      run_test "$executable" "$np"
    fi
  done

  # --- Display message if no tests were found
  if [ $found_tests -eq 0 ]; then
    echo -e "${YELLOW}No compiled tests found for $category_name. Make sure to build these tests first.${NC}"
  fi

  echo ""
}

# --- Run C and C11 tests for a category
run_all_langs() {
  local dir=$1
  local name=$2
  local np=$3

  # --- Run C tests if enabled
  if [ $ENABLE_C -eq 1 ]; then
    run_test_category "$dir" "$name (C/C++)" "$np" "c_*"
  fi

  # --- Run C11 tests if enabled
  if [ $ENABLE_C11 -eq 1 ]; then
    run_test_category "$dir" "$name (C11)" "$np" "c11_*"
  fi
}

# --- Test category functions
run_setup_tests() {
  if [ $EXCLUDE_SETUP -eq 1 ]; then
    return
  fi
  local np=${SETUP_NP}
  run_all_langs "$SETUP_BIN" "Setup" "$np"
}

run_threads_tests() {
  if [ $EXCLUDE_THREADS -eq 1 ]; then
    return
  fi
  local np=${THREADS_NP}
  run_all_langs "$THREADS_BIN" "Threads Support" "$np"
}

run_memory_tests() {
  if [ $EXCLUDE_MEM -eq 1 ]; then
    return
  fi
  local np=${MEMORY_NP}
  run_all_langs "$MEMORY_BIN" "Memory Management" "$np"
}

run_teams_tests() {
  if [ $EXCLUDE_TEAMS -eq 1 ]; then
    return
  fi
  local np=${TEAMS_NP}
  run_all_langs "$TEAMS_BIN" "Teams Management" "$np"
}

run_ctx_tests() {
  if [ $EXCLUDE_CTX -eq 1 ]; then
    return
  fi
  local np=${CTX_NP}
  run_all_langs "$CTX_BIN" "Context Management" "$np"
}

run_remote_tests() {
  if [ $EXCLUDE_REMOTE -eq 1 ]; then
    return
  fi
  local np=${REMOTE_NP}
  run_all_langs "$RMA_BIN" "Remote Memory Access" "$np"
}

run_atomics_tests() {
  if [ $EXCLUDE_ATOMICS -eq 1 ]; then
    return
  fi
  local np=${ATOMICS_NP}
  run_all_langs "$ATOMIC_BIN" "Atomic Memory Operations" "$np"
}

run_signaling_tests() {
  if [ $EXCLUDE_SIGNALING -eq 1 ]; then
    return
  fi
  local np=${SIGNALING_NP}
  run_all_langs "$SIGNALING_BIN" "Signaling Operations" "$np"
}

run_collectives_tests() {
  if [ $EXCLUDE_COLLECTIVES -eq 1 ]; then
    return
  fi
  local np=${COLLECTIVES_NP}
  run_all_langs "$COLLECTIVE_BIN" "Collective Operations" "$np"
}

run_pt2pt_synch_tests() {
  if [ $EXCLUDE_PT2PT_SYNCH -eq 1 ]; then
    return
  fi
  local np=${PT2PT_SYNC_NP}
  run_all_langs "$PT2PT_SYNC_BIN" "Point-to-Point Synchronization" "$np"
}

run_locking_tests() {
  if [ $EXCLUDE_LOCKING -eq 1 ]; then
    return
  fi
  local np=${LOCKING_NP}
  run_all_langs "$LOCKING_BIN" "Distributed Locking" "$np"
}

# --- Run all tests
run_all_tests() {
  run_setup_tests
  run_threads_tests
  run_memory_tests
  run_teams_tests
  run_ctx_tests
  run_remote_tests
  run_atomics_tests
  run_signaling_tests
  run_collectives_tests
  run_pt2pt_synch_tests
  run_locking_tests
}

# --- Print test summary
print_summary() {
  echo -e "\n${BOLD}Test Summary:${NC}"
  echo -e "${BLUE}Total tests run: $TESTS_TOTAL${NC}"

  # FIXME: Add pass/fail counts

  echo ""

  # Print logs directory location
  echo -e "${BOLD}Test logs are available in:${NC} ${YELLOW}$SHMEMVV_LOG_DIR${NC}"
  echo -e "For detailed test results, check the log files in this directory."
  echo ""
}

# --- Apply global PE count override if specified
apply_np_override() {
  if [ -n "$OVERRIDE_NP" ]; then
    SETUP_NP=$OVERRIDE_NP
    THREADS_NP=$OVERRIDE_NP
    MEMORY_NP=$OVERRIDE_NP
    TEAMS_NP=$OVERRIDE_NP
    CTX_NP=$OVERRIDE_NP
    REMOTE_NP=$OVERRIDE_NP
    ATOMICS_NP=$OVERRIDE_NP
    SIGNALING_NP=$OVERRIDE_NP
    COLLECTIVES_NP=$OVERRIDE_NP
    PT2PT_SYNC_NP=$OVERRIDE_NP
    LOCKING_NP=$OVERRIDE_NP
  fi
}

# --- Parse command line arguments
parse_args() {
  while [[ $# -gt 0 ]]; do
    case "$1" in
    --help)
      display_usage
      exit 0
      ;;
    --enable_c11)
      ENABLE_C11=1
      ;;
    --enable_c)
      ENABLE_C=1
      ;;
    --test_setup)
      RUN_SETUP=1
      RUN_ALL=0
      ;;
    --test_threads)
      RUN_THREADS=1
      RUN_ALL=0
      ;;
    --test_mem)
      RUN_MEM=1
      RUN_ALL=0
      ;;
    --test_teams)
      RUN_TEAMS=1
      RUN_ALL=0
      ;;
    --test_ctx)
      RUN_CTX=1
      RUN_ALL=0
      ;;
    --test_remote)
      RUN_REMOTE=1
      RUN_ALL=0
      ;;
    --test_atomics)
      RUN_ATOMICS=1
      RUN_ALL=0
      ;;
    --test_signaling)
      RUN_SIGNALING=1
      RUN_ALL=0
      ;;
    --test_collectives)
      RUN_COLLECTIVES=1
      RUN_ALL=0
      ;;
    --test_pt2pt_synch)
      RUN_PT2PT_SYNCH=1
      RUN_ALL=0
      ;;
    --test_locking)
      RUN_LOCKING=1
      RUN_ALL=0
      ;;
    --test_all)
      RUN_ALL=1
      ;;
    --exclude_setup)
      EXCLUDE_SETUP=1
      ;;
    --exclude_threads)
      EXCLUDE_THREADS=1
      ;;
    --exclude_mem)
      EXCLUDE_MEM=1
      ;;
    --exclude_teams)
      EXCLUDE_TEAMS=1
      ;;
    --exclude_ctx)
      EXCLUDE_CTX=1
      ;;
    --exclude_remote)
      EXCLUDE_REMOTE=1
      ;;
    --exclude_atomics)
      EXCLUDE_ATOMICS=1
      ;;
    --exclude_signaling)
      EXCLUDE_SIGNALING=1
      ;;
    --exclude_collectives)
      EXCLUDE_COLLECTIVES=1
      ;;
    --exclude_pt2pt_synch)
      EXCLUDE_PT2PT_SYNCH=1
      ;;
    --exclude_locking)
      EXCLUDE_LOCKING=1
      ;;
    --np)
      if [[ $# -lt 2 ]]; then
        echo -e "${RED}Error: --np requires a numeric argument${NC}"
        exit 1
      fi
      if ! [[ $2 =~ ^[0-9]+$ ]]; then
        echo -e "${RED}Error: --np argument must be a number${NC}"
        exit 1
      fi
      OVERRIDE_NP=$2
      shift
      ;;
    --launcher)
      if [[ $# -lt 2 ]]; then
        echo -e "${RED}Error: --launcher requires an argument${NC}"
        exit 1
      fi
      LAUNCHER=$2
      shift
      ;;
    --launcher_args)
      if [[ $# -lt 2 ]]; then
        echo -e "${RED}Error: --launcher_args requires an argument${NC}"
        exit 1
      fi
      LAUNCHER_ARGS=$2
      shift
      ;;
    --verbose)
      VERBOSE=1
      ;;
    --no-color)
      COLOR=0
      RED=''
      GREEN=''
      YELLOW=''
      BLUE=''
      BOLD=''
      NC=''
      ;;
    *)
      echo -e "${RED}Error: Unknown option $1${NC}"
      display_usage
      exit 1
      ;;
    esac
    shift
  done
}

# --- Main execution
main() {
  parse_args "$@"
  apply_np_override

  # Check if at least one language is enabled
  if [ $ENABLE_C -eq 0 ] && [ $ENABLE_C11 -eq 0 ]; then
    echo -e "${RED}Error: You must enable at least one test language using --enable_c or --enable_c11${NC}"
    display_usage
    exit 1
  fi

  setup_environment

  echo -e ""
  echo -e "${BOLD}OpenSHMEM V&V Test Suite Runner${NC}"
  echo -e "Using launcher: $LAUNCHER $LAUNCHER_ARGS"
  echo ""

  # --- Run selected tests
  if [ $RUN_ALL -eq 1 ]; then
    run_all_tests
  else
    [ $RUN_SETUP -eq 1 ] && run_setup_tests
    [ $RUN_THREADS -eq 1 ] && run_threads_tests
    [ $RUN_MEM -eq 1 ] && run_memory_tests
    [ $RUN_TEAMS -eq 1 ] && run_teams_tests
    [ $RUN_CTX -eq 1 ] && run_ctx_tests
    [ $RUN_REMOTE -eq 1 ] && run_remote_tests
    [ $RUN_ATOMICS -eq 1 ] && run_atomics_tests
    [ $RUN_SIGNALING -eq 1 ] && run_signaling_tests
    [ $RUN_COLLECTIVES -eq 1 ] && run_collectives_tests
    [ $RUN_PT2PT_SYNCH -eq 1 ] && run_pt2pt_synch_tests
    [ $RUN_LOCKING -eq 1 ] && run_locking_tests
  fi

  print_summary
}

# Execute main with all arguments
main "$@"
