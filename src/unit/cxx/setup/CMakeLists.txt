#####################################
# src/unit/c/setup/CMakeLists.txt
#####################################

# --- Add the common source files
set(SOURCES
  ../../../shmemvv.c
  ../../../log.c
)

# --- Add the executables
add_executable(cxx_shmem_info_get_name cxx_shmem_info_get_name.cpp ${SOURCES})
add_executable(cxx_shmem_info_get_version cxx_shmem_info_get_version.cpp ${SOURCES})
add_executable(cxx_shmem_my_pe cxx_shmem_my_pe.cpp ${SOURCES})
add_executable(cxx_shmem_n_pes cxx_shmem_n_pes.cpp ${SOURCES})
add_executable(cxx_shmem_pe_accessible cxx_shmem_pe_accessible.cpp ${SOURCES})

# --- Set the output directory for the executables
set_target_properties(
  cxx_shmem_info_get_name
  cxx_shmem_info_get_version
  cxx_shmem_my_pe
  cxx_shmem_n_pes
  cxx_shmem_pe_accessible
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/setup
)

# --- Include directories for each target
target_include_directories(cxx_shmem_info_get_name PRIVATE src/include)
target_include_directories(cxx_shmem_info_get_version PRIVATE src/include)
target_include_directories(cxx_shmem_my_pe PRIVATE src/include)
target_include_directories(cxx_shmem_n_pes PRIVATE src/include)
target_include_directories(cxx_shmem_pe_accessible PRIVATE src/include)
