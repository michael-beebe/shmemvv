#####################################
# src/unit/c/memory/CMakeLists.txt
#####################################

# --- Add the common source files
set(SOURCES
  ../../../shmemvv.c
  ../../../log.c
)

# --- Add the executables
add_executable(cxx_shmem_addr_accessible cxx_shmem_addr_accessible.cpp ${SOURCES})
add_executable(cxx_shmem_align cxx_shmem_align.cpp ${SOURCES})
add_executable(cxx_shmem_calloc cxx_shmem_calloc.cpp ${SOURCES})
add_executable(cxx_shmem_malloc_free cxx_shmem_malloc_free.cpp ${SOURCES})
add_executable(cxx_shmem_malloc_with_hints cxx_shmem_malloc_with_hints.cpp ${SOURCES})
add_executable(cxx_shmem_ptr cxx_shmem_ptr.cpp ${SOURCES})
add_executable(cxx_shmem_realloc cxx_shmem_realloc.cpp ${SOURCES})
add_executable(cxx_shmem_quiet cxx_shmem_quiet.cpp ${SOURCES})
add_executable(cxx_shmem_fence cxx_shmem_fence.cpp ${SOURCES})

# --- Set the output directory for the executables
set_target_properties(
  cxx_shmem_addr_accessible
  cxx_shmem_align
  cxx_shmem_calloc
  cxx_shmem_malloc_free
  cxx_shmem_malloc_with_hints
  cxx_shmem_ptr
  cxx_shmem_realloc
  cxx_shmem_quiet
  cxx_shmem_fence
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/memory
)

# --- Include directories for each target
target_include_directories(cxx_shmem_addr_accessible PRIVATE src/include)
target_include_directories(cxx_shmem_align PRIVATE src/include)
target_include_directories(cxx_shmem_calloc PRIVATE src/include)
target_include_directories(cxx_shmem_malloc_free PRIVATE src/include)
target_include_directories(cxx_shmem_malloc_with_hints PRIVATE src/include)
target_include_directories(cxx_shmem_ptr PRIVATE src/include)
target_include_directories(cxx_shmem_realloc PRIVATE src/include)
target_include_directories(cxx_shmem_quiet PRIVATE src/include)
target_include_directories(cxx_shmem_fence PRIVATE src/include)

# --- Link any necessary libraries to each target
target_link_libraries(cxx_shmem_addr_accessible)
target_link_libraries(cxx_shmem_align)
target_link_libraries(cxx_shmem_calloc)
target_link_libraries(cxx_shmem_malloc_free)
target_link_libraries(cxx_shmem_malloc_with_hints)
target_link_libraries(cxx_shmem_ptr)
target_link_libraries(cxx_shmem_realloc)
target_link_libraries(cxx_shmem_quiet)
target_link_libraries(cxx_shmem_fence)
