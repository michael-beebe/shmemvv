/**
 * @file cxx_atomics_tests.cpp
 * @brief Contains tests for OpenSHMEM atomic routines.
 */

#include "cxx_atomics_tests.h"

/**
 * @brief Tests the shmem_atomic_fetch() routine.
 *
 * This test verifies that the shmem_atomic_fetch() routine correctly retrieves the value
 * from a remote memory location.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_atomic_fetch() {
  static ulong *dest;
  static ulong fetch;
  dest = (ulong *)shmem_malloc(sizeof(ulong));
  ulong value = 42;
  *dest = value;
  shmem_barrier_all();
  int mype = shmem_my_pe();
  fetch = shmem_ulong_atomic_fetch(dest, mype);
  shmem_barrier_all();
  bool success = (fetch == value);
  shmem_free(dest);
  return success;
}

/**
 * @brief Tests the shmem_atomic_set() routine.
 *
 * This test verifies that the shmem_atomic_set() routine correctly sets the value
 * at a remote memory location.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_atomic_set() {
  static ulong *dest;
  dest = (ulong *)shmem_malloc(sizeof(ulong));
  ulong value = 42;
  shmem_barrier_all();
  int mype = shmem_my_pe();
  shmem_ulong_atomic_set(dest, value, mype);
  shmem_barrier_all();
  bool success = (*dest == value);
  shmem_free(dest);
  return success;
}

/**
 * @brief Tests the shmem_atomic_compare_swap() routine.
 *
 * This test verifies that the shmem_atomic_compare_swap() routine correctly swaps the value
 * at a remote memory location if it matches the expected value.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_atomic_compare_swap() {
  static ulong *dest;
  dest = (ulong *)shmem_malloc(sizeof(ulong));
  ulong old = 42, new_val = 43;
  *dest = old;
  shmem_barrier_all();
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();
  shmem_barrier_all(); 
  ulong swapped = shmem_ulong_atomic_compare_swap(dest, old, new_val, (mype + 1) % npes);
  shmem_barrier_all();
  bool success = (swapped == old && *dest == new_val);
  shmem_barrier_all();
  shmem_free(dest);
  return success;
}

/**
 * @brief Tests the shmem_atomic_swap() routine.
 *
 * This test verifies that the shmem_atomic_swap() routine correctly swaps the value
 * at a remote memory location and returns the old value.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_atomic_swap() {
  static ulong *dest;
  dest = (ulong *)shmem_malloc(sizeof(ulong));
  ulong value = 42, new_val = 43;
  *dest = value;
  shmem_barrier_all();
  int mype = shmem_my_pe();
  ulong swapped = shmem_ulong_atomic_swap(dest, new_val, mype);
  shmem_barrier_all();
  bool success = (swapped == value && *dest == new_val);
  shmem_free(dest);
  return success;
}

/**
 * @brief Tests the shmem_atomic_fetch_inc() routine.
 *
 * This test verifies that the shmem_atomic_fetch_inc() routine correctly increments the value
 * at a remote memory location and returns the old value.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_atomic_fetch_inc() {
  static ulong *dest;
  static ulong fetch;
  dest = (ulong *)shmem_malloc(sizeof(ulong));
  ulong value = 42;
  *dest = value;
  shmem_barrier_all();
  int mype = shmem_my_pe();
  fetch = shmem_ulong_atomic_fetch_inc(dest, mype);
  shmem_barrier_all();
  bool success = (fetch == value && *dest == value + 1);
  shmem_free(dest);
  return success;
}

/**
 * @brief Tests the shmem_atomic_inc() routine.
 *
 * This test verifies that the shmem_atomic_inc() routine correctly increments the value
 * at a remote memory location.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_atomic_inc() {
  static ulong *dest;
  dest = (ulong *)shmem_malloc(sizeof(ulong));
  ulong value = 42;
  *dest = value;
  shmem_barrier_all();
  int mype = shmem_my_pe();
  shmem_ulong_atomic_inc(dest, mype);
  shmem_barrier_all();
  bool success = (*dest == value + 1);
  shmem_free(dest);
  return success;
}

/**
 * @brief Tests the shmem_atomic_fetch_add() routine.
 *
 * This test verifies that the shmem_atomic_fetch_add() routine correctly adds a value
 * to the remote memory location and returns the old value.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_atomic_fetch_add() {
  static ulong *dest;
  static ulong fetch;
  dest = (ulong *)shmem_malloc(sizeof(ulong));
  ulong value = 42, add_val = 10;
  *dest = value;
  shmem_barrier_all();
  int mype = shmem_my_pe();
  fetch = shmem_ulong_atomic_fetch_add(dest, add_val, mype);
  shmem_barrier_all();
  bool success = (fetch == value && *dest == value + add_val);
  shmem_free(dest);
  return success;
}

/**
 * @brief Tests the shmem_atomic_add() routine.
 *
 * This test verifies that the shmem_atomic_add() routine correctly adds a value
 * to the remote memory location.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_atomic_add() {
  static ulong *dest;
  dest = (ulong *)shmem_malloc(sizeof(ulong));
  ulong value = 42, add_val = 10;
  *dest = value;
  shmem_barrier_all();
  int mype = shmem_my_pe();
  shmem_ulong_atomic_add(dest, add_val, mype);
  shmem_barrier_all();
  bool success = (*dest == value + add_val);
  shmem_free(dest);
  return success;
}

/**
 * @brief Tests the shmem_atomic_fetch_and() routine.
 *
 * This test verifies that the shmem_atomic_fetch_and() routine correctly performs a bitwise AND
 * operation with the remote memory location and returns the old value.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_atomic_fetch_and() {
  static ulong *dest;
  static ulong fetch;
  dest = (ulong *)shmem_malloc(sizeof(ulong));
  ulong value = 42, and_val = 15;
  *dest = value;
  shmem_barrier_all();
  int mype = shmem_my_pe();
  fetch = shmem_ulong_atomic_fetch_and(dest, and_val, mype);
  shmem_barrier_all();
  bool success = (fetch == value && *dest == (value & and_val));
  shmem_free(dest);
  return success;
}

/**
 * @brief Tests the shmem_atomic_and() routine.
 *
 * This test verifies that the shmem_atomic_and() routine correctly performs a bitwise AND
 * operation with the remote memory location.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_atomic_and() {
  static ulong *dest;
  dest = (ulong *)shmem_malloc(sizeof(ulong));
  ulong value = 42, and_val = 15;
  *dest = value;
  shmem_barrier_all();
  int mype = shmem_my_pe();
  shmem_ulong_atomic_and(dest, and_val, mype);
  shmem_barrier_all();
  bool success = (*dest == (value & and_val));
  shmem_free(dest);
  return success;
}

/**
 * @brief Tests the shmem_atomic_fetch_or() routine.
 *
 * This test verifies that the shmem_atomic_fetch_or() routine correctly performs a bitwise OR
 * operation with the remote memory location and returns the old value.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_atomic_fetch_or() {
  static ulong *dest;
  static ulong fetch;
  dest = (ulong *)shmem_malloc(sizeof(ulong));
  ulong value = 42, or_val = 15;
  *dest = value;
  shmem_barrier_all();
  int mype = shmem_my_pe();
  fetch = shmem_ulong_atomic_fetch_or(dest, or_val, mype);
  shmem_barrier_all();
  bool success = (fetch == value && *dest == (value | or_val));
  shmem_free(dest);
  return success;
}

/**
 * @brief Tests the shmem_atomic_or() routine.
 *
 * This test verifies that the shmem_atomic_or() routine correctly performs a bitwise OR
 * operation with the remote memory location.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_atomic_or() {
  static ulong *dest;
  dest = (ulong *)shmem_malloc(sizeof(ulong));
  ulong value = 42, or_val = 15;
  *dest = value;
  shmem_barrier_all();
  int mype = shmem_my_pe();
  shmem_ulong_atomic_or(dest, or_val, mype);
  shmem_barrier_all();
  bool success = (*dest == (value | or_val));
  shmem_free(dest);
  return success;
}

/**
 * @brief Tests the shmem_atomic_fetch_xor() routine.
 *
 * This test verifies that the shmem_atomic_fetch_xor() routine correctly performs a bitwise XOR
 * operation with the remote memory location and returns the old value.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_atomic_fetch_xor() {
  static ulong *dest;
  static ulong fetch;
  dest = (ulong *)shmem_malloc(sizeof(ulong));
  ulong value = 42, xor_val = 15;
  *dest = value;
  shmem_barrier_all();
  int mype = shmem_my_pe();
  fetch = shmem_ulong_atomic_fetch_xor(dest, xor_val, mype);
  shmem_barrier_all();
  bool success = (fetch == value && *dest == (value ^ xor_val));
  shmem_free(dest);
  return success;
}

/**
 * @brief Tests the shmem_atomic_xor() routine.
 *
 * This test verifies that the shmem_atomic_xor() routine correctly performs a bitwise XOR
 * operation with the remote memory location.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_atomic_xor() {
  static ulong *dest;
  dest = (ulong *)shmem_malloc(sizeof(ulong));
  ulong value = 42, xor_val = 15;
  *dest = value;
  shmem_barrier_all();
  int mype = shmem_my_pe();
  shmem_ulong_atomic_xor(dest, xor_val, mype);
  shmem_barrier_all();
  bool success = (*dest == (value ^ xor_val));
  shmem_free(dest);
  return success;
}

/**
 * @brief Tests the shmem_atomic_fetch_nbi() routine.
 *
 * This test verifies that the shmem_atomic_fetch_nbi() routine correctly retrieves the value
 * from a remote memory location in a non-blocking manner.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_atomic_fetch_nbi() {
  static ulong *dest;
  static ulong fetch;
  dest = (ulong *)shmem_malloc(sizeof(ulong));
  ulong value = 42;
  *dest = value;
  shmem_barrier_all();
  int mype = shmem_my_pe();
  shmem_ulong_atomic_fetch_nbi(&fetch, dest, mype);
  shmem_quiet();
  shmem_barrier_all();
  bool success = (fetch == value);
  shmem_free(dest);
  return success;
}

/**
 * @brief Tests the shmem_atomic_compare_swap_nbi() routine.
 *
 * This test verifies that the shmem_atomic_compare_swap_nbi() routine correctly swaps the value
 * at a remote memory location in a non-blocking manner if it matches the expected value.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_atomic_compare_swap_nbi() {
  static ulong *dest;
  static ulong fetch;
  dest = (ulong *)shmem_malloc(sizeof(ulong));
  fetch = 0;
  ulong old = 42, new_val = 43;
  *dest = old;
  shmem_barrier_all();
  int mype = shmem_my_pe();
  shmem_ulong_atomic_compare_swap_nbi(&fetch, dest, old, new_val, mype);
  shmem_quiet();
  shmem_barrier_all();
  bool success = (fetch == old && *dest == new_val);
  shmem_free(dest);
  return success;
}

/**
 * @brief Tests the shmem_atomic_swap_nbi() routine.
 *
 * This test verifies that the shmem_atomic_swap_nbi() routine correctly swaps the value
 * at a remote memory location in a non-blocking manner and returns the old value.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_atomic_swap_nbi() {
  static ulong *dest;
  static ulong fetch;
  dest = (ulong *)shmem_malloc(sizeof(ulong));
  fetch = 0;
  ulong value = 42, new_val = 43;
  *dest = value;
  shmem_barrier_all();
  int mype = shmem_my_pe();
  shmem_ulong_atomic_swap_nbi(&fetch, dest, new_val, mype);
  shmem_quiet();
  shmem_barrier_all();
  bool success = (fetch == value && *dest == new_val);
  shmem_free(dest);
  return success;
}

/**
 * @brief Tests the shmem_atomic_fetch_inc_nbi() routine.
 *
 * This test verifies that the shmem_atomic_fetch_inc_nbi() routine correctly increments the value
 * at a remote memory location in a non-blocking manner and returns the old value.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_atomic_fetch_inc_nbi() {
  static ulong *dest;
  static ulong fetch;
  dest = (ulong *)shmem_malloc(sizeof(ulong));
  fetch = 0;
  ulong value = 42;
  *dest = value;
  shmem_barrier_all();
  int mype = shmem_my_pe();
  shmem_ulong_atomic_fetch_inc_nbi(&fetch, dest, mype);
  shmem_quiet();
  shmem_barrier_all();
  bool success = (fetch == value && *dest == value + 1);
  shmem_free(dest);
  return success;
}

/**
 * @brief Tests the shmem_atomic_fetch_add_nbi() routine.
 *
 * This test verifies that the shmem_atomic_fetch_add_nbi() routine correctly adds a value
 * to the remote memory location in a non-blocking manner and returns the old value.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_atomic_fetch_add_nbi() {
  static ulong *dest;
  static ulong fetch;
  dest = (ulong *)shmem_malloc(sizeof(ulong));
  fetch = 0;
  ulong value = 42, add_val = 10;
  *dest = value;
  shmem_barrier_all();
  int mype = shmem_my_pe();
  shmem_ulong_atomic_fetch_add_nbi(&fetch, dest, add_val, mype);
  shmem_quiet();
  shmem_barrier_all();
  bool success = (fetch == value && *dest == value + add_val);
  shmem_free(dest);
  return success;
}

/**
 * @brief Tests the shmem_atomic_fetch_and_nbi() routine.
 *
 * This test verifies that the shmem_atomic_fetch_and_nbi() routine correctly performs a bitwise AND
 * operation with the remote memory location in a non-blocking manner and returns the old value.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_atomic_fetch_and_nbi() {
  static ulong *dest;
  static ulong fetch;
  dest = (ulong *)shmem_malloc(sizeof(ulong));
  fetch = 0;
  ulong value = 42, and_val = 15;
  *dest = value;
  shmem_barrier_all();
  int mype = shmem_my_pe();
  shmem_ulong_atomic_fetch_and_nbi(&fetch, dest, and_val, mype);
  shmem_quiet();
  shmem_barrier_all();
  bool success = (fetch == value && *dest == (value & and_val));
  shmem_free(dest);
  return success;
}

/**
 * @brief Tests the shmem_atomic_fetch_or_nbi() routine.
 *
 * This test verifies that the shmem_atomic_fetch_or_nbi() routine correctly performs a bitwise OR
 * operation with the remote memory location in a non-blocking manner and returns the old value.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_atomic_fetch_or_nbi() {
  static ulong *dest;
  static ulong fetch;
  dest = (ulong *)shmem_malloc(sizeof(ulong));
  fetch = 0;
  ulong value = 42, or_val = 15;
  *dest = value;
  shmem_barrier_all();
  int mype = shmem_my_pe();
  shmem_ulong_atomic_fetch_or_nbi(&fetch, dest, or_val, mype);
  shmem_quiet();
  shmem_barrier_all();
  bool success = (fetch == value && *dest == (value | or_val));
  shmem_free(dest);
  return success;
}

/**
 * @brief Tests the shmem_atomic_fetch_xor_nbi() routine.
 *
 * This test verifies that the shmem_atomic_fetch_xor_nbi() routine correctly performs a bitwise XOR
 * operation with the remote memory location in a non-blocking manner and returns the old value.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_atomic_fetch_xor_nbi() {
  static ulong *dest;
  static ulong fetch;
  dest = (ulong *)shmem_malloc(sizeof(ulong));
  fetch = 0;
  ulong value = 42, xor_val = 15;
  *dest = value;
  shmem_barrier_all();
  int mype = shmem_my_pe();
  shmem_ulong_atomic_fetch_xor_nbi(&fetch, dest, xor_val, mype);
  shmem_quiet();
  shmem_barrier_all();
  bool success = (fetch == value && *dest == (value ^ xor_val));
  shmem_free(dest);
  return success;
}

/**
 * @brief Run all C/CXX atomics tests
 */
