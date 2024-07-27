/**
 * @file cxx_mem_ordering_tests.cpp
 * @brief Contains OpenSHMEM memory ordering tests.
 */

#include "cxx_mem_ordering_tests.h"

/**
 * @brief Tests the shmem_fence() routine.
 *
 * This test verifies that the shmem_fence() routine correctly ensures the ordering of memory
 * operations by checking that the memory store operation on one PE is seen by another PE
 * in the correct order.
 *
 * @return True if the test is successful, false otherwise.
 */
bool text_cxx_shmem_fence(void) {
  long *flag = (long *)p_shmem_malloc(sizeof(long));
  *flag = 0;
  int mype = p_shmem_my_pe();

  p_shmem_barrier_all();

  if (mype == 0) {
    p_shmem_long_p(flag, 1, 1);
    p_shmem_fence();
    *flag = 2;
  }

  p_shmem_barrier_all();

  bool result = true;
  if (mype == 1) {
    if (*flag != 1) {
      result = false;
    }
  }

  p_shmem_free(flag);
  return result;
}

/**
 * @brief Tests the shmem_quiet() routine.
 *
 * This test verifies that the shmem_quiet() routine correctly ensures the completion of
 * all outstanding memory operations by checking that a memory store operation on one PE
 * is completed before proceeding.
 *
 * @return True if the test is successful, false otherwise.
 */
bool text_cxx_shmem_quiet(void) {
  long *flag = (long *)p_shmem_malloc(sizeof(long));
  *flag = 0;
  int mype = p_shmem_my_pe();

  p_shmem_barrier_all();

  if (mype == 0) {
    p_shmem_long_p(flag, 1, 1);
    p_shmem_quiet();
  }

  p_shmem_barrier_all();

  bool result = true;
  if (mype == 1) {
    if (*flag != 1) {
      result = false;
    }
  }

  p_shmem_free(flag);
  return result;
}
