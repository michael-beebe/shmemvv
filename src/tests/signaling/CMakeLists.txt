# ---- Add the source files for signaling operation tests
add_library(signaling_tests signaling_tests.cpp signaling_tests.hpp)

# ---- Link the libraries with the additional linker flags
target_link_libraries(signaling_tests ${CMAKE_C_LINKER_FLAGS})
