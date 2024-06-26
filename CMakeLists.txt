# ---- Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.10)

# ---- Project name and version
project(shmemvv VERSION 1.0)

# --- Enable Testing 
enable_testing()

# ---- Add subdirectory for src
add_subdirectory(src)

