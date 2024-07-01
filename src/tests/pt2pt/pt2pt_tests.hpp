/**
  @file pt2pt_tests.hpp
  @brief Contains functions with test functions for the pt2pt synchronization routines
 */

#ifndef PT2PT_TESTS_HPP
#define PT2PT_TESTS_HPP

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctime>
#include <unistd.h>

/* Function Prototypes */
/**
 * @brief Tests the shmem_wait_until() routine.
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_wait_until(void);

/**
 * @brief Tests the shmem_wait_until_all() routine.
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_wait_until_all(void);

/**
 * @brief Tests the shmem_wait_until_any() routine.
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_wait_until_any(void);

/**
 * @brief Tests the shmem_wait_until_some() routine.
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_wait_until_some(void);

/**
 * @brief Tests the shmem_wait_until_all_vector() routine.
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_wait_until_all_vector(void);

/**
 * @brief Tests the shmem_wait_until_any_vector() routine.
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_wait_until_any_vector(void);

/**
 * @brief Tests the shmem_wait_until_some_vector() routine.
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_wait_until_some_vector(void);

/**
 * @brief Tests the shmem_test() routine.
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_test(void);

/**
 * @brief Tests the shmem_test_all() routine.
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_test_all(void);

/**
 * @brief Tests the shmem_test_any() routine.
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_test_any(void);

/**
 * @brief Tests the shmem_test_some() routine.
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_test_some(void);

/**
 * @brief Tests the shmem_test_all_vector() routine.
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_test_all_vector(void);

/**
 * @brief Tests the shmem_test_any_vector() routine.
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_test_any_vector(void);

/**
 * @brief Tests the shmem_test_some_vector() routine.
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_test_some_vector(void);

/**
 * @brief Tests the shmem_signal_wait_until() routine.
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_signal_wait_until(void);

#endif /* PT2PT_TESTS_HPP */
