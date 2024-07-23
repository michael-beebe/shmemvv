/**
  @file threads_tests.cpp
  @brief Contains OpenSHMEM threads tests.
 */

#include "threads_tests.hpp"

/**
  @brief Tests the initialization of OpenSHMEM with threading support.
  @return True if the initialization with threading support is successful, false otherwise.
 */
bool test_shmem_init_thread(void) {
  int provided;
  p_shmem_init_thread(SHMEM_THREAD_MULTIPLE, &provided);
  return (provided == SHMEM_THREAD_MULTIPLE);
}

/**
  @brief Tests querying the level of threading support in OpenSHMEM.
  @return True if the queried threading level is at least SHMEM_THREAD_SINGLE, false otherwise.
 */
bool test_shmem_query_thread(void) {
  int provided;

  /* Query the level of threading support */
  p_shmem_query_thread(&provided);

  /* Check if the provided threading level is one of the valid levels */
  bool success = (provided == SHMEM_THREAD_SINGLE ||
                  provided == SHMEM_THREAD_FUNNELED ||
                  provided == SHMEM_THREAD_SERIALIZED ||
                  provided == SHMEM_THREAD_MULTIPLE);

  return success;
}

