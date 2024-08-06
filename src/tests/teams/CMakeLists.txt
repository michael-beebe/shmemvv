#####################################
# src/tests/teams/CMakeLists.txt
#####################################

# ---- Add the source files for team tests
add_library(teams_tests teams_tests.cpp teams_tests.h)

# ---- Link the libraries with the additional linker flags
target_link_libraries(teams_tests ${CMAKE_C_LINKER_FLAGS})
