# ---- Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.10)

# ---- Project name and version
project(shmemvv VERSION 1.0)

# --- Enable Testing 
enable_testing()

# ---- Add debug option
option(DEBUG "Enable debugging output" OFF)

if (DEBUG)
  add_definitions(-D_DEBUG_)
endif()

# ---- Add subdirectory for src
add_subdirectory(src)
