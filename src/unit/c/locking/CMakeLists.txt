#####################################
# src/unit/c/locking/CMakeLists.txt
#####################################

# --- Add the common source files
set(SOURCES
  ../../../shmemvv.c
)

# --- Add the executables
add_executable(c_shmem_lock_unlock c_shmem_lock_unlock.c ${SOURCES})

# --- Set the output directory for the executables
set_target_properties(
  c_shmem_lock_unlock
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/locking
)

# --- Include directories for each target
target_include_directories(c_shmem_lock_unlock PRIVATE src/include)

# --- Link any necessary libraries to each target
target_link_libraries(c_shmem_lock_unlock)
