/**
  @file atomics_tests.cpp
  @brief Contains OpenSHMEM atomic memory operations tests.
 */

#include "atomics_tests.hpp"

/**
  @brief Tests the shmem_atomic_fetch() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_atomic_fetch() {
  static int *dest;
  static int fetch;
  dest = (int *)shmem_malloc(sizeof(int));
  int value = 42;
  *dest = value;
  shmem_barrier_all();
  fetch = shmem_atomic_fetch(dest, 1);
  bool success = (fetch == value);
  shmem_free(dest);
  return success;
}

/**
  @brief Tests the shmem_atomic_set() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_atomic_set() {
  static int *dest;
  dest = (int *)shmem_malloc(sizeof(int));
  int value = 42;
  shmem_barrier_all();
  shmem_atomic_set(dest, value, 1);
  shmem_barrier_all();
  bool success = (*dest == value);
  shmem_free(dest);
  return success;
}

/**
  @brief Tests the shmem_atomic_compare_swap() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_atomic_compare_swap() {
  static int *dest;
  dest = (int *)shmem_malloc(sizeof(int));
  int old = 42, new_val = 43;
  *dest = old;
  shmem_barrier_all();
  int swapped = shmem_atomic_compare_swap(dest, old, new_val, 1);
  bool success = (swapped == old && *dest == new_val);
  shmem_free(dest);
  return success;
}

/**
  @brief Tests the shmem_atomic_swap() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_atomic_swap() {
  static int *dest;
  dest = (int *)shmem_malloc(sizeof(int));
  int value = 42, new_val = 43;
  *dest = value;
  shmem_barrier_all();
  int swapped = shmem_atomic_swap(dest, new_val, 1);
  bool success = (swapped == value && *dest == new_val);
  shmem_free(dest);
  return success;
}

/**
  @brief Tests the shmem_atomic_fetch_inc() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_atomic_fetch_inc() {
  static int *dest;
  static int fetch;
  dest = (int *)shmem_malloc(sizeof(int));
  int value = 42;
  *dest = value;
  shmem_barrier_all();
  fetch = shmem_atomic_fetch_inc(dest, 1);
  bool success = (fetch == value && *dest == value + 1);
  shmem_free(dest);
  return success;
}

/**
  @brief Tests the shmem_atomic_inc() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_atomic_inc() {
  static int *dest;
  dest = (int *)shmem_malloc(sizeof(int));
  int value = 42;
  *dest = value;
  shmem_barrier_all();
  shmem_atomic_inc(dest, 1);
  shmem_barrier_all();
  bool success = (*dest == value + 1);
  shmem_free(dest);
  return success;
}

/**
  @brief Tests the shmem_atomic_fetch_add() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_atomic_fetch_add() {
  static int *dest;
  static int fetch;
  dest = (int *)shmem_malloc(sizeof(int));
  int value = 42, add_val = 10;
  *dest = value;
  shmem_barrier_all();
  fetch = shmem_atomic_fetch_add(dest, add_val, 1);
  bool success = (fetch == value && *dest == value + add_val);
  shmem_free(dest);
  return success;
}

/**
  @brief Tests the shmem_atomic_add() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_atomic_add() {
  static int *dest;
  dest = (int *)shmem_malloc(sizeof(int));
  int value = 42, add_val = 10;
  *dest = value;
  shmem_barrier_all();
  shmem_atomic_add(dest, add_val, 1);
  shmem_barrier_all();
  bool success = (*dest == value + add_val);
  shmem_free(dest);
  return success;
}

/**
  @brief Tests the shmem_atomic_fetch_and() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_atomic_fetch_and() {
  static int *dest;
  static int fetch;
  dest = (int *)shmem_malloc(sizeof(int));
  int value = 42, and_val = 15;
  *dest = value;
  shmem_barrier_all();
  fetch = shmem_atomic_fetch_and(dest, and_val, 1);
  bool success = (fetch == value && *dest == (value & and_val));
  shmem_free(dest);
  return success;
}

/**
  @brief Tests the shmem_atomic_and() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_atomic_and() {
  static int *dest;
  dest = (int *)shmem_malloc(sizeof(int));
  int value = 42, and_val = 15;
  *dest = value;
  shmem_barrier_all();
  shmem_atomic_and(dest, and_val, 1);
  shmem_barrier_all();
  bool success = (*dest == (value & and_val));
  shmem_free(dest);
  return success;
}

/**
  @brief Tests the shmem_atomic_fetch_or() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_atomic_fetch_or() {
  static int *dest;
  static int fetch;
  dest = (int *)shmem_malloc(sizeof(int));
  int value = 42, or_val = 15;
  *dest = value;
  shmem_barrier_all();
  fetch = shmem_atomic_fetch_or(dest, or_val, 1);
  bool success = (fetch == value && *dest == (value | or_val));
  shmem_free(dest);
  return success;
}

/**
  @brief Tests the shmem_atomic_or() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_atomic_or() {
  static int *dest;
  dest = (int *)shmem_malloc(sizeof(int));
  int value = 42, or_val = 15;
  *dest = value;
  shmem_barrier_all();
  shmem_atomic_or(dest, or_val, 1);
  shmem_barrier_all();
  bool success = (*dest == (value | or_val));
  shmem_free(dest);
  return success;
}

/**
  @brief Tests the shmem_atomic_fetch_xor() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_atomic_fetch_xor() {
  static int *dest;
  static int fetch;
  dest = (int *)shmem_malloc(sizeof(int));
  int value = 42, xor_val = 15;
  *dest = value;
  shmem_barrier_all();
  fetch = shmem_atomic_fetch_xor(dest, xor_val, 1);
  bool success = (fetch == value && *dest == (value ^ xor_val));
  shmem_free(dest);
  return success;
}

/**
  @brief Tests the shmem_atomic_xor() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_atomic_xor() {
  static int *dest;
  dest = (int *)shmem_malloc(sizeof(int));
  int value = 42, xor_val = 15;
  *dest = value;
  shmem_barrier_all();
  shmem_atomic_xor(dest, xor_val, 1);
  shmem_barrier_all();
  bool success = (*dest == (value ^ xor_val));
  shmem_free(dest);
  return success;
}

/**
  @brief Tests the shmem_atomic_fetch_nbi() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_atomic_fetch_nbi() {
  static int *dest;
  static int fetch;
  dest = (int *)shmem_malloc(sizeof(int));
  int value = 42;
  *dest = value;
  shmem_barrier_all();
  shmem_atomic_fetch_nbi(&fetch, dest, 1);
  shmem_quiet();
  bool success = (fetch == value);
  shmem_free(dest);
  return success;
}

/**
  @brief Tests the shmem_atomic_compare_swap_nbi() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_atomic_compare_swap_nbi() {
  static int *dest;
  static int fetch;
  dest = (int *)shmem_malloc(sizeof(int));
  fetch = 0;
  int old = 42, new_val = 43;
  *dest = old;
  shmem_barrier_all();
  shmem_atomic_compare_swap_nbi(&fetch, dest, old, new_val, 1);
  shmem_quiet();
  bool success = (fetch == old && *dest == new_val);
  shmem_free(dest);
  return success;
}

/**
  @brief Tests the shmem_atomic_swap_nbi() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_atomic_swap_nbi() {
  static int *dest;
  static int fetch;
  dest = (int *)shmem_malloc(sizeof(int));
  fetch = 0;
  int value = 42, new_val = 43;
  *dest = value;
  shmem_barrier_all();
  shmem_atomic_swap_nbi(&fetch, dest, new_val, 1);
  shmem_quiet();
  bool success = (fetch == value && *dest == new_val);
  shmem_free(dest);
  return success;
}

/**
  @brief Tests the shmem_atomic_fetch_inc_nbi() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_atomic_fetch_inc_nbi() {
  static int *dest;
  static int fetch;
  dest = (int *)shmem_malloc(sizeof(int));
  fetch = 0;
  int value = 42;
  *dest = value;
  shmem_barrier_all();
  shmem_atomic_fetch_inc_nbi(&fetch, dest, 1);
  shmem_quiet();
  bool success = (fetch == value && *dest == value + 1);
  shmem_free(dest);
  return success;
}

/**
  @brief Tests the shmem_atomic_fetch_add_nbi() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_atomic_fetch_add_nbi() {
  static int *dest;
  static int fetch;
  dest = (int *)shmem_malloc(sizeof(int));
  fetch = 0;
  int value = 42, add_val = 10;
  *dest = value;
  shmem_barrier_all();
  shmem_atomic_fetch_add_nbi(&fetch, dest, add_val, 1);
  shmem_quiet();
  bool success = (fetch == value && *dest == value + add_val);
  shmem_free(dest);
  return success;
}

/**
  @brief Tests the shmem_atomic_fetch_and_nbi() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_atomic_fetch_and_nbi() {
  static int *dest;
  static int fetch;
  dest = (int *)shmem_malloc(sizeof(int));
  fetch = 0;
  int value = 42, and_val = 15;
  *dest = value;
  shmem_barrier_all();
  shmem_atomic_fetch_and_nbi(&fetch, dest, and_val, 1);
  shmem_quiet();
  bool success = (fetch == value && *dest == (value & and_val));
  shmem_free(dest);
  return success;
}

/**
  @brief Tests the shmem_atomic_fetch_or_nbi() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_atomic_fetch_or_nbi() {
  static int *dest;
  static int fetch;
  dest = (int *)shmem_malloc(sizeof(int));
  fetch = 0;
  int value = 42, or_val = 15;
  *dest = value;
  shmem_barrier_all();
  shmem_atomic_fetch_or_nbi(&fetch, dest, or_val, 1);
  shmem_quiet();
  bool success = (fetch == value && *dest == (value | or_val));
  shmem_free(dest);
  return success;
}

/**
  @brief Tests the shmem_atomic_fetch_xor_nbi() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_atomic_fetch_xor_nbi() {
  static int *dest;
  static int fetch;
  dest = (int *)shmem_malloc(sizeof(int));
  fetch = 0;
  int value = 42, xor_val = 15;
  *dest = value;
  shmem_barrier_all();
  shmem_atomic_fetch_xor_nbi(&fetch, dest, xor_val, 1);
  shmem_quiet();
  bool success = (fetch == value && *dest == (value ^ xor_val));
  shmem_free(dest);
  return success;
}
