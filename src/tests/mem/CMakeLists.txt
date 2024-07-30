# ---- Add the source files for memory tests
add_library(mem_tests mem_tests.cpp mem_tests.h)

# ---- Link the libraries with the additional linker flags
target_link_libraries(mem_tests ${CMAKE_C_LINKER_FLAGS})
