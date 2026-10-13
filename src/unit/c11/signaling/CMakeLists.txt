#####################################
# src/unit/c/signaling/CMakeLists.txt
#####################################

# --- Add the common source files
set(SOURCES
  ../../../shmemvv.c
  ../../../log.c
)

add_executable(c11_shmem_put_signal c11_shmem_put_signal.c ${SOURCES})
add_executable(c11_shmem_put_signal_nbi c11_shmem_put_signal_nbi.c ${SOURCES})

# --- Set the output directory for the executables
set_target_properties(
  c11_shmem_put_signal
  c11_shmem_put_signal_nbi
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/signaling
)

# --- Include directories for each target
target_include_directories(c11_shmem_put_signal PRIVATE src/include)
target_include_directories(c11_shmem_put_signal_nbi PRIVATE src/include)

# --- Link any necessary libraries to each target
target_link_libraries(c11_shmem_put_signal)
target_link_libraries(c11_shmem_put_signal_nbi)

# --- OpenSHMEM 1.6 coverage
if(SHMEMVV_HAVE_SIGNAL_SET)
  add_executable(c11_shmem_signal_set ../../c/signaling/c_shmem_signal_set.c ${SOURCES})
  target_compile_definitions(c11_shmem_signal_set PRIVATE API_USE_GENERICS)
  set_target_properties(c11_shmem_signal_set PROPERTIES
    C_STANDARD 11 C_STANDARD_REQUIRED YES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/signaling)
endif()
if(SHMEMVV_HAVE_SIGNAL_ADD)
  add_executable(c11_shmem_signal_add ../../c/signaling/c_shmem_signal_add.c ${SOURCES})
  target_compile_definitions(c11_shmem_signal_add PRIVATE API_USE_GENERICS)
  set_target_properties(c11_shmem_signal_add PROPERTIES
    C_STANDARD 11 C_STANDARD_REQUIRED YES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/signaling)
endif()
