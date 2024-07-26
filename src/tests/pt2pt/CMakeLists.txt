# ---- Add the source files for point-to-point synchronization tests
add_library(cxx_pt2pt_tests cxx_pt2pt_tests.cpp cxx_pt2pt_tests.h)

# ---- Link the libraries with the additional linker flags
target_link_libraries(cxx_pt2pt_tests ${CMAKE_C_LINKER_FLAGS})
