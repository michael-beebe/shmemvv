/**
 * @file cxx_remote_tests.h
 * @brief Contains function declarations for the OpenSHMEM remote memory access tests.
 */

#ifndef CXX_REMOTE_TESTS_H
#define CXX_REMOTE_TESTS_H

#include "routines.h"
#include "shmemvv.h"

#include <shmem.h>
#include <iostream>

/**
 * @brief Tests the shmem_put() routine.
 *
 * This test verifies that the shmem_put() function correctly transfers data from one PE to another.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_put(void);

/**
 * @brief Tests the shmem_p() routine.
 *
 * This test verifies that the shmem_p() function correctly transfers a single data element from one PE to another.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_p(void);

/**
 * @brief Tests the shmem_iput() routine.
 *
 * This test verifies that the shmem_iput() function correctly transfers data from one PE to another using an indirect stride.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_iput(void);

/**
 * @brief Tests the shmem_get() routine.
 *
 * This test verifies that the shmem_get() function correctly retrieves data from one PE to another.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_get(void);

/**
 * @brief Tests the shmem_g() routine.
 *
 * This test verifies that the shmem_g() function correctly retrieves a single data element from one PE to another.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_g(void);

/**
 * @brief Tests the shmem_iget() routine.
 *
 * This test verifies that the shmem_iget() function correctly retrieves data from one PE to another using an indirect stride.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_iget(void);

/**
 * @brief Tests the shmem_put_nbi() routine.
 *
 * This test verifies that the shmem_put_nbi() function correctly transfers data from one PE to another using non-blocking operations.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_put_nbi(void);

/**
 * @brief Tests the shmem_get_nbi() routine.
 *
 * This test verifies that the shmem_get_nbi() function correctly retrieves data from one PE to another using non-blocking operations.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_get_nbi(void);

/**
 * @brief Run all C/CXX RMA tests
 */
void run_cxx_remote_tests(int mype, int npes);

#endif /* REMOTE_TESTS_H */
