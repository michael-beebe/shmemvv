# ---- Add the source files for communication tests
add_library(cxx_comms_tests cxx_comms_tests.cpp cxx_comms_tests.h)

# ---- Link the libraries with the additional linker flags
target_link_libraries(cxx_comms_tests ${CMAKE_C_LINKER_FLAGS})
