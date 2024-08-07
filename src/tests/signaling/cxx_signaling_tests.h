/**
 * @file cxx_signaling_tests.h
 * @brief Contains function declarations for the OpenSHMEM signaling tests.
 */

#ifndef CXX_SIGNALING_TESTS_H
#define CXX_SIGNALING_TESTS_H

#include "routines.h"
#include "shmemvv.h"

#include <shmem.h>

/**
 * @brief Tests the shmem_put_signal() routine.
 *
 * This test verifies that the shmem_put_signal() function correctly transfers a
 * value and sets a signal on the target PE.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_put_signal(void);

/**
 * @brief Tests the shmem_put_signal_nbi() routine.
 *
 * This test verifies that the shmem_put_signal_nbi() function correctly
 * transfers a value and sets a signal on the target PE using non-blocking
 * operations.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_put_signal_nbi(void);

/**
 * @brief Tests the shmem_signal_fetch() routine.
 *
 * This test verifies that the shmem_signal_fetch() function correctly fetches
 * the signal value from the target PE.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_signal_fetch(void);

/**
 * @brief Run all C/CXX signaling tests
 */
void run_cxx_signaling_tests(int mype, int npes);

#endif /* SIGNALING_TESTS_H */
