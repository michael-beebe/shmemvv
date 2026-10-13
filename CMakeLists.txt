# ---- Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.10)

# ---- Project name and version
project(shmemvv VERSION 1.5.1)

# ---- Add debug option
option(DEBUG "Enable debugging output" OFF)

# ---- Add debug definition
if (DEBUG)
  add_definitions(-D_DEBUG_)
endif()

# ---- Add verbose definition
if (VERBOSE)
  add_definitions(-D_VERBOSE_)
endif()

# ---- Global language test options
option(DISABLE_ALL_C_TESTS "Disable all C language tests" OFF)
option(DISABLE_ALL_C11_TESTS "Disable all C11 language tests" OFF)

# ---- Test category options
# These options allow you to selectively disable specific test categories
# All test categories are built by default unless explicitly disabled.
# Example: cmake -DDISABLE_C_ATOMICS=ON -DDISABLE_C11_COLLECTIVES=ON ..
#
# Global language options:
# DISABLE_ALL_C_TESTS - Disable all C language tests
# DISABLE_ALL_C11_TESTS - Disable all C11 language tests
#
# Available options for C tests:
# DISABLE_C_ATOMICS - Disable C atomic memory operation tests
# DISABLE_C_COLLECTIVES - Disable C collective operation tests
# DISABLE_C_RMA - Disable C remote memory access tests
# DISABLE_C_CTX - Disable C context management tests
# DISABLE_C_LOCKING - Disable C distributed locking tests
# DISABLE_C_MEMORY - Disable C memory management tests
# DISABLE_C_PT2PT_SYNC - Disable C point-to-point synchronization tests
# DISABLE_C_SETUP - Disable C setup tests
# DISABLE_C_SIGNALING - Disable C signaling operation tests
# DISABLE_C_TEAMS - Disable C teams management tests
# DISABLE_C_THREADS - Disable C thread support tests
#
# Available options for C11 tests:
# DISABLE_C11_ATOMICS - Disable C11 atomic memory operation tests
# DISABLE_C11_COLLECTIVES - Disable C11 collective operation tests
# DISABLE_C11_RMA - Disable C11 remote memory access tests
# DISABLE_C11_CTX - Disable C11 context management tests
# DISABLE_C11_LOCKING - Disable C11 distributed locking tests
# DISABLE_C11_MEMORY - Disable C11 memory management tests
# DISABLE_C11_PT2PT_SYNC - Disable C11 point-to-point synchronization tests
# DISABLE_C11_SETUP - Disable C11 setup tests
# DISABLE_C11_SIGNALING - Disable C11 signaling operation tests
# DISABLE_C11_TEAMS - Disable C11 teams management tests
# DISABLE_C11_THREADS - Disable C11 thread support tests

# ---- Check for dlfcn.h
# include(CheckIncludeFile)
# check_include_file("dlfcn.h" HAVE_DLFCN_H)

# if (NOT HAVE_DLFCN_H)
#   message(FATAL_ERROR "dlfcn.h not found, but required!")
# endif()

# ---- Add include directory
include_directories(${CMAKE_SOURCE_DIR}/src/include)

# ---- Add subdirectory for src
include(CheckSymbolExists)
include(CheckCSourceCompiles)
check_symbol_exists(shmem_ctx_session_start "shmem.h" SHMEMVV_HAVE_SESSIONS)
check_symbol_exists(shmem_query_initialized "shmem.h" SHMEMVV_HAVE_QUERY_INITIALIZED)
check_symbol_exists(shmem_ctx_pe_quiet "shmem.h" SHMEMVV_HAVE_PE_QUIET)
check_symbol_exists(shmem_ctx_int_ibput "shmem.h" SHMEMVV_HAVE_IBPUT)
check_symbol_exists(shmem_ctx_int_ibget "shmem.h" SHMEMVV_HAVE_IBGET)
check_symbol_exists(shmem_ctx_signal_set "shmem.h" SHMEMVV_HAVE_SIGNAL_SET)
check_symbol_exists(shmem_ctx_signal_add "shmem.h" SHMEMVV_HAVE_SIGNAL_ADD)
check_symbol_exists(shmem_int_sum_inscan "shmem.h" SHMEMVV_HAVE_INSCAN)
check_symbol_exists(shmem_int_sum_exscan "shmem.h" SHMEMVV_HAVE_EXSCAN)
check_symbol_exists(pshmem_int_ibput "pshmem.h" SHMEMVV_HAVE_PSHMEM_IBPUT)
check_symbol_exists(pshmem_int_ibget "pshmem.h" SHMEMVV_HAVE_PSHMEM_IBGET)
set(saved_required_flags "${CMAKE_REQUIRED_FLAGS}")
set(CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS} -std=c11")
check_c_source_compiles("\
#include <shmem.h>
#define CHECK(fn, type) _Static_assert(_Generic(&(fn), type: 1, default: 0), #fn)
CHECK(shmem_int_test_all_vector, int (*)(int *, size_t, const int *, int, const int *));
CHECK(shmem_int_test_any_vector, size_t (*)(int *, size_t, const int *, int, const int *));
CHECK(shmem_int_test_some_vector, size_t (*)(int *, size_t, size_t *, const int *, int, const int *));
CHECK(shmem_int_wait_until_all_vector, void (*)(int *, size_t, const int *, int, const int *));
CHECK(shmem_int_wait_until_any_vector, size_t (*)(int *, size_t, const int *, int, const int *));
CHECK(shmem_int_wait_until_some_vector, size_t (*)(int *, size_t, size_t *, const int *, int, const int *));
int main(void) { return 0; }
" SHMEMVV_HAVE_CONST_VECTOR)
set(CMAKE_REQUIRED_FLAGS "${saved_required_flags}")

# Older implementations can still build the existing tests. A 1.6 validation
# run must request the complete surface rather than silently omit tests.
option(REQUIRE_OPENSHMEM_1_6 "Require all OpenSHMEM 1.6 test families" OFF)
if(REQUIRE_OPENSHMEM_1_6)
  foreach(feature SESSIONS QUERY_INITIALIZED PE_QUIET IBPUT IBGET
                  SIGNAL_SET SIGNAL_ADD INSCAN EXSCAN CONST_VECTOR)
    if(NOT SHMEMVV_HAVE_${feature})
      message(FATAL_ERROR "Missing OpenSHMEM 1.6 API: ${feature}")
    endif()
  endforeach()
endif()
add_subdirectory(src)
