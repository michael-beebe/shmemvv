#####################################
# src/unit/c/signaling/CMakeLists.txt
#####################################

# --- Add the common source files
set(SOURCES
  ../../../shmemvv.c
  ../../../log.c
)

add_executable(cxx_shmem_put_signal cxx_shmem_put_signal.cpp ${SOURCES})
add_executable(cxx_shmem_put_signal_nbi cxx_shmem_put_signal_nbi.cpp ${SOURCES})
add_executable(cxx_shmem_signal_fetch cxx_shmem_signal_fetch.cpp ${SOURCES})

# --- Set the output directory for the executables
set_target_properties(
  cxx_shmem_put_signal
  cxx_shmem_put_signal_nbi
  cxx_shmem_signal_fetch
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/signaling
)

# --- Include directories for each target
target_include_directories(cxx_shmem_put_signal PRIVATE src/include)
target_include_directories(cxx_shmem_put_signal_nbi PRIVATE src/include)
target_include_directories(cxx_shmem_signal_fetch PRIVATE src/include)

# --- Link any necessary libraries to each target
target_link_libraries(cxx_shmem_put_signal)
target_link_libraries(cxx_shmem_put_signal_nbi)
target_link_libraries(cxx_shmem_signal_fetch)
