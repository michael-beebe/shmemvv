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
  shmem_init_thread(SHMEM_THREAD_MULTIPLE, &provided);
  return (provided == SHMEM_THREAD_MULTIPLE);
}

/**
  @brief Tests querying the level of threading support in OpenSHMEM.
  @return True if the queried threading level is SHMEM_THREAD_MULTIPLE, false otherwise.
 */
bool test_shmem_query_thread(void) {
  int provided;
  shmem_query_thread(&provided);
  // #ifdef _DEBUG_
  //   if (shmem_my_pe() == 0) {
  //     std::cout << "Querying " << &provided << std::endl;
  //   }
  // #endif
  return (provided == SHMEM_THREAD_MULTIPLE);
}
