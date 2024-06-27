/**
  @file atomics_tests.hpp
  @brief Contains function declarations for the OpenSHMEM atomic memory operations tests.
 */

#ifndef ATOMICS_TESTS_HPP
#define ATOMICS_TESTS_HPP

#include <shmem.h>
#include <iostream>

/* Function declarations for atomic memory operations tests */
bool test_shmem_atomic_fetch(void);
bool test_shmem_atomic_set(void);
bool test_shmem_atomic_compare_swap(void);
bool test_shmem_atomic_swap(void);
bool test_shmem_atomic_fetch_inc(void);
bool test_shmem_atomic_inc(void);
bool test_shmem_atomic_fetch_add(void);
bool test_shmem_atomic_add(void);
bool test_shmem_atomic_fetch_and(void);
bool test_shmem_atomic_and(void);
bool test_shmem_atomic_fetch_or(void);
bool test_shmem_atomic_or(void);
bool test_shmem_atomic_fetch_xor(void);
bool test_shmem_atomic_xor(void);
bool test_shmem_atomic_fetch_nbi(void);
bool test_shmem_atomic_compare_swap_nbi(void);
bool test_shmem_atomic_swap_nbi(void);
bool test_shmem_atomic_fetch_inc_nbi(void);
bool test_shmem_atomic_fetch_add_nbi(void);
bool test_shmem_atomic_fetch_and_nbi(void);
bool test_shmem_atomic_fetch_or_nbi(void);
bool test_shmem_atomic_fetch_xor_nbi(void);

#endif /* ATOMICS_TESTS_HPP */
