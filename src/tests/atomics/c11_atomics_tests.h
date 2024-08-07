/**
 * @file c11_atomics_tests.h
 * @brief Contains function declarations for the OpenSHMEM atomic memory operations tests.
 */

#ifndef C11_ATOMICS_TESTS_H
#define C11_ATOMICS_TESTS_H

#include "routines.h"
#include "shmemvv.h"

#include <shmem.h>
#include <stdio.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Tests the shmem_atomic_fetch() routine.
 *
 * This test verifies that the shmem_atomic_fetch() routine correctly retrieves the value
 * from a remote memory location.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_atomic_fetch(void);

/**
 * @brief Tests the shmem_atomic_set() routine.
 *
 * This test verifies that the shmem_atomic_set() routine correctly sets the value
 * at a remote memory location.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_atomic_set(void);

/**
 * @brief Tests the shmem_atomic_compare_swap() routine.
 *
 * This test verifies that the shmem_atomic_compare_swap() routine correctly swaps the value
 * at a remote memory location if it matches the expected value.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_atomic_compare_swap(void);

/**
 * @brief Tests the shmem_atomic_swap() routine.
 *
 * This test verifies that the shmem_atomic_swap() routine correctly swaps the value
 * at a remote memory location and returns the old value.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_atomic_swap(void);

/**
 * @brief Tests the shmem_atomic_fetch_inc() routine.
 *
 * This test verifies that the shmem_atomic_fetch_inc() routine correctly increments the value
 * at a remote memory location and returns the old value.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_atomic_fetch_inc(void);

/**
 * @brief Tests the shmem_atomic_inc() routine.
 *
 * This test verifies that the shmem_atomic_inc() routine correctly increments the value
 * at a remote memory location.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_atomic_inc(void);

/**
 * @brief Tests the shmem_atomic_fetch_add() routine.
 *
 * This test verifies that the shmem_atomic_fetch_add() routine correctly adds a value
 * to the remote memory location and returns the old value.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_atomic_fetch_add(void);

/**
 * @brief Tests the shmem_atomic_add() routine.
 *
 * This test verifies that the shmem_atomic_add() routine correctly adds a value
 * to the remote memory location.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_atomic_add(void);

/**
 * @brief Tests the shmem_atomic_fetch_and() routine.
 *
 * This test verifies that the shmem_atomic_fetch_and() routine correctly performs a bitwise AND
 * operation with the remote memory location and returns the old value.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_atomic_fetch_and(void);

/**
 * @brief Tests the shmem_atomic_and() routine.
 *
 * This test verifies that the shmem_atomic_and() routine correctly performs a bitwise AND
 * operation with the remote memory location.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_atomic_and(void);

/**
 * @brief Tests the shmem_atomic_fetch_or() routine.
 *
 * This test verifies that the shmem_atomic_fetch_or() routine correctly performs a bitwise OR
 * operation with the remote memory location and returns the old value.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_atomic_fetch_or(void);

/**
 * @brief Tests the shmem_atomic_or() routine.
 *
 * This test verifies that the shmem_atomic_or() routine correctly performs a bitwise OR
 * operation with the remote memory location.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_atomic_or(void);

/**
 * @brief Tests the shmem_atomic_fetch_xor() routine.
 *
 * This test verifies that the shmem_atomic_fetch_xor() routine correctly performs a bitwise XOR
 * operation with the remote memory location and returns the old value.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_atomic_fetch_xor(void);

/**
 * @brief Tests the shmem_atomic_xor() routine.
 *
 * This test verifies that the shmem_atomic_xor() routine correctly performs a bitwise XOR
 * operation with the remote memory location.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_atomic_xor(void);

/**
 * @brief Tests the shmem_atomic_fetch_nbi() routine.
 *
 * This test verifies that the shmem_atomic_fetch_nbi() routine correctly retrieves the value
 * from a remote memory location in a non-blocking manner.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_atomic_fetch_nbi(void);

/**
 * @brief Tests the shmem_atomic_compare_swap_nbi() routine.
 *
 * This test verifies that the shmem_atomic_compare_swap_nbi() routine correctly swaps the value
 * at a remote memory location in a non-blocking manner if it matches the expected value.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_atomic_compare_swap_nbi(void);

/**
 * @brief Tests the shmem_atomic_swap_nbi() routine.
 *
 * This test verifies that the shmem_atomic_swap_nbi() routine correctly swaps the value
 * at a remote memory location in a non-blocking manner and returns the old value.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_atomic_swap_nbi(void);

/**
 * @brief Tests the shmem_atomic_fetch_inc_nbi() routine.
 *
 * This test verifies that the shmem_atomic_fetch_inc_nbi() routine correctly increments the value
 * at a remote memory location in a non-blocking manner and returns the old value.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_atomic_fetch_inc_nbi(void);

/**
 * @brief Tests the shmem_atomic_fetch_add_nbi() routine.
 *
 * This test verifies that the shmem_atomic_fetch_add_nbi() routine correctly adds a value
 * to the remote memory location in a non-blocking manner and returns the old value.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_atomic_fetch_add_nbi(void);

/**
 * @brief Tests the shmem_atomic_fetch_and_nbi() routine.
 *
 * This test verifies that the shmem_atomic_fetch_and_nbi() routine correctly performs a bitwise AND
 * operation with the remote memory location in a non-blocking manner and returns the old value.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_atomic_fetch_and_nbi(void);

/**
 * @brief Tests the shmem_atomic_fetch_or_nbi() routine.
 *
 * This test verifies that the shmem_atomic_fetch_or_nbi() routine correctly performs a bitwise OR
 * operation with the remote memory location in a non-blocking manner and returns the old value.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_atomic_fetch_or_nbi(void);

/**
 * @brief Tests the shmem_atomic_fetch_xor_nbi() routine.
 *
 * This test verifies that the shmem_atomic_fetch_xor_nbi() routine correctly performs a bitwise XOR
 * operation with the remote memory location in a non-blocking manner and returns the old value.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_atomic_fetch_xor_nbi(void);

/**
 * @brief Run all C11 atomics tests
 */
void run_c11_atomics_tests(int mype, int npes);

#ifdef __cplusplus
}
#endif

#endif /* C11_ATOMICS_TESTS_H */
