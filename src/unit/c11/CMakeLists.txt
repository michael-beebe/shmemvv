#####################################
# src/unit/c11/CMakeLists.txt
#####################################

# --- Specify C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# --- Define options to disable specific C11 test categories
# All test categories are built by default unless explicitly disabled
option(DISABLE_C11_ATOMICS "Disable C11 atomic memory operation tests" OFF)
option(DISABLE_C11_COLLECTIVES "Disable C11 collective operation tests" OFF)
option(DISABLE_C11_RMA "Disable C11 remote memory access tests" OFF)
option(DISABLE_C11_PT2PT_SYNC "Disable C11 point-to-point synchronization tests" OFF)
option(DISABLE_C11_SIGNALING "Disable C11 signaling operation tests" OFF)

# --- Skip all C11 tests if DISABLE_ALL_C11_TESTS is ON
if(NOT DISABLE_ALL_C11_TESTS)

  # --- Conditionally add subdirectories based on disable options
  if(NOT DISABLE_C11_ATOMICS)
    add_subdirectory(atomics)
  endif()

  if(NOT DISABLE_C11_COLLECTIVES)
    add_subdirectory(collectives)
  endif()

  if(NOT DISABLE_C11_RMA)
    add_subdirectory(rma)
  endif()

  if(NOT DISABLE_C11_PT2PT_SYNC)
    add_subdirectory(pt2pt_sync)
  endif()

  if(NOT DISABLE_C11_SIGNALING)
    add_subdirectory(signaling)
  endif()

endif()
