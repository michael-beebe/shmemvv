/**
 * @file cxx_threads_tests.cpp
 * @brief Contains OpenSHMEM threads tests.
 */

#include "threads_tests.h"

/**
 * @brief Tests the initialization of OpenSHMEM with threading support.
 *
 * This test verifies that OpenSHMEM can be initialized with the specified level of threading support.
 *
 * @return True if the initialization with threading support is successful, false otherwise.
 */
bool text_shmem_init_thread(void) {
  int provided;
  p_shmem_init_thread(SHMEM_THREAD_MULTIPLE, &provided);
  return (provided == SHMEM_THREAD_MULTIPLE);
}

/**
 * @brief Tests querying the level of threading support in OpenSHMEM.
 *
 * This test verifies that the `p_shmem_query_thread` function
 * correctly queries the level of threading support provided by OpenSHMEM.
 *
 * @return True if the query is successful and the level of threading support is one of the valid levels, false otherwise.
 */
bool text_shmem_query_thread(void) {
  int provided;
  p_shmem_query_thread(&provided);
  bool success = (provided == SHMEM_THREAD_SINGLE ||
                  provided == SHMEM_THREAD_FUNNELED ||
                  provided == SHMEM_THREAD_SERIALIZED ||
                  provided == SHMEM_THREAD_MULTIPLE);
  return success;
}
