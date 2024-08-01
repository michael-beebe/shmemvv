#####################################
# src/tests/comms/CMakeLists.txt
#####################################

# ---- Add the source files for communication tests
add_library(comms_tests comms_tests.cpp comms_tests.h)

# ---- Link the libraries with the additional linker flags
target_link_libraries(comms_tests ${CMAKE_C_LINKER_FLAGS})
