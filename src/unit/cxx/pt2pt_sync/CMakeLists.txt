#####################################
# src/unit/c/pt2pt_sync/CMakeLists.txt
#####################################

# --- Add the common source files
set(SOURCES
  ../../../shmemvv.c
  ../../../log.c
)

# --- Add the executables
add_executable(cxx_shmem_signal_wait_until cxx_shmem_signal_wait_until.cpp ${SOURCES})
add_executable(cxx_shmem_test_all_vector cxx_shmem_test_all_vector.cpp ${SOURCES})
add_executable(cxx_shmem_test_all cxx_shmem_test_all.cpp ${SOURCES})
add_executable(cxx_shmem_test_any_vector cxx_shmem_test_any_vector.cpp ${SOURCES})
add_executable(cxx_shmem_test_any cxx_shmem_test_any.cpp ${SOURCES})
add_executable(cxx_shmem_test_some_vector cxx_shmem_test_some_vector.cpp ${SOURCES})
add_executable(cxx_shmem_test_some cxx_shmem_test_some.cpp ${SOURCES})
add_executable(cxx_shmem_test cxx_shmem_test.cpp ${SOURCES})
add_executable(cxx_shmem_wait_until_all_vector cxx_shmem_wait_until_all_vector.cpp ${SOURCES})
add_executable(cxx_shmem_wait_until_all cxx_shmem_wait_until_all.cpp ${SOURCES})
add_executable(cxx_shmem_wait_until_any cxx_shmem_wait_until_any.cpp ${SOURCES})
add_executable(cxx_shmem_wait_until_any_vector cxx_shmem_wait_until_any_vector.cpp ${SOURCES})
add_executable(cxx_shmem_wait_until_some cxx_shmem_wait_until_some.cpp ${SOURCES})
add_executable(cxx_shmem_wait_until_some_vector cxx_shmem_wait_until_some_vector.cpp ${SOURCES})
add_executable(cxx_shmem_wait_until cxx_shmem_wait_until.cpp ${SOURCES})

# --- Set the output directory for the executables
set_target_properties(
  cxx_shmem_signal_wait_until
  cxx_shmem_test_all_vector
  cxx_shmem_test_all
  cxx_shmem_test_any_vector
  cxx_shmem_test_any
  cxx_shmem_test_some_vector
  cxx_shmem_test
  cxx_shmem_wait_until_all_vector
  cxx_shmem_wait_until_all
  cxx_shmem_wait_until_any_vector
  cxx_shmem_wait_until_any
  cxx_shmem_wait_until_some_vector
  cxx_shmem_wait_until_some
  cxx_shmem_wait_until

  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/pt2pt_sync
)

# --- Include directories for each target
target_include_directories(cxx_shmem_signal_wait_until PRIVATE src/include)
target_include_directories(cxx_shmem_test_all_vector PRIVATE src/include)
target_include_directories(cxx_shmem_test_all PRIVATE src/include)
target_include_directories(cxx_shmem_test_any_vector PRIVATE src/include)
target_include_directories(cxx_shmem_test_any PRIVATE src/include)
target_include_directories(cxx_shmem_test_some_vector PRIVATE src/include)
target_include_directories(cxx_shmem_test PRIVATE src/include)
target_include_directories(cxx_shmem_wait_until_all_vector PRIVATE src/include)
target_include_directories(cxx_shmem_wait_until_all PRIVATE src/include)
target_include_directories(cxx_shmem_wait_until_any_vector PRIVATE src/include)
target_include_directories(cxx_shmem_wait_until_any PRIVATE src/include)
target_include_directories(cxx_shmem_wait_until_some_vector PRIVATE src/include)
target_include_directories(cxx_shmem_wait_until_some PRIVATE src/include)
target_include_directories(cxx_shmem_wait_until PRIVATE src/include)

# --- Link any necessary libraries to each target
target_link_libraries(cxx_shmem_signal_wait_until)
target_link_libraries(cxx_shmem_test_all_vector)
target_link_libraries(cxx_shmem_test_all)
target_link_libraries(cxx_shmem_test_any_vector)
target_link_libraries(cxx_shmem_test_any)
target_link_libraries(cxx_shmem_test_some_vector)
target_link_libraries(cxx_shmem_test)
target_link_libraries(cxx_shmem_wait_until_all_vector)
target_link_libraries(cxx_shmem_wait_until_all)
target_link_libraries(cxx_shmem_wait_until_any_vector)
target_link_libraries(cxx_shmem_wait_until_any)
target_link_libraries(cxx_shmem_wait_until_some_vector)
target_link_libraries(cxx_shmem_wait_until_some)
target_link_libraries(cxx_shmem_wait_until)

