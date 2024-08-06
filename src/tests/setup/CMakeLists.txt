#####################################
# src/tests/setup/CMakeLists.txt
#####################################

# ---- Add the source files for setup tests
add_library(setup_tests setup_tests.cpp setup_tests.h)

# ---- Link the libraries with the additional linker flags
target_link_libraries(setup_tests ${CMAKE_C_LINKER_FLAGS})