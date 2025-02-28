#####################################
# src/unit/c/teams/CMakeLists.txt
#####################################

set(CMAKE_C_STANDARD 11)

# --- Add the common source files
set(SOURCES
  ../../../shmemvv.c
  ../../../log.c
)

# --- Add the executables
add_executable(c11_shmem_team_destroy c11_shmem_team_destroy.c ${SOURCES})
add_executable(c11_shmem_team_get_config c11_shmem_team_get_config.c ${SOURCES})
add_executable(c11_shmem_team_my_pe c11_shmem_team_my_pe.c ${SOURCES})
add_executable(c11_shmem_team_n_pes c11_shmem_team_n_pes.c ${SOURCES})
add_executable(c11_shmem_team_split_2d c11_shmem_team_split_2d.c ${SOURCES})
add_executable(c11_shmem_team_split_strided c11_shmem_team_split_strided.c ${SOURCES})
add_executable(c11_shmem_team_sync c11_shmem_team_sync.c ${SOURCES})
add_executable(c11_shmem_team_translate_pe c11_shmem_team_translate_pe.c ${SOURCES})

# --- Set the output directory for the executables
set_target_properties(
  c11_shmem_team_destroy
  c11_shmem_team_get_config
  c11_shmem_team_my_pe
  c11_shmem_team_n_pes
  c11_shmem_team_split_2d
  c11_shmem_team_split_strided
  c11_shmem_team_sync
  c11_shmem_team_translate_pe
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/teams
)

# --- Include directories for each target
target_include_directories(c11_shmem_team_destroy PRIVATE src/include)
target_include_directories(c11_shmem_team_get_config PRIVATE src/include)
target_include_directories(c11_shmem_team_my_pe PRIVATE src/include)
target_include_directories(c11_shmem_team_n_pes PRIVATE src/include)
target_include_directories(c11_shmem_team_split_2d PRIVATE src/include)
target_include_directories(c11_shmem_team_split_strided PRIVATE src/include)
target_include_directories(c11_shmem_team_sync PRIVATE src/include)
target_include_directories(c11_shmem_team_translate_pe PRIVATE src/include)

# --- Link any necessary libraries to each target
target_link_libraries(c11_shmem_team_destroy)
target_link_libraries(c11_shmem_team_get_config)
target_link_libraries(c11_shmem_team_my_pe)
target_link_libraries(c11_shmem_team_n_pes)
target_link_libraries(c11_shmem_team_split_2d)
target_link_libraries(c11_shmem_team_split_strided)
target_link_libraries(c11_shmem_team_sync)
target_link_libraries(c11_shmem_team_translate_pe)
