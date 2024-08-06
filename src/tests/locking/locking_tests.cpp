/**
 * @file cxx_locking_tests.cpp
 * @brief Contains OpenSHMEM distributed locking tests.
 */

#include "locking_tests.h"

/**
 * @brief Tests the shmem_set_lock() and shmem_clear_lock() routines.
 *
 * This test verifies that the shmem_set_lock() and shmem_clear_lock() routines correctly
 * set and clear a distributed lock. It ensures that the lock is properly set by one PE
 * and the state is correctly observed by another PE.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_lock_unlock(void) {
  long *lock = (long *)shmem_malloc(sizeof(long));
  *lock = 0;
  int mype = shmem_my_pe();
  bool result = true;

  shmem_barrier_all();

  if (mype == 0) {
    shmem_set_lock(lock);
    *lock = 1;
    shmem_clear_lock(lock);
  }

  shmem_barrier_all();

  if (mype == 1) {
    shmem_set_lock(lock);
    if (*lock != 1) {
      result = false;
    }
    shmem_clear_lock(lock);
  }

  shmem_barrier_all();

  shmem_free(lock);
  return result;
}

/**
 * @brief Run all locking tests
 */
void run_locking_tests(int mype, int npes) {
  /* Make sure there are at least 2 PEs */
  if ( !(npes > 1) ) {
    if (mype == 0) {
      display_not_enough_pes("DISTRIBUTED LOCKING");
    }
  }
  else {
    /* Run the shmem_set_lock and shmem_clear_lock tests */
    shmem_barrier_all();
    bool result_shmem_lock_unlock = test_shmem_lock_unlock();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("shmem_set_lock()", result_shmem_lock_unlock, false);
      display_test_result("shmem_clear_lock()", result_shmem_lock_unlock, false);
    }
  }
}
