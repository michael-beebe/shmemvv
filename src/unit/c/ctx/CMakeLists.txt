#####################################
# src/unit/c/ctx/CMakeLists.txt
#####################################

# --- Add the common source files
set(SOURCES
  ../../../shmemvv.c
)

# --- Add the executables
add_executable(c_shmem_ctx_create_destroy c_shmem_ctx_create_destroy.c ${SOURCES})
add_executable(c_shmem_ctx_get_team c_shmem_ctx_get_team.c ${SOURCES})
add_executable(c_shmem_team_create_ctx c_shmem_team_create_ctx.c ${SOURCES})

# --- Set the output directory for the executables
set_target_properties(
  c_shmem_ctx_create_destroy
  c_shmem_ctx_get_team
  c_shmem_team_create_ctx

  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/ctx
)

# --- Include directories for each target
target_include_directories(c_shmem_ctx_create_destroy PRIVATE src/include)
target_include_directories(c_shmem_ctx_get_team PRIVATE src/include)
target_include_directories(c_shmem_team_create_ctx PRIVATE src/include)

# --- Link any necessary libraries to each target
target_link_libraries(c_shmem_ctx_create_destroy)
target_link_libraries(c_shmem_ctx_get_team)
target_link_libraries(c_shmem_team_create_ctx)
