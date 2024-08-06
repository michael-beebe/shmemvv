#####################################
# src/tests/signaling/CMakeLists.txt
#####################################

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# ---- Add the source files for signaling operation tests
add_library(cxx_signaling_tests cxx_signaling_tests.cpp cxx_signaling_tests.h)
add_library(c11_signaling_tests c11_signaling_tests.c c11_signaling_tests.h)

# ---- Link the libraries with the additional linker flags
target_link_libraries(cxx_signaling_tests ${CMAKE_C_LINKER_FLAGS})
target_link_libraries(c11_signaling_tests ${CMAKE_C_LINKER_FLAGS})

