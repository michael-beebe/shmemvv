# ---- Add the source files for atomic memory operation tests
add_library(cxx_atomics_tests cxx_atomics_tests.cpp cxx_atomics_tests.h)

# ---- Link the libraries with the additional linker flags
target_link_libraries(cxx_atomics_tests ${CMAKE_C_LINKER_FLAGS})
