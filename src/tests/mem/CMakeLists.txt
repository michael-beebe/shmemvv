# ---- Add the source files for memory tests
add_library(cxx_mem_tests cxx_mem_tests.cpp cxx_mem_tests.h)

# ---- Link the libraries with the additional linker flags
target_link_libraries(cxx_mem_tests ${CMAKE_C_LINKER_FLAGS})
