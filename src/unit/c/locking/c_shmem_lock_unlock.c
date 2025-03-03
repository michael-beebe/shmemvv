/**
 * @file c_shmem_lock_unlock.c
 * @brief Unit test for the shmem_lock_unlock() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

bool test_c_shmem_lock_unlock(void) {
  log_routine("shmem_set/clear_lock()");
  long *lock = (long *)shmem_malloc(sizeof(long));
  long *value = (long *)shmem_malloc(sizeof(long));
  *lock = 0;
  log_info("created lock @ %p", (void *)lock);
  int mype = shmem_my_pe();
  bool result = true;

  shmem_barrier_all();

  if (mype == 0) {
    log_info("acquiring lock");
    shmem_set_lock(lock);
    log_info("acquired lock");
    *value = 1;
    log_info("clearing lock");
    shmem_clear_lock(lock);
    log_info("cleared lock");
  }

  shmem_barrier_all();

  if (mype == 1) {
    log_info("acquiring lock after pe 0 changes");
    shmem_set_lock(lock);
    log_info("acquired lock");
    if (*value != 1) {
      log_fail("didn't observe expected value. expected 1, got %d", *value);
      result = false;
    }
    log_info("clearing lock");
    shmem_clear_lock(lock);
    log_info("cleared lock");
  }

  shmem_barrier_all();

  shmem_free(lock);
  shmem_free(value);
  return result;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

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
    display_test_result("C shmem_lock_lock", result, false);
    display_test_result("C shmem_lock_unlock", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return EXIT_SUCCESS;
}
