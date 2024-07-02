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
  long *lock = (long *)shmem_malloc(sizeof(long));
  *lock = 0;
  int mype = shmem_my_pe();
  bool result = true;

  shmem_barrier_all();

  if (mype == 0) {
    shmem_set_lock(lock);
    *lock = 1; /* Set a flag while holding the lock */
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
