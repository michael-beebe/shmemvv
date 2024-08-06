#####################################
# src/tests/locking/CMakeLists.txt
#####################################

# ---- Add the source files for distributed locking tests
add_library(locking_tests locking_tests.cpp locking_tests.h)

# ---- Link the libraries with the additional linker flags
target_link_libraries(locking_tests ${CMAKE_C_LINKER_FLAGS})
