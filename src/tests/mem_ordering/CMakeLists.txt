# ---- Add the source files for memory ordering tests
add_library(cxx_mem_ordering_tests cxx_mem_ordering_tests.cpp cxx_mem_ordering_tests.h)

# ---- Link the libraries with the additional linker flags
target_link_libraries(cxx_mem_ordering_tests ${CMAKE_C_LINKER_FLAGS})
