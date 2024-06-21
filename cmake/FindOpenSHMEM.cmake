# ---- Find the OpenSHMEM compiler wrapper
find_program(OSHCC_EXECUTABLE oshcc)

if (OSHCC_EXECUTABLE)
  message(STATUS "Found oshcc: ${OSHCC_EXECUTABLE}")

  # ---- Determine the OpenSHMEM installation path from oshcc
  execute_process(
    COMMAND ${OSHCC_EXECUTABLE} -show
    OUTPUT_VARIABLE OSHCC_OUTPUT
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  string(REGEX MATCHALL "[^ ]+" OSHCC_OUTPUT_LIST ${OSHCC_OUTPUT})

  # ---- Initialize variables to store paths
  set(OPENSHMEM_INCLUDE_DIR "")
  set(OPENSHMEM_LIBRARY_DIR "")

  # ---- Extract the include and library paths
  foreach (ITEM IN LISTS OSHCC_OUTPUT_LIST)
    message(STATUS "Processing item: ${ITEM}") # Debug message
    if (ITEM MATCHES "^-I.*")
      string(SUBSTRING ${ITEM} 2 -1 OPENSHMEM_INCLUDE_DIR)
      message(STATUS "Found include directory: ${OPENSHMEM_INCLUDE_DIR}") # Debug message
    elseif (ITEM MATCHES "^-L.*")
      string(SUBSTRING ${ITEM} 2 -1 OPENSHMEM_LIBRARY_DIR)
      message(STATUS "Found library directory: ${OPENSHMEM_LIBRARY_DIR}") # Debug message
    elseif (ITEM MATCHES "-lsma")
      set(OPENSHMEM_LIBRARY "${OPENSHMEM_LIBRARY_DIR}/libshmem.so")
      message(STATUS "Found library: ${OPENSHMEM_LIBRARY}") # Debug message
    endif ()
  endforeach ()

  if (OPENSHMEM_INCLUDE_DIR AND OPENSHMEM_LIBRARY)
    set(OpenSHMEM_FOUND TRUE)
    set(OpenSHMEM_INCLUDE_DIRS ${OPENSHMEM_INCLUDE_DIR})
    set(OpenSHMEM_LIBRARIES ${OPENSHMEM_LIBRARY})
  else()
    set(OpenSHMEM_FOUND FALSE)
  endif ()

else ()
  set(OpenSHMEM_FOUND FALSE)
endif ()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OpenSHMEM DEFAULT_MSG OpenSHMEM_LIBRARIES OpenSHMEM_INCLUDE_DIRS)

if (NOT OpenSHMEM_FOUND)
  message(WARNING "Could not find OpenSHMEM. Please specify the paths manually in your cmake command.\n
  Example: -DOPENSHMEM_INCLUDE_DIR=/path/to/openshmem/include\n
  \t-DOPENSHMEM_LIBRARY=/path/to/openshmem/lib/libshmem.so")
else ()
  message(STATUS "OpenSHMEM include directory: ${OpenSHMEM_INCLUDE_DIRS}")
  message(STATUS "OpenSHMEM library: ${OpenSHMEM_LIBRARIES}")
endif ()

