#####################################
# src/unit/c/atomics/CMakeLists.txt
#####################################

# ---- Add executable for c_shmem_atomic_fetch
add_executable(c_shmem_atomic_fetch c_shmem_atomic_fetch.c)
set_target_properties(c_shmem_atomic_fetch PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

