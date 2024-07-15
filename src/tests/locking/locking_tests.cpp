/**
  @file locking_tests.cpp
  @brief Contains OpenSHMEM distributed locking tests.
 */

#include "locking_tests.hpp"

/**
  @brief Tests the shmem_set_lock() and shmem_clear_lock() routines.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_lock_unlock(void) {
  long *lock = (long *)p_shmem_malloc(sizeof(long));
  *lock = 0;
  int mype = p_shmem_my_pe();
  bool result = true;

  p_shmem_barrier_all();

  if (mype == 0) {
    p_shmem_set_lock(lock);
    *lock = 1; /* Set a flag while holding the lock */
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
