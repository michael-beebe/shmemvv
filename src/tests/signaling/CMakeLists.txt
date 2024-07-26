# ---- Add the source files for signaling operation tests
add_library(cxx_signaling_tests cxx_signaling_tests.cpp cxx_signaling_tests.h)

# ---- Link the libraries with the additional linker flags
target_link_libraries(cxx_signaling_tests ${CMAKE_C_LINKER_FLAGS})
