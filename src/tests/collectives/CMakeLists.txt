# ---- Add the source files for collective operation tests
add_library(collectives_tests collectives_tests.cpp collectives_tests.h)

# ---- Link the libraries with the additional linker flags
target_link_libraries(collectives_tests ${CMAKE_C_LINKER_FLAGS})
