#####################################
# src/unit/c/CMakeLists.txt
#####################################

# --- Define options to disable specific C test categories
# All test categories are built by default unless explicitly disabled
option(DISABLE_C_ATOMICS "Disable C atomic memory operation tests" OFF)
option(DISABLE_C_COLLECTIVES "Disable C collective operation tests" OFF)
option(DISABLE_C_RMA "Disable C remote memory access tests" OFF)
option(DISABLE_C_CTX "Disable C context management tests" OFF)
option(DISABLE_C_LOCKING "Disable C distributed locking tests" OFF)
option(DISABLE_C_MEMORY "Disable C memory management tests" OFF)
option(DISABLE_C_PT2PT_SYNC "Disable C point-to-point synchronization tests" OFF)
option(DISABLE_C_SETUP "Disable C setup tests" OFF)
option(DISABLE_C_SIGNALING "Disable C signaling operation tests" OFF)
option(DISABLE_C_TEAMS "Disable C teams management tests" OFF)
option(DISABLE_C_THREADS "Disable C thread support tests" OFF)

# --- Skip all C tests if DISABLE_ALL_C_TESTS is ON
if(NOT DISABLE_ALL_C_TESTS)

  # --- Conditionally add subdirectories based on disable options
  if(NOT DISABLE_C_ATOMICS)
    add_subdirectory(atomics)
  endif()

  if(NOT DISABLE_C_COLLECTIVES)
    add_subdirectory(collectives)
  endif()

  if(NOT DISABLE_C_RMA)
    add_subdirectory(rma)
  endif()

  if(NOT DISABLE_C_CTX)
    add_subdirectory(ctx)
  endif()

  if(NOT DISABLE_C_LOCKING)
    add_subdirectory(locking)
  endif()

  if(NOT DISABLE_C_MEMORY)
    add_subdirectory(memory)
  endif()

  if(NOT DISABLE_C_PT2PT_SYNC)
    add_subdirectory(pt2pt_sync)
  endif()

  if(NOT DISABLE_C_SETUP)
    add_subdirectory(setup)
  endif()

  if(NOT DISABLE_C_SIGNALING)
    add_subdirectory(signaling)
  endif()

  if(NOT DISABLE_C_TEAMS)
    add_subdirectory(teams)
  endif()

  if(NOT DISABLE_C_THREADS)
    add_subdirectory(threads)
  endif()

endif()
