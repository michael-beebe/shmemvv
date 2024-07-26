# ---- Add the source files for distributed locking tests
add_library(cxx_locking_tests cxx_locking_tests.cpp cxx_locking_tests.h)

# ---- Link the libraries with the additional linker flags
target_link_libraries(cxx_locking_tests ${CMAKE_C_LINKER_FLAGS})
