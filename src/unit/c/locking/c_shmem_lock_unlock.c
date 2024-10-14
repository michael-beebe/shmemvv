/**
 * @file c_shmem_lock_unlock.c
 * @brief Unit test for the shmem_lock_unlock() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"

bool test_c_shmem_lock_unlock(void) {
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

int main(int argc, char *argv[]) {
  shmem_init();

  if (!(shmem_n_pes() > 1)) {
    if (shmem_my_pe() == 0) {
      display_not_enough_pes("DISTRIBUTED LOCKING");
    }
  }

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= test_c_shmem_lock_unlock();

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_lock_lock()", result, false);
    display_test_result("C shmem_lock_unlock()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return EXIT_SUCCESS;
}
