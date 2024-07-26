/**
 * @file pt2pt_tests.h
 * @brief Contains function declarations for the point-to-point synchronization routines tests.
 */

#ifndef PT2PT_TESTS_H
#define PT2PT_TESTS_H

#include "routines.h"
#include <shmem.h>
#include <iostream>
#include <stdio.h>
#include <stdbool.h>
#include <ctime>
#include <unistd.h>

/* Function Prototypes */
/**
 * @brief Tests the shmem_wait_until() routine.
 *
 * This test verifies that the shmem_wait_until() function correctly waits until a condition
 * on a memory location is met.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_wait_until(void);

/**
 * @brief Tests the shmem_wait_until_all() routine.
 *
 * This test verifies that the shmem_wait_until_all() function correctly waits until all specified
 * conditions on an array of memory locations are met.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_wait_until_all(void);

/**
 * @brief Tests the shmem_wait_until_any() routine.
 *
 * This test verifies that the shmem_wait_until_any() function correctly waits until any one
 * of the specified conditions on an array of memory locations is met.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_wait_until_any(void);

/**
 * @brief Tests the shmem_wait_until_some() routine.
 *
 * This test verifies that the shmem_wait_until_some() function correctly waits until some
 * of the specified conditions on an array of memory locations are met.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_wait_until_some(void);

/**
 * @brief Tests the shmem_wait_until_all_vector() routine.
 *
 * This test verifies that the shmem_wait_until_all_vector() function correctly waits until all specified
 * conditions on a vector of memory locations are met.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_wait_until_all_vector(void);

/**
 * @brief Tests the shmem_wait_until_any_vector() routine.
 *
 * This test verifies that the shmem_wait_until_any_vector() function correctly waits until any one
 * of the specified conditions on a vector of memory locations is met.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_wait_until_any_vector(void);

/**
 * @brief Tests the shmem_wait_until_some_vector() routine.
 *
 * This test verifies that the shmem_wait_until_some_vector() function correctly waits until some
 * of the specified conditions on a vector of memory locations are met.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_wait_until_some_vector(void);

/**
 * @brief Tests the shmem_test() routine.
 *
 * This test verifies that the shmem_test() function correctly tests whether a condition
 * on a memory location is met without blocking.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_test(void);

/**
 * @brief Tests the shmem_test_all() routine.
 *
 * This test verifies that the shmem_test_all() function correctly tests whether all specified
 * conditions on an array of memory locations are met without blocking.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_test_all(void);

/**
 * @brief Tests the shmem_test_any() routine.
 *
 * This test verifies that the shmem_test_any() function correctly tests whether any one
 * of the specified conditions on an array of memory locations is met without blocking.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_test_any(void);

/**
 * @brief Tests the shmem_test_some() routine.
 *
 * This test verifies that the shmem_test_some() function correctly tests whether some
 * of the specified conditions on an array of memory locations are met without blocking.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_test_some(void);

/**
 * @brief Tests the shmem_test_all_vector() routine.
 *
 * This test verifies that the shmem_test_all_vector() function correctly tests whether all specified
 * conditions on a vector of memory locations are met without blocking.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_test_all_vector(void);

/**
 * @brief Tests the shmem_test_any_vector() routine.
 *
 * This test verifies that the shmem_test_any_vector() function correctly tests whether any one
 * of the specified conditions on a vector of memory locations is met without blocking.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_test_any_vector(void);

/**
 * @brief Tests the shmem_test_some_vector() routine.
 *
 * This test verifies that the shmem_test_some_vector() function correctly tests whether some
 * of the specified conditions on a vector of memory locations are met without blocking.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_test_some_vector(void);

/**
 * @brief Tests the shmem_signal_wait_until() routine.
 *
 * This test verifies that the shmem_signal_wait_until() function correctly waits until a signal
 * on a memory location meets a specified condition.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_signal_wait_until(void);

#endif /* PT2PT_TESTS_H */
