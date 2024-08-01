#####################################
# src/tests/collectives/CMakeLists.txt
#####################################

# ---- Add the source files for collective operation tests
add_library(cxx_collectives_tests cxx_collectives_tests.cpp cxx_collectives_tests.h)

# ---- Link the libraries with the additional linker flags
target_link_libraries(cxx_collectives_tests ${CMAKE_C_LINKER_FLAGS})
