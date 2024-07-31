/**
 * @file mem_ordering_tests.h
 * @brief Contains function declarations for the OpenSHMEM memory ordering tests.
 */

#ifndef MEM_ORDERING_TESTS_H
#define MEM_ORDERING_TESTS_H

#include "routines.h"
#include "shmemvv.h"

#include <shmem.h>
#include <iostream>
#include <cstdlib>
#include <cstring>

/**
 * @brief Tests the shmem_fence() routine.
 *
 * This test verifies that the shmem_fence() routine correctly ensures the ordering of memory
 * operations by checking that the memory store operation on one PE is seen by another PE
 * in the correct order.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_fence(void);

/**
 * @brief Tests the shmem_quiet() routine.
 *
 * This test verifies that the shmem_quiet() routine correctly ensures the completion of
 * all outstanding memory operations by checking that a memory store operation on one PE
 * is completed before proceeding.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_quiet(void);

/**
 * TODO: write docs
 * 
 */
void run_mem_ordering_tests(int mype, int npes);

#endif /* MEM_ORDERING_TESTS_H */
