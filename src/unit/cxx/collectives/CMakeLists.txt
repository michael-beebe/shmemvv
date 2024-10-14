# #####################################
# # src/unit/c/collectives/CMakeLists.txt
# #####################################

# --- Add the common source files
set(SOURCES
    ../../../shmemvv.c
)

# --- Add the executables, one for each collective test
add_executable(cxx_shmem_alltoall cxx_shmem_alltoall.cpp ${SOURCES})
add_executable(cxx_shmem_alltoalls cxx_shmem_alltoalls.cpp ${SOURCES})
add_executable(cxx_shmem_broadcast cxx_shmem_broadcast.cpp ${SOURCES})
add_executable(cxx_shmem_collect cxx_shmem_collect.cpp ${SOURCES})
add_executable(cxx_shmem_fcollect cxx_shmem_fcollect.cpp ${SOURCES})
add_executable(cxx_shmem_max_reduce cxx_shmem_max_reduce.cpp ${SOURCES})
add_executable(cxx_shmem_prod_reduce cxx_shmem_prod_reduce.cpp ${SOURCES})
add_executable(cxx_shmem_sum_reduce cxx_shmem_sum_reduce.cpp ${SOURCES})
add_executable(cxx_shmem_sync_all cxx_shmem_sync_all.cpp ${SOURCES})

# --- Set the output directory for the executables
set_target_properties(
  cxx_shmem_alltoall
  cxx_shmem_alltoalls
  cxx_shmem_broadcast
  cxx_shmem_collect
  cxx_shmem_fcollect
  cxx_shmem_max_reduce
  cxx_shmem_prod_reduce
  cxx_shmem_sum_reduce
  cxx_shmem_sync_all
  
  PROPERTIES
  # TODO: come up with a better way to organize unit tests bins
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/collectives
)

# --- Include directories for each target
target_include_directories(cxx_shmem_alltoall PRIVATE src/include)
target_include_directories(cxx_shmem_alltoalls PRIVATE src/include)
target_include_directories(cxx_shmem_broadcast PRIVATE src/include)
target_include_directories(cxx_shmem_collect PRIVATE src/include)
target_include_directories(cxx_shmem_fcollect PRIVATE src/include)
target_include_directories(cxx_shmem_max_reduce PRIVATE src/include)
target_include_directories(cxx_shmem_prod_reduce PRIVATE src/include)
target_include_directories(cxx_shmem_sum_reduce PRIVATE src/include)
target_include_directories(cxx_shmem_sync_all PRIVATE src/include)

# --- Link any necessary libraries to each target (replace `your_library` with actual libraries if any)
target_link_libraries(cxx_shmem_alltoall)
target_link_libraries(cxx_shmem_alltoalls)
target_link_libraries(cxx_shmem_broadcast)
target_link_libraries(cxx_shmem_collect)
target_link_libraries(cxx_shmem_fcollect)
target_link_libraries(cxx_shmem_max_reduce)
target_link_libraries(cxx_shmem_prod_reduce)
target_link_libraries(cxx_shmem_sum_reduce)
target_link_libraries(cxx_shmem_sync_all)
