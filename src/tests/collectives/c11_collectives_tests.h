/**
 * @file c11_collectives_tests.h
 * @brief Contains function declarations for the OpenSHMEM collectives tests.
 */

#ifndef C11_COLLECTIVES_TESTS_H
#define C11_COLLECTIVES_TESTS_H

#include "routines.h"
#include "shmemvv.h"

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Tests the shmem_sync() routine.
 *
 * This test verifies that the shmem_sync() routine correctly synchronizes all PEs.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_sync(void);

/**
 * @brief Tests the shmem_alltoall() routine.
 *
 * This test verifies that the shmem_alltoall() routine correctly performs an all-to-all
 * data exchange among all PEs.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_alltoall(void);

/**
 * @brief Tests the shmem_alltoalls() routine.
 *
 * This test verifies that the shmem_alltoalls() routine correctly performs a strided all-to-all
 * data exchange among all PEs.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_alltoalls(void);

/**
 * @brief Tests the shmem_broadcast() routine.
 *
 * This test verifies that the shmem_broadcast() routine correctly broadcasts data
 * from the root PE to all other PEs.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_broadcast(void);

/**
 * @brief Tests the shmem_collect() routine.
 *
 * This test verifies that the shmem_collect() routine correctly collects data
 * from all PEs to a single PE.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_collect(void);

/**
 * @brief Tests the shmem_fcollect() routine.
 *
 * This test verifies that the shmem_fcollect() routine correctly collects data
 * from all PEs to a single PE in a more efficient manner than shmem_collect().
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_fcollect(void);

/**
 * @brief Tests the shmem_and_reduce() routine.
 *
 * This test verifies that the shmem_and_reduce() routine correctly performs a bitwise AND
 * reduction operation across all PEs.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_and_reduce(void);

/**
 * @brief Tests the shmem_max_reduce() routine.
 *
 * This test verifies that the shmem_max_reduce() routine correctly computes the maximum
 * of data from all PEs and stores it on the root PE.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_max_reduce(void);

/**
 * @brief Tests the shmem_min_reduce() routine.
 *
 * This test verifies that the shmem_min_reduce() routine correctly computes the minimum
 * of data from all PEs and stores it on the root PE.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_min_reduce(void);

/**
 * @brief Tests the shmem_sum_reduce() routine.
 *
 * This test verifies that the shmem_sum_reduce() routine correctly computes the sum
 * of data from all PEs and stores it on the root PE.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_sum_reduce(void);

/**
 * @brief Tests the shmem_prod_reduce() routine.
 *
 * This test verifies that the shmem_prod_reduce() routine correctly computes the product
 * of data from all PEs and stores it on the root PE.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_prod_reduce(void);

/**
 * @brief Run all C11 collectives tests
 * 
 */
void run_c11_collectives_tests(int mype, int npes);

#ifdef __cplusplus
}
#endif

#endif /* COLLECTIVES_TESTS_H */

