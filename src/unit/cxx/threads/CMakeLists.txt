#####################################
# src/unit/c/threads/CMakeLists.txt
#####################################

# --- Add the common source files
set(SOURCES
  ../../../shmemvv.c
  ../../../log.c
)

# --- Add the executables
add_executable(cxx_shmem_init_thread cxx_shmem_init_thread.cpp ${SOURCES})
add_executable(cxx_shmem_query_thread cxx_shmem_query_thread.cpp ${SOURCES})

# --- Set the output directory for the executables
set_target_properties(
  cxx_shmem_init_thread
  cxx_shmem_query_thread
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/threads
)

# --- Include directories for each target
target_include_directories(cxx_shmem_init_thread PRIVATE src/include)
target_include_directories(cxx_shmem_query_thread PRIVATE src/include)

# --- Link any necessary libraries to each target
target_link_libraries(cxx_shmem_init_thread)
target_link_libraries(cxx_shmem_query_thread)
