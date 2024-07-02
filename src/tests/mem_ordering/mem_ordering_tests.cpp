/**
  @file mem_ordering_tests.cpp
  @brief Contains OpenSHMEM memory ordering tests.
 */

#include "mem_ordering_tests.hpp"

/**
  @brief Tests the shmem_fence() routine.
  @return True if the test is successful, false otherwise.
 */
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

/**
  @brief Tests the shmem_quiet() routine.
  @return True if the test is successful, false otherwise.
 */
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
