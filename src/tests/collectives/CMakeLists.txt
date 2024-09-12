#####################################
# src/tests/collectives/CMakeLists.txt
#####################################\
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# ---- Add the source files for collective operation tests
add_library(cxx_collectives_tests cxx_collectives_tests.cpp cxx_collectives_tests.h)
#add_library(c11_collectives_tests c11_collectives_tests.c c11_collectives_tests.h)

# ---- Link the libraries with the additional linker flags
target_link_libraries(cxx_collectives_tests ${CMAKE_C_LINKER_FLAGS})
#target_link_libraries(c11_collectives_tests ${CMAKE_C_LINKER_FLAGS})
