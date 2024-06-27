/**
  @file signaling_tests.hpp
  @brief Contains function declarations for the OpenSHMEM signaling tests.
 */

#ifndef SIGNALING_TESTS_HPP
#define SIGNALING_TESTS_HPP

#include <shmem.h>

/**
  @brief Tests the shmem_put_signal() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_put_signal(void);

/**
  @brief Tests the shmem_put_signal_nbi() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_put_signal_nbi(void);

/**
  @brief Tests the shmem_signal_fetch() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_signal_fetch(void);

#endif /* SIGNALING_TESTS_HPP */
