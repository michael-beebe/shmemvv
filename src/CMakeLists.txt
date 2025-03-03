#####################################
# src/CMakeLists.txt
#####################################

# ---- Add include directories
include_directories(${CMAKE_SOURCE_DIR}/src/include)

# ---- Add library directories
link_directories(${CMAKE_LIBRARY_PATH})

# ---- Add subdirs
add_subdirectory(unit)

