#####################################
# src/tests/pt2pt/CMakeLists.txt
#####################################
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# ---- Add the source files for point-to-point synchronization tests
add_library(cxx_pt2pt_tests cxx_pt2pt_tests.cpp cxx_pt2pt_tests.h)
add_library(c11_pt2pt_tests c11_pt2pt_tests.c c11_pt2pt_tests.h)

# ---- Link the libraries with the additional linker flags
target_link_libraries(cxx_pt2pt_tests ${CMAKE_C_LINKER_FLAGS})
target_link_libraries(c11_pt2pt_tests ${CMAKE_C_LINKER_FLAGS})
