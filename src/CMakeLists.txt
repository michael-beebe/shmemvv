# ---- Specify C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# ---- Add include directories
include_directories(${CMAKE_SOURCE_DIR}/src/include)

# ---- Add library directories
link_directories(${CMAKE_LIBRARY_PATH})

# ---- Collect source files
set(SOURCES
  main.cpp
  shmemvv.cpp
  routines.cpp
)

# ---- Add the main executable
add_executable(shmemvv ${SOURCES})

# ---- Link the libraries with the additional linker flags
target_link_libraries(
  shmemvv ${CMAKE_C_LINKER_FLAGS}
  
  setup_tests

  cxx_threads_tests
  cxx_mem_tests
  cxx_teams_tests
  cxx_comms_tests
  cxx_remote_tests
  cxx_atomics_tests
  cxx_signaling_tests
  cxx_collectives_tests
  cxx_pt2pt_tests
  cxx_mem_ordering_tests
  cxx_locking_tests
)

# ---- Specify the output directory for the executable
set_target_properties(shmemvv PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# ---- Add the tests directory
add_subdirectory(tests)

# ---- Install the main executable
install(TARGETS shmemvv
  RUNTIME DESTINATION bin
)
