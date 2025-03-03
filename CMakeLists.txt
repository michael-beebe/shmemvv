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
add_subdirectory(src)