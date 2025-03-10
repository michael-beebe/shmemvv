#####################################
# src/unit/c/threads/CMakeLists.txt
#####################################

set(CMAKE_C_STANDARD 11)

# --- Add the common source files
set(SOURCES
  ../../../shmemvv.c
  ../../../log.c
)

# --- Add the executables
add_executable(c11_shmem_init_thread c11_shmem_init_thread.c ${SOURCES})
add_executable(c11_shmem_query_thread c11_shmem_query_thread.c ${SOURCES})

# --- Set the output directory for the executables
set_target_properties(
  c11_shmem_init_thread
  c11_shmem_query_thread
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/threads
)

# --- Include directories for each target
target_include_directories(c11_shmem_init_thread PRIVATE src/include)
target_include_directories(c11_shmem_query_thread PRIVATE src/include)

# --- Link any necessary libraries to each target
target_link_libraries(c11_shmem_init_thread)
target_link_libraries(c11_shmem_query_thread)
