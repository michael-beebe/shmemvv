#####################################
# src/tests/mem_ordering/CMakeLists.txt
#####################################

# ---- Add the source files for memory ordering tests
add_library(mem_ordering_tests mem_ordering_tests.cpp mem_ordering_tests.h)

# ---- Link the libraries with the additional linker flags
target_link_libraries(mem_ordering_tests ${CMAKE_C_LINKER_FLAGS})
