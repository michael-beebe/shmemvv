# #####################################
# # src/unit/c/collectives/CMakeLists.txt
# #####################################

# --- Add the common source files
set(SOURCES
    ../../../shmemvv.c
    ../../../log.c
)

# --- Add the executables, one for each collective test
add_executable(c_shmem_alltoall c_shmem_alltoall.c ${SOURCES})
add_executable(c_shmem_alltoallmem c_shmem_alltoallmem.c ${SOURCES})
add_executable(c_shmem_alltoalls c_shmem_alltoalls.c ${SOURCES})
add_executable(c_shmem_alltoallsmem c_shmem_alltoallsmem.c ${SOURCES})
add_executable(c_shmem_broadcast c_shmem_broadcast.c ${SOURCES})
add_executable(c_shmem_broadcastmem c_shmem_broadcastmem.c ${SOURCES})
add_executable(c_shmem_collect c_shmem_collect.c ${SOURCES})
add_executable(c_shmem_collectmem c_shmem_collectmem.c ${SOURCES})
add_executable(c_shmem_fcollect c_shmem_fcollect.c ${SOURCES})
add_executable(c_shmem_fcollectmem c_shmem_fcollectmem.c ${SOURCES})
add_executable(c_shmem_reduce c_shmem_reduce.c ${SOURCES})
add_executable(c_shmem_sync_all c_shmem_sync_all.c ${SOURCES})
add_executable(c_shmem_team_sync c_shmem_team_sync.c ${SOURCES})

# --- Set the output directory for the executables
set_target_properties(
  c_shmem_alltoall
  c_shmem_alltoallmem
  c_shmem_alltoalls
  c_shmem_alltoallsmem
  c_shmem_broadcast
  c_shmem_broadcastmem
  c_shmem_collect
  c_shmem_collectmem
  c_shmem_fcollect
  c_shmem_fcollectmem
  c_shmem_reduce
  c_shmem_sync_all
  c_shmem_team_sync
  
  PROPERTIES
  # TODO: come up with a better way to organize unit tests bins
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/collectives
)

# --- Include directories for each target
target_include_directories(c_shmem_alltoall PRIVATE src/include)
target_include_directories(c_shmem_alltoallmem PRIVATE src/include)
target_include_directories(c_shmem_alltoalls PRIVATE src/include)
target_include_directories(c_shmem_alltoallsmem PRIVATE src/include)
target_include_directories(c_shmem_broadcast PRIVATE src/include)
target_include_directories(c_shmem_broadcastmem PRIVATE src/include)
target_include_directories(c_shmem_collect PRIVATE src/include)
target_include_directories(c_shmem_collectmem PRIVATE src/include)
target_include_directories(c_shmem_fcollect PRIVATE src/include)
target_include_directories(c_shmem_fcollectmem PRIVATE src/include)
target_include_directories(c_shmem_reduce PRIVATE src/include)
target_include_directories(c_shmem_sync_all PRIVATE src/include)
target_include_directories(c_shmem_team_sync PRIVATE src/include)

# --- Link any necessary libraries to each target (replace `your_library` with actual libraries if any)
target_link_libraries(c_shmem_alltoall)
target_link_libraries(c_shmem_alltoallmem)
target_link_libraries(c_shmem_alltoalls)
target_link_libraries(c_shmem_alltoallsmem)
target_link_libraries(c_shmem_broadcast)
target_link_libraries(c_shmem_broadcastmem)
target_link_libraries(c_shmem_collect)
target_link_libraries(c_shmem_collectmem)
target_link_libraries(c_shmem_fcollect)
target_link_libraries(c_shmem_fcollectmem)
target_link_libraries(c_shmem_reduce)
target_link_libraries(c_shmem_sync_all)
target_link_libraries(c_shmem_team_sync)