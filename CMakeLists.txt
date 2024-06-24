# cmake_minimum_required(VERSION 3.10)
# project(shmemvv)

# # ---- Enable testing
# enable_testing()

# # ---- Add include and library directories from CMake variables
# if(CMAKE_INCLUDE_PATH)
#   include_directories(${CMAKE_INCLUDE_PATH})
# endif()
# if(CMAKE_LIBRARY_PATH)
#   link_directories(${CMAKE_LIBRARY_PATH})
# endif()

# # --- Add subdirectories
# add_subdirectory(src)


#####################################################################

# ---- Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.10)

# ---- Project name and version
project(shmemvv VERSION 1.0)

# --- Enable Testing 
enable_testing()

# ---- Add subdirectory for src
add_subdirectory(src)

