#####################################
# src/unit/c/memory/CMakeLists.txt
#####################################

# --- Add the common source files
set(SOURCES
  ../../../shmemvv.c
  ../../../log.c
)

# --- Add the executables
add_executable(c_shmem_addr_accessible c_shmem_addr_accessible.c ${SOURCES})
add_executable(c_shmem_align c_shmem_align.c ${SOURCES})
add_executable(c_shmem_calloc c_shmem_calloc.c ${SOURCES})
add_executable(c_shmem_malloc_free c_shmem_malloc_free.c ${SOURCES})
add_executable(c_shmem_malloc_with_hints c_shmem_malloc_with_hints.c ${SOURCES})
add_executable(c_shmem_ptr c_shmem_ptr.c ${SOURCES})
add_executable(c_shmem_realloc c_shmem_realloc.c ${SOURCES})
add_executable(c_shmem_quiet c_shmem_quiet.c ${SOURCES})
add_executable(c_shmem_fence c_shmem_fence.c ${SOURCES})

# --- Set the output directory for the executables
set_target_properties(
  c_shmem_addr_accessible
  c_shmem_align
  c_shmem_calloc
  c_shmem_malloc_free
  c_shmem_malloc_with_hints
  c_shmem_ptr
  c_shmem_realloc
  c_shmem_quiet
  c_shmem_fence
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/memory
)

# --- Include directories for each target
target_include_directories(c_shmem_addr_accessible PRIVATE src/include)
target_include_directories(c_shmem_align PRIVATE src/include)
target_include_directories(c_shmem_calloc PRIVATE src/include)
target_include_directories(c_shmem_malloc_free PRIVATE src/include)
target_include_directories(c_shmem_malloc_with_hints PRIVATE src/include)
target_include_directories(c_shmem_ptr PRIVATE src/include)
target_include_directories(c_shmem_realloc PRIVATE src/include)
target_include_directories(c_shmem_quiet PRIVATE src/include)
target_include_directories(c_shmem_fence PRIVATE src/include)

# --- Link any necessary libraries to each target
target_link_libraries(c_shmem_addr_accessible)
target_link_libraries(c_shmem_align)
target_link_libraries(c_shmem_calloc)
target_link_libraries(c_shmem_malloc_free)
target_link_libraries(c_shmem_malloc_with_hints)
target_link_libraries(c_shmem_ptr)
target_link_libraries(c_shmem_realloc)
target_link_libraries(c_shmem_quiet)
target_link_libraries(c_shmem_fence)
