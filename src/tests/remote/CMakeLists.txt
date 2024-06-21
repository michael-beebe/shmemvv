# ---- Add the source files for remote memory access tests
add_library(remote_tests remote_tests.cpp remote_tests.hpp)

# ---- Link the libraries with the additional linker flags
target_link_libraries(remote_tests ${CMAKE_C_LINKER_FLAGS})
