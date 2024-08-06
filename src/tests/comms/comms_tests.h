/**
 * @file comms_tests.h
 * @brief Contains function declarations for the OpenSHMEM communication/context tests.
 */

#ifndef COMMS_TESTS_H
#define COMMS_TESTS_H

#include "routines.h"
#include "shmemvv.h"

#include <shmem.h>

/**
 * @brief Tests the shmem_ctx_create() function.
 *
 * This test verifies that the shmem_ctx_create() function correctly creates a communication context.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_ctx_create(void);

/**
 * @brief Tests the shmem_team_create_ctx() function.
 *
 * This test verifies that the shmem_team_create_ctx() function correctly creates a context
 * for a specified team.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_create_ctx(void);

/**
 * @brief Tests the shmem_ctx_destroy() function.
 *
 * This test verifies that the shmem_ctx_destroy() function correctly destroys a communication context.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_ctx_destroy(void);

/**
 * @brief Tests the shmem_ctx_get_team() function.
 *
 * This test verifies that the shmem_ctx_get_team() function correctly retrieves the team
 * associated with a given communication context.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_ctx_get_team(void);

/**
* @brief Run all comms/ctx tests
 * 
 */
void run_comms_tests(int mype, int npes);

#endif /* COMMS_TESTS_H */
