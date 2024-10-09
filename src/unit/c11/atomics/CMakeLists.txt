#####################################
# src/unit/c/atomics/CMakeLists.txt
#####################################

set(CMAKE_C_STANDARD 11)

# --- Add the common source files
set(SOURCES
    ../../../shmemvv.c
)

# --- Add the executables, one for each atomic test
add_executable(c11_shmem_atomic_fetch c11_shmem_atomic_fetch.c ${SOURCES})
add_executable(c11_shmem_atomic_set c11_shmem_atomic_set.c ${SOURCES})
add_executable(c11_shmem_atomic_compare_swap c11_shmem_atomic_compare_swap.c ${SOURCES})
add_executable(c11_shmem_atomic_swap c11_shmem_atomic_swap.c ${SOURCES})
add_executable(c11_shmem_atomic_fetch_inc c11_shmem_atomic_fetch_inc.c ${SOURCES})
add_executable(c11_shmem_atomic_inc c11_shmem_atomic_inc.c ${SOURCES})
add_executable(c11_shmem_atomic_fetch_add c11_shmem_atomic_fetch_add.c ${SOURCES})
add_executable(c11_shmem_atomic_add c11_shmem_atomic_add.c ${SOURCES})
add_executable(c11_shmem_atomic_fetch_and c11_shmem_atomic_fetch_and.c ${SOURCES})
add_executable(c11_shmem_atomic_and c11_shmem_atomic_and.c ${SOURCES})
add_executable(c11_shmem_atomic_fetch_or c11_shmem_atomic_fetch_or.c ${SOURCES})
add_executable(c11_shmem_atomic_or c11_shmem_atomic_or.c ${SOURCES})
add_executable(c11_shmem_atomic_fetch_xor c11_shmem_atomic_fetch_xor.c ${SOURCES})
add_executable(c11_shmem_atomic_xor c11_shmem_atomic_xor.c ${SOURCES})
add_executable(c11_shmem_atomic_fetch_nbi c11_shmem_atomic_fetch_nbi.c ${SOURCES})
add_executable(c11_shmem_atomic_compare_swap_nbi c11_shmem_atomic_compare_swap_nbi.c ${SOURCES})
add_executable(c11_shmem_atomic_swap_nbi c11_shmem_atomic_swap_nbi.c ${SOURCES})
add_executable(c11_shmem_atomic_fetch_inc_nbi c11_shmem_atomic_fetch_inc_nbi.c ${SOURCES})
add_executable(c11_shmem_atomic_fetch_add_nbi c11_shmem_atomic_fetch_add_nbi.c ${SOURCES})
add_executable(c11_shmem_atomic_fetch_and_nbi c11_shmem_atomic_fetch_and_nbi.c ${SOURCES})
add_executable(c11_shmem_atomic_fetch_or_nbi c11_shmem_atomic_fetch_or_nbi.c ${SOURCES})
add_executable(c11_shmem_atomic_fetch_xor_nbi c11_shmem_atomic_fetch_xor_nbi.c ${SOURCES})

# --- Set the output directory for the executables
set_target_properties(
  c11_shmem_atomic_fetch
  c11_shmem_atomic_set
  c11_shmem_atomic_compare_swap
  c11_shmem_atomic_swap
  c11_shmem_atomic_fetch_inc
  c11_shmem_atomic_inc
  c11_shmem_atomic_fetch_add
  c11_shmem_atomic_add
  c11_shmem_atomic_fetch_and
  c11_shmem_atomic_and
  c11_shmem_atomic_fetch_or
  c11_shmem_atomic_or
  c11_shmem_atomic_fetch_xor
  c11_shmem_atomic_xor
  c11_shmem_atomic_fetch_nbi
  c11_shmem_atomic_compare_swap_nbi
  c11_shmem_atomic_swap_nbi
  c11_shmem_atomic_fetch_inc_nbi
  c11_shmem_atomic_fetch_add_nbi
  c11_shmem_atomic_fetch_and_nbi
  c11_shmem_atomic_fetch_or_nbi
  c11_shmem_atomic_fetch_xor_nbi
  PROPERTIES
  # TODO: come up with a better way to organize unit tests bins
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/atomics
)

# --- Include directories for each target
target_include_directories(c11_shmem_atomic_fetch PRIVATE src/include)
target_include_directories(c11_shmem_atomic_set PRIVATE src/include)
target_include_directories(c11_shmem_atomic_compare_swap PRIVATE src/include)
target_include_directories(c11_shmem_atomic_swap PRIVATE src/include)
target_include_directories(c11_shmem_atomic_fetch_inc PRIVATE src/include)
target_include_directories(c11_shmem_atomic_inc PRIVATE src/include)
target_include_directories(c11_shmem_atomic_fetch_add PRIVATE src/include)
target_include_directories(c11_shmem_atomic_add PRIVATE src/include)
target_include_directories(c11_shmem_atomic_fetch_and PRIVATE src/include)
target_include_directories(c11_shmem_atomic_and PRIVATE src/include)
target_include_directories(c11_shmem_atomic_fetch_or PRIVATE src/include)
target_include_directories(c11_shmem_atomic_or PRIVATE src/include)
target_include_directories(c11_shmem_atomic_fetch_xor PRIVATE src/include)
target_include_directories(c11_shmem_atomic_xor PRIVATE src/include)
target_include_directories(c11_shmem_atomic_fetch_nbi PRIVATE src/include)
target_include_directories(c11_shmem_atomic_compare_swap_nbi PRIVATE src/include)
target_include_directories(c11_shmem_atomic_swap_nbi PRIVATE src/include)
target_include_directories(c11_shmem_atomic_fetch_inc_nbi PRIVATE src/include)
target_include_directories(c11_shmem_atomic_fetch_add_nbi PRIVATE src/include)
target_include_directories(c11_shmem_atomic_fetch_and_nbi PRIVATE src/include)
target_include_directories(c11_shmem_atomic_fetch_or_nbi PRIVATE src/include)
target_include_directories(c11_shmem_atomic_fetch_xor_nbi PRIVATE src/include)

# --- Link any necessary libraries to each target (replace `your_library` with actual libraries if any)
target_link_libraries(c11_shmem_atomic_fetch)
target_link_libraries(c11_shmem_atomic_set)
target_link_libraries(c11_shmem_atomic_compare_swap)
target_link_libraries(c11_shmem_atomic_swap)
target_link_libraries(c11_shmem_atomic_fetch_inc)
target_link_libraries(c11_shmem_atomic_inc)
target_link_libraries(c11_shmem_atomic_fetch_add)
target_link_libraries(c11_shmem_atomic_add)
target_link_libraries(c11_shmem_atomic_fetch_and)
target_link_libraries(c11_shmem_atomic_and)
target_link_libraries(c11_shmem_atomic_fetch_or)
target_link_libraries(c11_shmem_atomic_or)
target_link_libraries(c11_shmem_atomic_fetch_xor)
target_link_libraries(c11_shmem_atomic_xor)
target_link_libraries(c11_shmem_atomic_fetch_nbi)
target_link_libraries(c11_shmem_atomic_compare_swap_nbi)
target_link_libraries(c11_shmem_atomic_swap_nbi)
target_link_libraries(c11_shmem_atomic_fetch_inc_nbi)
target_link_libraries(c11_shmem_atomic_fetch_add_nbi)
target_link_libraries(c11_shmem_atomic_fetch_and_nbi)
target_link_libraries(c11_shmem_atomic_fetch_or_nbi)
target_link_libraries(c11_shmem_atomic_fetch_xor_nbi)
