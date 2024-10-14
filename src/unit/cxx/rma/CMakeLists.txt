#####################################
# src/unit/c/rma/CMakeLists.txt
#####################################

# --- Add the common source files
set(SOURCES
    ../../../shmemvv.c
)

# --- Add the executables
add_executable(cxx_shmem_put cxx_shmem_put.cpp ${SOURCES})
add_executable(cxx_shmem_p cxx_shmem_p.cpp ${SOURCES})
add_executable(cxx_shmem_get cxx_shmem_get.cpp ${SOURCES})
add_executable(cxx_shmem_g cxx_shmem_g.cpp ${SOURCES})
add_executable(cxx_shmem_iput cxx_shmem_iput.cpp ${SOURCES})
add_executable(cxx_shmem_iget cxx_shmem_iget.cpp ${SOURCES})
add_executable(cxx_shmem_put_nbi cxx_shmem_put_nbi.cpp ${SOURCES})
add_executable(cxx_shmem_get_nbi cxx_shmem_get_nbi.cpp ${SOURCES})

# --- Set the output directory for the executables
set_target_properties(
  cxx_shmem_put
  cxx_shmem_p
  cxx_shmem_get
  cxx_shmem_g
  cxx_shmem_iput
  cxx_shmem_iget
  cxx_shmem_put_nbi
  cxx_shmem_get_nbi
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/rma
)

# --- Include directories for each target
target_include_directories(cxx_shmem_put PRIVATE src/include)
target_include_directories(cxx_shmem_p PRIVATE src/include)
target_include_directories(cxx_shmem_get PRIVATE src/include)
target_include_directories(cxx_shmem_g PRIVATE src/include)
target_include_directories(cxx_shmem_iput PRIVATE src/include)
target_include_directories(cxx_shmem_iget PRIVATE src/include)
target_include_directories(cxx_shmem_put_nbi PRIVATE src/include)
target_include_directories(cxx_shmem_get_nbi PRIVATE src/include)

# --- Link any necessary libraries to each target
target_link_libraries(cxx_shmem_put)
target_link_libraries(cxx_shmem_p)
target_link_libraries(cxx_shmem_get)
target_link_libraries(cxx_shmem_g)
target_link_libraries(cxx_shmem_iput)
target_link_libraries(cxx_shmem_iget)
target_link_libraries(cxx_shmem_put_nbi)
target_link_libraries(cxx_shmem_get_nbi)
