# ---- Add the source files for thread tests
add_library(threads_tests threads_tests.cpp threads_tests.hpp)

# ---- Link the libraries with the additional linker flags
target_link_libraries(threads_tests ${CMAKE_C_LINKER_FLAGS})
