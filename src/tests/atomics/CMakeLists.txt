# ---- Add the source files for atomic memory operation tests
add_library(atomics_tests atomics_tests.cpp atomics_tests.h)

# ---- Link the libraries with the additional linker flags
target_link_libraries(atomics_tests ${CMAKE_C_LINKER_FLAGS})
