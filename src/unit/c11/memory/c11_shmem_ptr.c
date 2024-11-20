/**
 * @file c11_shmem_ptr.c
 * @brief Unit test for shmem_ptr
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

bool test_shmem_ptr() {
  log_routine("shmem_ptr()");
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();
  int *ptr = (int *)shmem_malloc(sizeof(int));
  log_info("shmem_malloc'd %d bytes @ %p", sizeof(int), (void *)ptr);

  if (ptr == NULL) {
    log_fail("shmem_malloc ret'd null!");
    return false;
  }

  *ptr = mype;
  log_info("set %p to %d", (void *)ptr, mype);

  shmem_barrier_all();

  bool test_passed = true;

  for (int pe = 0; pe < npes; ++pe) {
    int *remote_ptr = (int *)shmem_ptr(ptr, pe);
    log_info("got ptr %p on pe %d", (void *)ptr, pe);

    if (remote_ptr != NULL) {
      log_info("shmem_ptr returned valid ptr, verifying data");
      int remote_val = *remote_ptr;
      if (remote_val != pe) {
        log_fail("remote ptr read (%p) produced unexpected value (expected %d, "
                 "got %d)",
                 (void *)remote_ptr, pe, remote_val);
        test_passed = false;
      } else {
        log_fail("remote ptr read (%p @ pe %d) valid", (void *)remote_ptr, pe);
      }
    } else if (pe == mype) {
      log_fail("shmem_ptr on own pe returned null ptr!");
      test_passed = false;
    } else {
      log_warn("ptr was null, but was ptr to remote pe. your openshmem impl "
               "may not support shmem_ptr");
    }
  }

  if (test_passed)
    log_info("shmem_ptr validated");
  else
    log_fail("at least one shmem_ptr call failed validation");
  shmem_free(ptr);
  return test_passed;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = test_shmem_ptr();
  int rc = EXIT_SUCCESS;

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_ptr()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
