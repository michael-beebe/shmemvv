#####################################
# src/unit/c/signaling/CMakeLists.txt
#####################################

# --- Add the common source files
set(SOURCES
  ../../../shmemvv.c
  ../../../log.c
)

add_executable(c_shmem_put_signal c_shmem_put_signal.c ${SOURCES})
add_executable(c_shmem_put_signal_nbi c_shmem_put_signal_nbi.c ${SOURCES})
add_executable(c_shmem_signal_fetch c_shmem_signal_fetch.c ${SOURCES})

# --- Set the output directory for the executables
set_target_properties(
  c_shmem_put_signal
  c_shmem_put_signal_nbi
  c_shmem_signal_fetch
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/signaling
)

# --- Include directories for each target
target_include_directories(c_shmem_put_signal PRIVATE src/include)
target_include_directories(c_shmem_put_signal_nbi PRIVATE src/include)
target_include_directories(c_shmem_signal_fetch PRIVATE src/include)

# --- Link any necessary libraries to each target
target_link_libraries(c_shmem_put_signal)
target_link_libraries(c_shmem_put_signal_nbi)
target_link_libraries(c_shmem_signal_fetch)

# --- OpenSHMEM 1.6 coverage
if(SHMEMVV_HAVE_SIGNAL_SET)
  add_executable(c_shmem_signal_set c_shmem_signal_set.c ${SOURCES})
  set_target_properties(c_shmem_signal_set PROPERTIES
    C_STANDARD 11 C_STANDARD_REQUIRED YES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/signaling)
endif()
if(SHMEMVV_HAVE_SIGNAL_ADD)
  add_executable(c_shmem_signal_add c_shmem_signal_add.c ${SOURCES})
  set_target_properties(c_shmem_signal_add PROPERTIES
    C_STANDARD 11 C_STANDARD_REQUIRED YES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/signaling)
endif()
