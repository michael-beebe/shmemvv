#####################################
# src/unit/c/signaling/CMakeLists.txt
#####################################

# --- Add the common source files
set(SOURCES
  ../../../shmemvv.c
)

add_executable(c11_shmem_put_signal c11_shmem_put_signal.c ${SOURCES})
add_executable(c11_shmem_put_signal_nbi c11_shmem_put_signal_nbi.c ${SOURCES})
add_executable(c11_shmem_signal_fetch c11_shmem_signal_fetch.c ${SOURCES})

# --- Set the output directory for the executables
set_target_properties(
  c11_shmem_put_signal
  c11_shmem_put_signal_nbi
  c11_shmem_signal_fetch
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/signaling
)

# --- Include directories for each target
target_include_directories(c11_shmem_put_signal PRIVATE src/include)
target_include_directories(c11_shmem_put_signal_nbi PRIVATE src/include)
target_include_directories(c11_shmem_signal_fetch PRIVATE src/include)

# --- Link any necessary libraries to each target
target_link_libraries(c11_shmem_put_signal)
target_link_libraries(c11_shmem_put_signal_nbi)
target_link_libraries(c11_shmem_signal_fetch)
