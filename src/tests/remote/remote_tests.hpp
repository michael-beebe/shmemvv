/**
  @file remote_tests.hpp
  @brief Contains function declarations for the OpenSHMEM remote memory access tests.
 */

#ifndef REMOTE_TESTS_HPP
#define REMOTE_TESTS_HPP

#include <shmem.h>
#include <iostream>

/**
  @brief Tests the shmem_put() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_put(void);

/**
  @brief Tests the shmem_p() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_p(void);

/**
  @brief Tests the shmem_iput() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_iput(void);

/**
  @brief Tests the shmem_get() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_get(void);

/**
  @brief Tests the shmem_g() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_g(void);

/**
  @brief Tests the shmem_iget() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_iget(void);

/**
  @brief Tests the shmem_put_nbi() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_put_nbi(void);

/**
  @brief Tests the shmem_get_nbi() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_get_nbi(void);

#endif /* REMOTE_TESTS_HPP */
