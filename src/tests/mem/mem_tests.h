/**
 * @file mem_tests.h
 * @brief Contains function declarations for the OpenSHMEM memory management tests.
 */

#ifndef MEM_TESTS_H
#define MEM_TESTS_H

#include "routines.h"
#include "shmemvv.h"

#include <shmem.h>
#include <iostream>

/**
 * @brief Tests the shmem_ptr() routine.
 *
 * This test verifies that the shmem_ptr() function correctly provides access to the memory
 * of another PE and that the memory content is accessible and correct.
 *
 * @return True if the pointer is accessible, false otherwise.
 */
bool test_shmem_ptr(void);

/**
 * @brief Tests the shmem_malloc() and shmem_free() functions.
 *
 * This test verifies that the shmem_malloc() function allocates memory correctly and that
 * the shmem_free() function deallocates the memory correctly.
 *
 * @return True if the tests are successful, false otherwise.
 */
bool test_shmem_malloc_free(void);

/**
 * @brief Tests the shmem_addr_accessible() routine.
 *
 * This test verifies that the shmem_addr_accessible() function correctly checks whether
 * a memory address is accessible from all PEs.
 *
 * @return True if the address is accessible from all PEs, false otherwise.
 */
bool test_shmem_addr_accessible(void);

/**
 * @brief Tests the shmem_realloc() routine.
 *
 * This test verifies that the shmem_realloc() function correctly reallocates memory
 * and that the new memory block is usable.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_realloc(void);

/**
 * @brief Tests the shmem_align() routine.
 *
 * This test verifies that the shmem_align() function correctly allocates memory with
 * the specified alignment.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_align(void);

/**
 * @brief Tests the shmem_malloc_with_hints() routine.
 *
 * This test verifies that the shmem_malloc_with_hints() function correctly allocates memory
 * with the specified hints.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_malloc_with_hints(void);

/**
 * @brief Tests the shmem_calloc() routine.
 *
 * This test verifies that the shmem_calloc() function correctly allocates and initializes
 * memory to zero.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_calloc(void);

/**
 * @brief Run all memory tests
 */
void run_mem_tests(int mype, int npes);

#endif /* MEM_TESTS_H */
