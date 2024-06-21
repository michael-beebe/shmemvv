# ---- Add the source files for point-to-point synchronization tests
add_library(pt2pt_tests pt2pt_tests.cpp pt2pt_tests.hpp)

# ---- Link the libraries with the additional linker flags
target_link_libraries(pt2pt_tests ${CMAKE_C_LINKER_FLAGS})
