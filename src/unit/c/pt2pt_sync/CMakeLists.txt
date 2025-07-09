#####################################
# src/unit/c/pt2pt_sync/CMakeLists.txt
#####################################

# --- Add the common source files
set(SOURCES
  ../../../shmemvv.c
  ../../../log.c
)

# --- Add the executables
add_executable(c_shmem_signal_wait_until c_shmem_signal_wait_until.c ${SOURCES})
add_executable(c_shmem_test_all_vector c_shmem_test_all_vector.c ${SOURCES})
add_executable(c_shmem_test_all c_shmem_test_all.c ${SOURCES})
add_executable(c_shmem_test_any_vector c_shmem_test_any_vector.c ${SOURCES})
add_executable(c_shmem_test_any c_shmem_test_any.c ${SOURCES})
add_executable(c_shmem_test_some_vector c_shmem_test_some_vector.c ${SOURCES})
add_executable(c_shmem_test_some c_shmem_test_some.c ${SOURCES})
add_executable(c_shmem_test c_shmem_test.c ${SOURCES})
add_executable(c_shmem_wait_until_all_vector c_shmem_wait_until_all_vector.c ${SOURCES})
add_executable(c_shmem_wait_until_all c_shmem_wait_until_all.c ${SOURCES})
add_executable(c_shmem_wait_until_any c_shmem_wait_until_any.c ${SOURCES})
add_executable(c_shmem_wait_until_any_vector c_shmem_wait_until_any_vector.c ${SOURCES})
add_executable(c_shmem_wait_until_some c_shmem_wait_until_some.c ${SOURCES})
add_executable(c_shmem_wait_until_some_vector c_shmem_wait_until_some_vector.c ${SOURCES})
add_executable(c_shmem_wait_until c_shmem_wait_until.c ${SOURCES})

# --- Set the output directory for the executables
set_target_properties(
  c_shmem_signal_wait_until
  c_shmem_test_all_vector
  c_shmem_test_all
  c_shmem_test_any_vector
  c_shmem_test_any
  c_shmem_test_some_vector
  c_shmem_test_some
  c_shmem_test
  c_shmem_wait_until_all_vector
  c_shmem_wait_until_all
  c_shmem_wait_until_any_vector
  c_shmem_wait_until_any
  c_shmem_wait_until_some_vector
  c_shmem_wait_until_some
  c_shmem_wait_until

  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/pt2pt_sync
)

# --- Include directories for each target
target_include_directories(c_shmem_signal_wait_until PRIVATE src/include)
target_include_directories(c_shmem_test_all_vector PRIVATE src/include)
target_include_directories(c_shmem_test_all PRIVATE src/include)
target_include_directories(c_shmem_test_any_vector PRIVATE src/include)
target_include_directories(c_shmem_test_any PRIVATE src/include)
target_include_directories(c_shmem_test_some_vector PRIVATE src/include)
target_include_directories(c_shmem_test_some PRIVATE src/include)
target_include_directories(c_shmem_test PRIVATE src/include)
target_include_directories(c_shmem_wait_until_all_vector PRIVATE src/include)
target_include_directories(c_shmem_wait_until_all PRIVATE src/include)
target_include_directories(c_shmem_wait_until_any_vector PRIVATE src/include)
target_include_directories(c_shmem_wait_until_any PRIVATE src/include)
target_include_directories(c_shmem_wait_until_some_vector PRIVATE src/include)
target_include_directories(c_shmem_wait_until_some PRIVATE src/include)
target_include_directories(c_shmem_wait_until PRIVATE src/include)

# --- Link any necessary libraries to each target
target_link_libraries(c_shmem_signal_wait_until)
target_link_libraries(c_shmem_test_all_vector)
target_link_libraries(c_shmem_test_all)
target_link_libraries(c_shmem_test_any_vector)
target_link_libraries(c_shmem_test_any)
target_link_libraries(c_shmem_test_some_vector)
target_link_libraries(c_shmem_test_some)
target_link_libraries(c_shmem_test)
target_link_libraries(c_shmem_wait_until_all_vector)
target_link_libraries(c_shmem_wait_until_all)
target_link_libraries(c_shmem_wait_until_any_vector)
target_link_libraries(c_shmem_wait_until_any)
target_link_libraries(c_shmem_wait_until_some_vector)
target_link_libraries(c_shmem_wait_until_some)
target_link_libraries(c_shmem_wait_until)

