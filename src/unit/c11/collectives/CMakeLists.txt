# #####################################
# # src/unit/c/collectives/CMakeLists.txt
# #####################################

set(CMAKE_C_STANDARD 11)

# --- Add the common source files
set(SOURCES
    ../../../shmemvv.c
    ../../../log.c
)

# --- Add the executables, one for each collective test
add_executable(c11_shmem_alltoall c11_shmem_alltoall.c ${SOURCES})
add_executable(c11_shmem_alltoalls c11_shmem_alltoalls.c ${SOURCES})
add_executable(c11_shmem_broadcast c11_shmem_broadcast.c ${SOURCES})
add_executable(c11_shmem_collect c11_shmem_collect.c ${SOURCES})
add_executable(c11_shmem_fcollect c11_shmem_fcollect.c ${SOURCES})
add_executable(c11_shmem_reduce c11_shmem_reduce.c ${SOURCES})
add_executable(c11_shmem_sync_all c11_shmem_sync_all.c ${SOURCES})
add_executable(c11_shmem_sync c11_shmem_sync.c ${SOURCES})

# --- Set the output directory for the executables
set_target_properties(
  c11_shmem_alltoall
  c11_shmem_alltoalls
  c11_shmem_broadcast
  c11_shmem_collect
  c11_shmem_fcollect
  c11_shmem_reduce
  c11_shmem_sync_all
  c11_shmem_sync
  
  PROPERTIES
  # TODO: come up with a better way to organize unit tests bins
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/collectives
)

# --- Include directories for each target
target_include_directories(c11_shmem_alltoall PRIVATE src/include)
target_include_directories(c11_shmem_alltoalls PRIVATE src/include)
target_include_directories(c11_shmem_broadcast PRIVATE src/include)
target_include_directories(c11_shmem_collect PRIVATE src/include)
target_include_directories(c11_shmem_fcollect PRIVATE src/include)
target_include_directories(c11_shmem_reduce PRIVATE src/include)
target_include_directories(c11_shmem_sync_all PRIVATE src/include)
target_include_directories(c11_shmem_sync PRIVATE src/include)

# --- Link any necessary libraries to each target 
target_link_libraries(c11_shmem_alltoall)
target_link_libraries(c11_shmem_alltoalls)
target_link_libraries(c11_shmem_broadcast)
target_link_libraries(c11_shmem_collect)
target_link_libraries(c11_shmem_fcollect)
target_link_libraries(c11_shmem_reduce)
target_link_libraries(c11_shmem_sync_all)
target_link_libraries(c11_shmem_sync)

# --- OpenSHMEM 1.6 coverage
if(SHMEMVV_HAVE_INSCAN)
  add_executable(c11_shmem_sum_inscan ../../c/collectives/c_shmem_sum_inscan.c ${SOURCES})
  target_compile_definitions(c11_shmem_sum_inscan PRIVATE API_USE_GENERICS)
  set_target_properties(c11_shmem_sum_inscan PROPERTIES
    C_STANDARD 11 C_STANDARD_REQUIRED YES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/collectives)
endif()
if(SHMEMVV_HAVE_EXSCAN)
  add_executable(c11_shmem_sum_exscan ../../c/collectives/c_shmem_sum_exscan.c ${SOURCES})
  target_compile_definitions(c11_shmem_sum_exscan PRIVATE API_USE_GENERICS)
  set_target_properties(c11_shmem_sum_exscan PROPERTIES
    C_STANDARD 11 C_STANDARD_REQUIRED YES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/collectives)
endif()