void run_cxx_atomics_tests(int mype, int npes) {
  /* Make sure there are at least 2 PEs */
  if ( !(npes > 1) ) {
    if (mype == 0) {
      display_not_enough_pes("ATOMIC MEMORY OPS");
    }
  }
  else {
    /* Run shmem_atomic_fetch() test */
    shmem_barrier_all();
    bool result_shmem_atomic_fetch = test_cxx_shmem_atomic_fetch();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_fetch()", result_shmem_atomic_fetch, false);
    }

    /* Run shmem_atomic_set() test */
    shmem_barrier_all();
    bool result_shmem_atomic_set = test_cxx_shmem_atomic_set();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_set()", result_shmem_atomic_set, false); 
    }

    /* Run shmem_atomic_compare_swap() test */
    shmem_barrier_all();
    bool result_shmem_atomic_compare_swap = test_cxx_shmem_atomic_compare_swap();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_compare_swap()", result_shmem_atomic_compare_swap, false); 
    }

    /* Run shmem_atomic_swap() test */
    shmem_barrier_all();
    bool result_shmem_atomic_swap = test_cxx_shmem_atomic_swap();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_swap()", result_shmem_atomic_swap, false);
    }

    /* Run shmem_atomic_fetch_inc() test */
    shmem_barrier_all();
    bool result_shmem_atomic_fetch_inc = test_cxx_shmem_atomic_fetch_inc();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_fetch_inc()", result_shmem_atomic_fetch_inc, false);
    }

    /* Run shmem_atomic_inc() test */
    shmem_barrier_all();
    bool result_shmem_atomic_inc = test_cxx_shmem_atomic_inc();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_inc()", result_shmem_atomic_inc, false);
    }

    /* Run shmem_atomic_fetch_add() test */
    shmem_barrier_all();
    bool result_shmem_atomic_fetch_add = test_cxx_shmem_atomic_fetch_add();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_fetch_add()", result_shmem_atomic_fetch_add, false);
    }

    /* Run shmem_atomic_add() test */
    shmem_barrier_all();
    bool result_shmem_atomic_add = test_cxx_shmem_atomic_add();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_add()", result_shmem_atomic_add, false);
    }

    /* Run shmem_atomic_fetch_and() test */
    shmem_barrier_all();
    bool result_shmem_atomic_fetch_and = test_cxx_shmem_atomic_fetch_and();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_fetch_and()", result_shmem_atomic_fetch_and, false);
    }

    /* Run shmem_atomic_and() test */
    shmem_barrier_all();
    bool result_shmem_atomic_and = test_cxx_shmem_atomic_and();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_and()", result_shmem_atomic_and, false);
    }

    /* Run shmem_atomic_fetch_or() test */
    shmem_barrier_all();
    bool result_shmem_atomic_fetch_or = test_cxx_shmem_atomic_fetch_or();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_fetch_or()", result_shmem_atomic_fetch_or, false);
    }

    /* Run shmem_atomic_or() test */
    shmem_barrier_all();
    bool result_shmem_atomic_or = test_cxx_shmem_atomic_or();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_or()", result_shmem_atomic_or, false);
    }

    /* Run shmem_atomic_fetch_xor() test */
    shmem_barrier_all();
    bool result_shmem_atomic_fetch_xor = test_cxx_shmem_atomic_fetch_xor();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_fetch_xor()", result_shmem_atomic_fetch_xor, false);
    }

    /* Run shmem_atomic_xor() test */
    shmem_barrier_all();
    bool result_shmem_atomic_xor = test_cxx_shmem_atomic_xor();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_xor()", result_shmem_atomic_xor, false);
    }

    /* Run shmem_atomic_fetch_nbi() test */
    shmem_barrier_all();
    bool result_shmem_atomic_fetch_nbi = test_cxx_shmem_atomic_fetch_nbi();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_fetch_nbi()", result_shmem_atomic_fetch_nbi, false);
    }

    /* Run shmem_atomic_compare_swap_nbi() test */
    shmem_barrier_all();
    bool result_shmem_atomic_compare_swap_nbi = test_cxx_shmem_atomic_compare_swap_nbi();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_compare_swap_nbi()", result_shmem_atomic_compare_swap_nbi, false);
    }

    /* Run shmem_atomic_swap_nbi() test */
    shmem_barrier_all();
    bool result_shmem_atomic_swap_nbi = test_cxx_shmem_atomic_swap_nbi();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_swap_nbi()", result_shmem_atomic_swap_nbi, false);
    }

    /* Run shmem_atomic_fetch_inc_nbi() test */
    shmem_barrier_all();
    bool result_shmem_atomic_fetch_inc_nbi = test_cxx_shmem_atomic_fetch_inc_nbi();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_fetch_inc_nbi()", result_shmem_atomic_fetch_inc_nbi, false);
    }

    /* Run shmem_atomic_fetch_add_nbi() test */
    shmem_barrier_all();
    bool result_shmem_atomic_fetch_add_nbi = test_cxx_shmem_atomic_fetch_add_nbi();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_fetch_add_nbi()", result_shmem_atomic_fetch_add_nbi, false);
    }

    /* Run shmem_atomic_fetch_and_nbi() test */
    shmem_barrier_all();
    bool result_shmem_atomic_fetch_and_nbi = test_cxx_shmem_atomic_fetch_and_nbi();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_fetch_and_nbi()", result_shmem_atomic_fetch_and_nbi, false);
    }

    /* Run shmem_atomic_fetch_or_nbi() test */
    shmem_barrier_all();
    bool result_shmem_atomic_fetch_or_nbi = test_cxx_shmem_atomic_fetch_or_nbi();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_fetch_or_nbi()", result_shmem_atomic_fetch_or_nbi, false);
    }

    /* Run shmem_atomic_fetch_xor_nbi() test */
    shmem_barrier_all();
    bool result_shmem_atomic_fetch_xor_nbi = test_cxx_shmem_atomic_fetch_xor_nbi();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_fetch_xor_nbi()", result_shmem_atomic_fetch_xor_nbi, false);
    }
  }
}

