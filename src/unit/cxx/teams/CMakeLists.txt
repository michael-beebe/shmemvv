#####################################
# src/unit/c/teams/CMakeLists.txt
#####################################

# --- Add the common source files
set(SOURCES
  ../../../shmemvv.c
)

# --- Add the executables
add_executable(cxx_shmem_team_destroy cxx_shmem_team_destroy.cpp ${SOURCES})
add_executable(cxx_shmem_team_get_config cxx_shmem_team_get_config.cpp ${SOURCES})
add_executable(cxx_shmem_team_my_pe cxx_shmem_team_my_pe.cpp ${SOURCES})
add_executable(cxx_shmem_team_n_pes cxx_shmem_team_n_pes.cpp ${SOURCES})
add_executable(cxx_shmem_team_split_2d cxx_shmem_team_split_2d.cpp ${SOURCES})
add_executable(cxx_shmem_team_split_strided cxx_shmem_team_split_strided.cpp ${SOURCES})
add_executable(cxx_shmem_team_sync cxx_shmem_team_sync.cpp ${SOURCES})
add_executable(cxx_shmem_team_translate_pe cxx_shmem_team_translate_pe.cpp ${SOURCES})

# --- Set the output directory for the executables
set_target_properties(
  cxx_shmem_team_destroy
  cxx_shmem_team_get_config
  cxx_shmem_team_my_pe
  cxx_shmem_team_n_pes
  cxx_shmem_team_split_2d
  cxx_shmem_team_split_strided
  cxx_shmem_team_sync
  cxx_shmem_team_translate_pe
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/teams
)

# --- Include directories for each target
target_include_directories(cxx_shmem_team_destroy PRIVATE src/include)
target_include_directories(cxx_shmem_team_get_config PRIVATE src/include)
target_include_directories(cxx_shmem_team_my_pe PRIVATE src/include)
target_include_directories(cxx_shmem_team_n_pes PRIVATE src/include)
target_include_directories(cxx_shmem_team_split_2d PRIVATE src/include)
target_include_directories(cxx_shmem_team_split_strided PRIVATE src/include)
target_include_directories(cxx_shmem_team_sync PRIVATE src/include)
target_include_directories(cxx_shmem_team_translate_pe PRIVATE src/include)

# --- Link any necessary libraries to each target
target_link_libraries(cxx_shmem_team_destroy)
target_link_libraries(cxx_shmem_team_get_config)
target_link_libraries(cxx_shmem_team_my_pe)
target_link_libraries(cxx_shmem_team_n_pes)
target_link_libraries(cxx_shmem_team_split_2d)
target_link_libraries(cxx_shmem_team_split_strided)
target_link_libraries(cxx_shmem_team_sync)
target_link_libraries(cxx_shmem_team_translate_pe)
