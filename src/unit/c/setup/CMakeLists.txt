#####################################
# src/unit/c/setup/CMakeLists.txt
#####################################

# --- Add the common source files
set(SOURCES
  ../../../shmemvv.c
  ../../../log.c
)

# --- Add the executables
add_executable(c_shmem_info_get_name c_shmem_info_get_name.c ${SOURCES})
add_executable(c_shmem_info_get_version c_shmem_info_get_version.c ${SOURCES})
add_executable(c_shmem_my_pe c_shmem_my_pe.c ${SOURCES})
add_executable(c_shmem_n_pes c_shmem_n_pes.c ${SOURCES})
add_executable(c_shmem_pe_accessible c_shmem_pe_accessible.c ${SOURCES})

# --- Set the output directory for the executables
set_target_properties(
  c_shmem_info_get_name
  c_shmem_info_get_version
  c_shmem_my_pe
  c_shmem_n_pes
  c_shmem_pe_accessible
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/setup
)

# --- Include directories for each target
target_include_directories(c_shmem_info_get_name PRIVATE src/include)
target_include_directories(c_shmem_info_get_version PRIVATE src/include)
target_include_directories(c_shmem_my_pe PRIVATE src/include)
target_include_directories(c_shmem_n_pes PRIVATE src/include)
target_include_directories(c_shmem_pe_accessible PRIVATE src/include)
