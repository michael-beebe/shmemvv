/**
 * @file signaling_tests.h
 * @brief Contains function declarations for the OpenSHMEM signaling tests.
 */

#ifndef C11_SIGNALING_TESTS_H
#define C11_SIGNALING_TESTS_H

#include "routines.h"
#include "shmemvv.h"

#include <shmem.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Tests the shmem_put_signal() routine.
 *
 * This test verifies that the shmem_put_signal() function correctly transfers a value
 * and sets a signal on the target PE.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_put_signal(void);

/**
 * @brief Tests the shmem_put_signal_nbi() routine.
 *
 * This test verifies that the shmem_put_signal_nbi() function correctly transfers a value
 * and sets a signal on the target PE using non-blocking operations.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_put_signal_nbi(void);

/**
 * @brief Tests the shmem_signal_fetch() routine.
 *
 * This test verifies that the shmem_signal_fetch() function correctly fetches the signal value
 * from the target PE.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_signal_fetch(void);

/**
 * TODO: write docs
 * 
 */
void run_c11_signaling_tests(int mype, int npes);

#ifdef __cplusplus
}
#endif

#endif /* SIGNALING_TESTS_H */

