# ---- Specify C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# ---- Add include directories
include_directories(${CMAKE_INCLUDE_PATH})

# ---- Add library directories
link_directories(${CMAKE_LIBRARY_PATH})

# ---- Add the main executable
add_executable(shmemvv main.cpp shmemvv.cpp)

# ---- Link the libraries with the additional linker flags
target_link_libraries(shmemvv ${CMAKE_C_LINKER_FLAGS})

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
