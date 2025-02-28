/**
 * @file c_shmem_lock_unlock.cpp
 * @brief Unit test for the shmem_lock_unlock() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"
#include "log.h"

bool test_c_shmem_lock_unlock(void) {
  log_routine("shmem_lock_unlock()");

  log_info("allocating lock");
  long *lock = (long *)shmem_malloc(sizeof(long));
  *lock = 0;
  int mype = shmem_my_pe();
  bool result = true;

  shmem_barrier_all();

  if (mype == 0) {
    log_info("PE 0: acquiring lock");
    shmem_set_lock(lock);
    log_info("PE 0: setting lock value to 1");
    *lock = 1;
    log_info("PE 0: releasing lock");
    shmem_clear_lock(lock);
  }

  shmem_barrier_all();

  if (mype == 1) {
    log_info("PE 1: acquiring lock");
    shmem_set_lock(lock);
    if (*lock != 1) {
      log_fail("PE 1: lock value is not 1");
      result = false;
    } else {
      log_info("PE 1: lock value verified as 1");
    }
    log_info("PE 1: releasing lock");
    shmem_clear_lock(lock);
  }

  shmem_barrier_all();

  log_info("freeing lock");
  shmem_free(lock);
  return result;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  if (!(shmem_n_pes() > 1)) {
    if (shmem_my_pe() == 0) {
      display_not_enough_pes("DISTRIBUTED LOCKING");
    }
    log_close(EXIT_SUCCESS);
    shmem_finalize();
    return EXIT_SUCCESS;
  }

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= test_c_shmem_lock_unlock();

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_lock_lock()", result, false);
    display_test_result("CXX shmem_lock_unlock()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return EXIT_SUCCESS;
}
