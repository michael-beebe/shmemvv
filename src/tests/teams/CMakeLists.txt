# ---- Add the source files for team tests
add_library(cxx_teams_tests cxx_teams_tests.cpp cxx_teams_tests.h)

# ---- Link the libraries with the additional linker flags
target_link_libraries(cxx_teams_tests ${CMAKE_C_LINKER_FLAGS})
