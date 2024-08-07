/**
 * @file cxx_mem_ordering_tests.cpp
 * @brief Contains OpenSHMEM memory ordering tests.
 */

#include "mem_ordering_tests.h"

/****************************************************************/
/**
 * @brief Tests the shmem_fence() routine.
 *
 * This test verifies that the shmem_fence() routine correctly ensures the
 * ordering of memory operations by checking that the memory store operation on
 * one PE is seen by another PE in the correct order.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
bool test_shmem_fence(void) {
  long *flag = (long *)shmem_malloc(sizeof(long));
  *flag = 0;
  int mype = shmem_my_pe();

  shmem_barrier_all();

  if (mype == 0) {
    shmem_long_p(flag, 1, 1);
    shmem_fence();
    *flag = 2;
  }

  shmem_barrier_all();

  bool result = true;
  if (mype == 1) {
    if (*flag != 1) {
      result = false;
    }
  }

  shmem_free(flag);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_quiet() routine.
 *
 * This test verifies that the shmem_quiet() routine correctly ensures the
 * completion of all outstanding memory operations by checking that a memory
 * store operation on one PE is completed before proceeding.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
bool test_shmem_quiet(void) {
  long *flag = (long *)shmem_malloc(sizeof(long));
  *flag = 0;
  int mype = shmem_my_pe();

  shmem_barrier_all();

  if (mype == 0) {
    shmem_long_p(flag, 1, 1);
    shmem_quiet();
  }

  shmem_barrier_all();

  bool result = true;
  if (mype == 1) {
    if (*flag != 1) {
      result = false;
    }
  }

  shmem_free(flag);
  return result;
}

/****************************************************************/
/**
 * @brief Run all memory ordering tests
 */
/****************************************************************/
void run_mem_ordering_tests(int mype, int npes) {
  /* Make sure there are at least 2 PEs */
  if (!(npes > 1)) {
    if (mype == 0) {
      display_not_enough_pes("MEMORY ORDERING");
    }
  } else {
    /* Run the shmem_fence() test */
    shmem_barrier_all();
    bool result_shmem_fence = test_shmem_fence();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("shmem_fence()", result_shmem_fence, false);
    }

    /* Run the shmem_quiet() test */
    shmem_barrier_all();
    bool result_shmem_quiet = test_shmem_quiet();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("shmem_quiet()", result_shmem_quiet, false);
    }
  }
}
