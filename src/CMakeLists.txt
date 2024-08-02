#####################################
# src/CMakeLists.txt
#####################################

# ---- Specify C++ standard
# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- Specify C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# ---- Add include directories
include_directories(${CMAKE_SOURCE_DIR}/src/include)

# ---- Add library directories
link_directories(${CMAKE_LIBRARY_PATH})

# ---- Collect source files
set(SOURCES
  main.cpp
  shmemvv.cpp
)

# ---- Add the main executable
add_executable(shmemvv ${SOURCES})

# ---- Link the libraries with the additional linker flags
target_link_libraries(
  shmemvv ${CMAKE_C_LINKER_FLAGS}
  
  # --- Agnostic tests
  setup_tests
  threads_tests
  mem_tests
  comms_tests
  teams_tests
  mem_ordering_tests
  locking_tests

  # --- C/C++ tests
  cxx_remote_tests
  cxx_atomics_tests
  cxx_signaling_tests
  cxx_collectives_tests
  cxx_pt2pt_tests

  # --- C11 tests
  c11_remote_tests
  c11_atomics_tests
  c11_signaling_tests
  c11_collectives_tests
  c11_pt2pt_tests
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
