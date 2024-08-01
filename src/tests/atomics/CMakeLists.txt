#####################################
# src/tests/atomics/CMakeLists.txt
#####################################

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# ---- Add the source files for atomic memory operation tests
add_library(cxx_atomics_tests cxx_atomics_tests.cpp cxx_atomics_tests.h)
add_library(c11_atomics_tests c11_atomics_tests.c c11_atomics_tests.h)

# ---- Link the libraries with the additional linker flags
target_link_libraries(cxx_atomics_tests ${CMAKE_CXX_LINKER_FLAGS})
target_link_libraries(c11_atomics_tests ${CMAKE_C_LINKER_FLAGS})
