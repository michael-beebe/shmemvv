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
bool test_shmem_init_thread(void) {
  int provided;
  shmem_init_thread(SHMEM_THREAD_MULTIPLE, &provided);
  return (provided == SHMEM_THREAD_MULTIPLE);
}

/**
 * @brief Tests querying the level of threading support in OpenSHMEM.
 *
 * This test verifies that the `shmem_query_thread` function
 * correctly queries the level of threading support provided by OpenSHMEM.
 *
 * @return True if the query is successful and the level of threading support is one of the valid levels, false otherwise.
 */
bool test_shmem_query_thread(void) {
  int provided;
  shmem_query_thread(&provided);
  bool success = (provided == SHMEM_THREAD_SINGLE ||
                  provided == SHMEM_THREAD_FUNNELED ||
                  provided == SHMEM_THREAD_SERIALIZED ||
                  provided == SHMEM_THREAD_MULTIPLE);
  return success;
}

/**
 * TODO: write docs
 * 
 */
void run_threads_tests(int mype, int npes) {
  /* If we made it here shmem_init_thread() passed */
  if (mype == 0) {
    display_test_result("shmem_init_thread()", true, true);
  }

  /* Test shmem_query_thread() */
  shmem_barrier_all();
  if ( !check_if_exists("shmem_query_thread") ) {
    if (mype == 0) {
      display_not_found_warning("shmem_query_thread()", false);
    }
  }
  else {
    bool result_shmem_query_thread = test_shmem_query_thread();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("shmem_query_thread()", result_shmem_query_thread, false);
    }
  }
}
