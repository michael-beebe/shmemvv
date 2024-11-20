/**
 * @file c11_shmem_quiet.c
 * @brief Unit test for the shmem_quiet routine.
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

bool test_shmem_quiet(void) {
  log_routine("shmem_quiet()");
  long *flag = (long *)shmem_malloc(sizeof(long));
  log_info("shmem_malloc'd %d bytes @ %p", sizeof(long), (void *)flag);
  *flag = 0;
  log_info("set %p to 0", (void *)flag);
  int mype = shmem_my_pe();

  shmem_barrier_all();

  if (mype == 0) {
    shmem_long_p(flag, 1, 1);
    log_info("shmem_long_p'd %p to 1", (void *)flag);
    shmem_quiet();
  }

  shmem_barrier_all();

  bool result = true;
  if (mype == 1) {
    log_info("validating result...");
    if (*flag != 1) {
      log_fail("got unexpected value in flag: expected 1, got %ld", *flag);
      result = false;
    } else {
      log_info("result is valid!");
    }
  } else {
    log_info("validation is done by pe 1, twiddling thumbs");
  }

  shmem_free(flag);
  return result;
}

int main(int argc, char **argv) {
  shmem_init();
  log_init(__FILE__);

  bool result = test_shmem_quiet();
  int rc = EXIT_SUCCESS;

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_quiet()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
