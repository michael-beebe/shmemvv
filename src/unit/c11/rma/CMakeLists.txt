#####################################
# src/unit/c/rma/CMakeLists.txt
#####################################

# --- Add the common source files
set(SOURCES
    ../../../shmemvv.c
)

# --- Add the executables
add_executable(c11_shmem_put c11_shmem_put.c ${SOURCES})
add_executable(c11_shmem_p c11_shmem_p.c ${SOURCES})
add_executable(c11_shmem_get c11_shmem_get.c ${SOURCES})
add_executable(c11_shmem_g c11_shmem_g.c ${SOURCES})
add_executable(c11_shmem_iput c11_shmem_iput.c ${SOURCES})
add_executable(c11_shmem_iget c11_shmem_iget.c ${SOURCES})
add_executable(c11_shmem_put_nbi c11_shmem_put_nbi.c ${SOURCES})
add_executable(c11_shmem_get_nbi c11_shmem_get_nbi.c ${SOURCES})

# --- Set the output directory for the executables
set_target_properties(
  c11_shmem_put
  c11_shmem_p
  c11_shmem_get
  c11_shmem_g
  c11_shmem_iput
  c11_shmem_iget
  c11_shmem_put_nbi
  c11_shmem_get_nbi
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/rma
)

# --- Include directories for each target
target_include_directories(c11_shmem_put PRIVATE src/include)
target_include_directories(c11_shmem_p PRIVATE src/include)
target_include_directories(c11_shmem_get PRIVATE src/include)
target_include_directories(c11_shmem_g PRIVATE src/include)
target_include_directories(c11_shmem_iput PRIVATE src/include)
target_include_directories(c11_shmem_iget PRIVATE src/include)
target_include_directories(c11_shmem_put_nbi PRIVATE src/include)
target_include_directories(c11_shmem_get_nbi PRIVATE src/include)

# --- Link any necessary libraries to each target
target_link_libraries(c11_shmem_put)
target_link_libraries(c11_shmem_p)
target_link_libraries(c11_shmem_get)
target_link_libraries(c11_shmem_g)
target_link_libraries(c11_shmem_iput)
target_link_libraries(c11_shmem_iget)
target_link_libraries(c11_shmem_put_nbi)
target_link_libraries(c11_shmem_get_nbi)
