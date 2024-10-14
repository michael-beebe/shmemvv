#####################################
# src/unit/c/ctx/CMakeLists.txt
#####################################

# --- Add the common source files
set(SOURCES
  ../../../shmemvv.c
)

# --- Add the executables
add_executable(cxx_shmem_ctx_create_destroy cxx_shmem_ctx_create_destroy.cpp ${SOURCES})
add_executable(cxx_shmem_ctx_get_team cxx_shmem_ctx_get_team.cpp ${SOURCES})
add_executable(cxx_shmem_team_create_ctx cxx_shmem_team_create_ctx.cpp ${SOURCES})

# --- Set the output directory for the executables
set_target_properties(
  cxx_shmem_ctx_create_destroy
  cxx_shmem_ctx_get_team
  cxx_shmem_team_create_ctx

  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/ctx
)

# --- Include directories for each target
target_include_directories(cxx_shmem_ctx_create_destroy PRIVATE src/include)
target_include_directories(cxx_shmem_ctx_get_team PRIVATE src/include)
target_include_directories(cxx_shmem_team_create_ctx PRIVATE src/include)

# --- Link any necessary libraries to each target
target_link_libraries(cxx_shmem_ctx_create_destroy)
target_link_libraries(cxx_shmem_ctx_get_team)
target_link_libraries(cxx_shmem_team_create_ctx)
