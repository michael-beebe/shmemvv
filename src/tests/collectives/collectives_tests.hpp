/**
  @file collectives_tests.hpp
  @brief Contains function declarations for the OpenSHMEM collectives tests.
 */

#ifndef COLLECTIVES_TESTS_HPP
#define COLLECTIVES_TESTS_HPP

#include <shmem.h>
#include <cstring>
#include <stdbool.h>
#include <stdio.h>

/**
  @brief Tests the shmem_sync() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_sync(void);

/**
  @brief Tests the shmem_sync_all() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_sync_all(void);

/**
  @brief Tests the shmem_alltoall() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_alltoall(void);

/**
  @brief Tests the shmem_alltoalls() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_alltoalls(void);

/**
  @brief Tests the shmem_broadcast() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_broadcast(void);

/**
  @brief Tests the shmem_collect() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_collect(void);

/**
  @brief Tests the shmem_fcollect() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_fcollect(void);

/**
  @brief Tests the shmem_and_reduce() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_and_reduce(void);

// /**
//   @brief Tests the shmem_or_reduce() routine.
//   @return True if the test is successful, false otherwise.
//  */
// bool test_shmem_or_reduce(void);

// /**
//   @brief Tests the shmem_xor_reduce() routine.
//   @return True if the test is successful, false otherwise.
//  */
// bool test_shmem_xor_reduce(void);

/**
  @brief Tests the shmem_max_reduce() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_max_reduce(void);

/**
  @brief Tests the shmem_min_reduce() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_min_reduce(void);

/**
  @brief Tests the shmem_sum_reduce() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_sum_reduce(void);

/**
  @brief Tests the shmem_prod_reduce() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_prod_reduce(void);

#endif /* COLLECTIVES_TESTS_HPP */
