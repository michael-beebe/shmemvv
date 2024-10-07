# ---- Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.10)

# ---- Project name and version
project(shmemvv VERSION 1.0)

# ---- Enable Testing 
enable_testing()

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