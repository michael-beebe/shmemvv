# ---- Add the source files for thread tests
add_library(cxx_threads_tests cxx_threads_tests.cpp cxx_threads_tests.h)

# ---- Link the libraries with the additional linker flags
target_link_libraries(cxx_threads_tests ${CMAKE_C_LINKER_FLAGS})
