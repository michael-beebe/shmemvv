cmake_minimum_required(VERSION 3.10)
project(shmemvv)

# ---- Set the module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# ---- Enable testing (if needed later)
enable_testing()

# ---- Find OpenSHMEM
find_package(OpenSHMEM)

# ---- If OpenSHMEM is not found, require the user to specify the paths
if (NOT OpenSHMEM_FOUND)
  message(WARNING "OpenSHMEM not found. Please specify the path to OpenSHMEM.")

  set(OPENSHMEM_INCLUDE_DIR "" CACHE PATH "Path to OpenSHMEM include directory")
  set(OPENSHMEM_LIBRARY "" CACHE FILEPATH "Path to OpenSHMEM library")

  if(NOT OPENSHMEM_INCLUDE_DIR)
    message(FATAL_ERROR "OPENSHMEM_INCLUDE_DIR is not set. Please specify the path to the OpenSHMEM include directory.")
  endif()

  if(NOT OPENSHMEM_LIBRARY)
    message(FATAL_ERROR "OPENSHMEM_LIBRARY is not set. Please specify the path to the OpenSHMEM library.")
  endif()

  set(OpenSHMEM_LIBRARIES ${OPENSHMEM_LIBRARY})
  set(OpenSHMEM_INCLUDE_DIRS ${OPENSHMEM_INCLUDE_DIR})
else()
  message(STATUS "OpenSHMEM found: ${OpenSHMEM_LIBRARIES}")
  message(STATUS "OpenSHMEM include directory: ${OpenSHMEM_INCLUDE_DIRS}")
endif()

