#####################################
# src/unit/c/atomics/CMakeLists.txt
#####################################

# --- Add the common source files
set(SOURCES
    ../../../shmemvv.c
)

# --- Add the executables, one for each atomic test
add_executable(cxx_shmem_atomic_fetch cxx_shmem_atomic_fetch.cpp ${SOURCES})
add_executable(cxx_shmem_atomic_set cxx_shmem_atomic_set.cpp ${SOURCES})
add_executable(cxx_shmem_atomic_compare_swap cxx_shmem_atomic_compare_swap.cpp ${SOURCES})
add_executable(cxx_shmem_atomic_swap cxx_shmem_atomic_swap.cpp ${SOURCES})
add_executable(cxx_shmem_atomic_fetch_inc cxx_shmem_atomic_fetch_inc.cpp ${SOURCES})
add_executable(cxx_shmem_atomic_inc cxx_shmem_atomic_inc.cpp ${SOURCES})
add_executable(cxx_shmem_atomic_fetch_add cxx_shmem_atomic_fetch_add.cpp ${SOURCES})
add_executable(cxx_shmem_atomic_add cxx_shmem_atomic_add.cpp ${SOURCES})
add_executable(cxx_shmem_atomic_fetch_and cxx_shmem_atomic_fetch_and.cpp ${SOURCES})
add_executable(cxx_shmem_atomic_and cxx_shmem_atomic_and.cpp ${SOURCES})
add_executable(cxx_shmem_atomic_fetch_or cxx_shmem_atomic_fetch_or.cpp ${SOURCES})
add_executable(cxx_shmem_atomic_or cxx_shmem_atomic_or.cpp ${SOURCES})
add_executable(cxx_shmem_atomic_fetch_xor cxx_shmem_atomic_fetch_xor.cpp ${SOURCES})
add_executable(cxx_shmem_atomic_xor cxx_shmem_atomic_xor.cpp ${SOURCES})
add_executable(cxx_shmem_atomic_fetch_nbi cxx_shmem_atomic_fetch_nbi.cpp ${SOURCES})
add_executable(cxx_shmem_atomic_compare_swap_nbi cxx_shmem_atomic_compare_swap_nbi.cpp ${SOURCES})
add_executable(cxx_shmem_atomic_swap_nbi cxx_shmem_atomic_swap_nbi.cpp ${SOURCES})
add_executable(cxx_shmem_atomic_fetch_inc_nbi cxx_shmem_atomic_fetch_inc_nbi.cpp ${SOURCES})
add_executable(cxx_shmem_atomic_fetch_add_nbi cxx_shmem_atomic_fetch_add_nbi.cpp ${SOURCES})
add_executable(cxx_shmem_atomic_fetch_and_nbi cxx_shmem_atomic_fetch_and_nbi.cpp ${SOURCES})
add_executable(cxx_shmem_atomic_fetch_or_nbi cxx_shmem_atomic_fetch_or_nbi.cpp ${SOURCES})
add_executable(cxx_shmem_atomic_fetch_xor_nbi cxx_shmem_atomic_fetch_xor_nbi.cpp ${SOURCES})

# --- Set the output directory for the executables
set_target_properties(
  cxx_shmem_atomic_fetch
  cxx_shmem_atomic_set
  cxx_shmem_atomic_compare_swap
  cxx_shmem_atomic_swap
  cxx_shmem_atomic_fetch_inc
  cxx_shmem_atomic_inc
  cxx_shmem_atomic_fetch_add
  cxx_shmem_atomic_add
  cxx_shmem_atomic_fetch_and
  cxx_shmem_atomic_and
  cxx_shmem_atomic_fetch_or
  cxx_shmem_atomic_or
  cxx_shmem_atomic_fetch_xor
  cxx_shmem_atomic_xor
  cxx_shmem_atomic_fetch_nbi
  cxx_shmem_atomic_compare_swap_nbi
  cxx_shmem_atomic_swap_nbi
  cxx_shmem_atomic_fetch_inc_nbi
  cxx_shmem_atomic_fetch_add_nbi
  cxx_shmem_atomic_fetch_and_nbi
  cxx_shmem_atomic_fetch_or_nbi
  cxx_shmem_atomic_fetch_xor_nbi
  PROPERTIES
  # TODO: come up with a better way to organize unit tests bins
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/atomics
)

