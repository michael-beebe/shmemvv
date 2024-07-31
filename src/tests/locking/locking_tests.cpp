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
  long *lock = (long *)p_shmem_malloc(sizeof(long));
  *lock = 0;
  int mype = p_shmem_my_pe();
  bool result = true;

  p_shmem_barrier_all();

  if (mype == 0) {
    p_shmem_set_lock(lock);
    *lock = 1;
    p_shmem_clear_lock(lock);
  }

  p_shmem_barrier_all();

  if (mype == 1) {
    p_shmem_set_lock(lock);
    if (*lock != 1) {
      result = false;
    }
    p_shmem_clear_lock(lock);
  }

  p_shmem_barrier_all();

  p_shmem_free(lock);
  return result;
}

/**
 * TODO: write docs
 * 
 */
void run_locking_tests(int mype, int npes) {
  shmem_barrier_all();
  if (mype == 0) {
    display_test_header("DISTRIBUTED LOCKING");
  }
  shmem_barrier_all();

  /* Make sure there are at least 2 PEs */
  if ( !(npes > 1) ) {
    if (mype == 0) {
      display_not_enough_pes("DISTRIBUTED LOCKING");
    }
  }
  else {
    /* Run the shmem_set_lock and shmem_clear_lock tests */
    shmem_barrier_all();
    if ( !check_if_exists("shmem_set_lock") ) {
      if (mype == 0) {
        display_not_found_warning("shmem_set_lock()", false);
      }
    }
    if ( !check_if_exists("shmem_clear_lock" )) {
      if (mype == 0) {
        display_not_found_warning("shmem_clear_lock()", false);
      }
    }
    shmem_barrier_all();
    if ( check_if_exists("shmem_set_lock") && check_if_exists("shmem_clear_lock") ) {
      bool result_shmem_lock_unlock = test_shmem_lock_unlock();
      shmem_barrier_all();
      if (mype == 0) {
        display_test_result("shmem_set_lock()", result_shmem_lock_unlock, false);
        display_test_result("shmem_clear_lock()", result_shmem_lock_unlock, false);
      }
    }
  }
}
