#####################################
# src/tests/remote/CMakeLists.txt
#####################################

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# ---- Add the source files for remote memory access tests
add_library(cxx_remote_tests cxx_remote_tests.cpp cxx_remote_tests.h)
add_library(c11_remote_tests c11_remote_tests.c c11_remote_tests.h)

# ---- Link the libraries with the additional linker flags
target_link_libraries(cxx_remote_tests ${CMAKE_C_LINKER_FLAGS})
target_link_libraries(c11_remote_tests ${CMAKE_C_LINKER_FLAGS})
