/**
 * @file threads_tests.h
 * @brief Contains function declarations for the OpenSHMEM threads tests.
 */

#ifndef THREADS_TESTS_H
#define THREADS_TESTS_H

#include "routines.h"
#include "shmemvv.h"

#include <shmem.h>
#include <iostream>

/**
 * @brief Tests the initialization of OpenSHMEM with threading support.
 *
 * This test verifies that OpenSHMEM can be initialized with the specified level of threading support.
 *
 * @return True if the initialization with threading support is successful, false otherwise.
 */
bool test_shmem_init_thread(void);

/**
 * @brief Tests querying the level of threading support in OpenSHMEM.
 *
 * This test verifies that the `p_shmem_query_thread` function correctly
 * queries the level of threading support provided by OpenSHMEM.
 *
 * @return True if the query is successful and the level of threading
 *         support is one of the valid levels, false otherwise.
 */
bool test_shmem_query_thread(void);

#endif /* THREADS_TESTS_H */
