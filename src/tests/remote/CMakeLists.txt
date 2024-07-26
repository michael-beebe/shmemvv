# ---- Add the source files for remote memory access tests
add_library(cxx_remote_tests cxx_remote_tests.cpp cxx_remote_tests.h)

# ---- Link the libraries with the additional linker flags
target_link_libraries(cxx_remote_tests ${CMAKE_C_LINKER_FLAGS})