# --- Include directories for each target
target_include_directories(cxx_shmem_atomic_fetch PRIVATE src/include)
target_include_directories(cxx_shmem_atomic_set PRIVATE src/include)
target_include_directories(cxx_shmem_atomic_compare_swap PRIVATE src/include)
target_include_directories(cxx_shmem_atomic_swap PRIVATE src/include)
target_include_directories(cxx_shmem_atomic_fetch_inc PRIVATE src/include)
target_include_directories(cxx_shmem_atomic_inc PRIVATE src/include)
target_include_directories(cxx_shmem_atomic_fetch_add PRIVATE src/include)
target_include_directories(cxx_shmem_atomic_add PRIVATE src/include)
target_include_directories(cxx_shmem_atomic_fetch_and PRIVATE src/include)
target_include_directories(cxx_shmem_atomic_and PRIVATE src/include)
target_include_directories(cxx_shmem_atomic_fetch_or PRIVATE src/include)
target_include_directories(cxx_shmem_atomic_or PRIVATE src/include)
target_include_directories(cxx_shmem_atomic_fetch_xor PRIVATE src/include)
target_include_directories(cxx_shmem_atomic_xor PRIVATE src/include)
target_include_directories(cxx_shmem_atomic_fetch_nbi PRIVATE src/include)
target_include_directories(cxx_shmem_atomic_compare_swap_nbi PRIVATE src/include)
target_include_directories(cxx_shmem_atomic_swap_nbi PRIVATE src/include)
target_include_directories(cxx_shmem_atomic_fetch_inc_nbi PRIVATE src/include)
target_include_directories(cxx_shmem_atomic_fetch_add_nbi PRIVATE src/include)
target_include_directories(cxx_shmem_atomic_fetch_and_nbi PRIVATE src/include)
target_include_directories(cxx_shmem_atomic_fetch_or_nbi PRIVATE src/include)
target_include_directories(cxx_shmem_atomic_fetch_xor_nbi PRIVATE src/include)

# --- Link any necessary libraries to each target (replace `your_library` with actual libraries if any)
target_link_libraries(cxx_shmem_atomic_fetch)
target_link_libraries(cxx_shmem_atomic_set)
target_link_libraries(cxx_shmem_atomic_compare_swap)
target_link_libraries(cxx_shmem_atomic_swap)
target_link_libraries(cxx_shmem_atomic_fetch_inc)
target_link_libraries(cxx_shmem_atomic_inc)
target_link_libraries(cxx_shmem_atomic_fetch_add)
target_link_libraries(cxx_shmem_atomic_add)
target_link_libraries(cxx_shmem_atomic_fetch_and)
target_link_libraries(cxx_shmem_atomic_and)
target_link_libraries(cxx_shmem_atomic_fetch_or)
target_link_libraries(cxx_shmem_atomic_or)
target_link_libraries(cxx_shmem_atomic_fetch_xor)
target_link_libraries(cxx_shmem_atomic_xor)
target_link_libraries(cxx_shmem_atomic_fetch_nbi)
target_link_libraries(cxx_shmem_atomic_compare_swap_nbi)
target_link_libraries(cxx_shmem_atomic_swap_nbi)
target_link_libraries(cxx_shmem_atomic_fetch_inc_nbi)
target_link_libraries(cxx_shmem_atomic_fetch_add_nbi)
target_link_libraries(cxx_shmem_atomic_fetch_and_nbi)
target_link_libraries(cxx_shmem_atomic_fetch_or_nbi)
target_link_libraries(cxx_shmem_atomic_fetch_xor_nbi)
