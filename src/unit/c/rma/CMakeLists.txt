#####################################
# src/unit/c/rma/CMakeLists.txt
#####################################

# --- Add the common source files
set(SOURCES
    ../../../shmemvv.c
    ../../../log.c
)

# --- Add the executables
add_executable(c_shmem_put c_shmem_put.c ${SOURCES})
add_executable(c_shmem_p c_shmem_p.c ${SOURCES})
add_executable(c_shmem_get c_shmem_get.c ${SOURCES})
add_executable(c_shmem_g c_shmem_g.c ${SOURCES})
add_executable(c_shmem_iput c_shmem_iput.c ${SOURCES})
add_executable(c_shmem_iget c_shmem_iget.c ${SOURCES})
add_executable(c_shmem_put_nbi c_shmem_put_nbi.c ${SOURCES})
add_executable(c_shmem_get_nbi c_shmem_get_nbi.c ${SOURCES})
if(SHMEMVV_HAVE_SESSIONS)
  add_executable(c_shmem_sessions c_shmem_sessions.c ${SOURCES})
  set_target_properties(c_shmem_sessions PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/rma)
endif()

# --- Set the output directory for the executables
set_target_properties(
  c_shmem_put
  c_shmem_p
  c_shmem_get
  c_shmem_g
  c_shmem_iput
  c_shmem_iget
  c_shmem_put_nbi
  c_shmem_get_nbi
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/rma
)

# --- Include directories for each target
target_include_directories(c_shmem_put PRIVATE src/include)
target_include_directories(c_shmem_p PRIVATE src/include)
target_include_directories(c_shmem_get PRIVATE src/include)
target_include_directories(c_shmem_g PRIVATE src/include)
target_include_directories(c_shmem_iput PRIVATE src/include)
target_include_directories(c_shmem_iget PRIVATE src/include)
target_include_directories(c_shmem_put_nbi PRIVATE src/include)
target_include_directories(c_shmem_get_nbi PRIVATE src/include)

# --- Link any necessary libraries to each target
target_link_libraries(c_shmem_put)
target_link_libraries(c_shmem_p)
target_link_libraries(c_shmem_get)
target_link_libraries(c_shmem_g)
target_link_libraries(c_shmem_iput)
target_link_libraries(c_shmem_iget)
target_link_libraries(c_shmem_put_nbi)
target_link_libraries(c_shmem_get_nbi)

# --- OpenSHMEM 1.6 coverage
if(SHMEMVV_HAVE_IBPUT)
  add_executable(c_shmem_ibput c_shmem_ibput.c ${SOURCES})
  set_target_properties(c_shmem_ibput PROPERTIES
    C_STANDARD 11 C_STANDARD_REQUIRED YES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/rma)
endif()
if(SHMEMVV_HAVE_PSHMEM_IBPUT)
  add_executable(c_pshmem_ibput c_shmem_ibput.c ${SOURCES})
  target_compile_definitions(c_pshmem_ibput PRIVATE API_USE_PSHMEM)
  set_target_properties(c_pshmem_ibput PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/rma)
endif()
if(SHMEMVV_HAVE_IBGET)
  add_executable(c_shmem_ibget c_shmem_ibget.c ${SOURCES})
  set_target_properties(c_shmem_ibget PROPERTIES
    C_STANDARD 11 C_STANDARD_REQUIRED YES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/rma)
endif()
if(SHMEMVV_HAVE_PSHMEM_IBGET)
  add_executable(c_pshmem_ibget c_shmem_ibget.c ${SOURCES})
  target_compile_definitions(c_pshmem_ibget PRIVATE API_USE_PSHMEM)
  set_target_properties(c_pshmem_ibget PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/rma)
endif()
