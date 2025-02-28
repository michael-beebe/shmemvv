/**
 * @file c_shmme_fence.cpp
 * @brief Unit test for the shmem_fence routine.
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"
#include "log.h"

bool test_shmem_fence(void) {
  log_routine("shmem_fence()");
  long *flag = (long *)shmem_malloc(sizeof(long));
  *flag = 0;
  int mype = shmem_my_pe();

  log_info("allocated flag at %p, initialized to 0", (void *)flag);

  shmem_barrier_all();

  if (mype == 0) {
    log_info("PE 0: putting value 1 to PE 1");
    shmem_long_p(flag, 1, 1);
    log_info("PE 0: calling shmem_fence");
    shmem_fence();
    log_info("PE 0: setting local flag to 2");
    *flag = 2;
  }

  shmem_barrier_all();

  bool result = true;
  if (mype == 1) {
    log_info("PE 1: checking flag value");
    if (*flag != 1) {
      log_fail("PE 1: flag value is %ld, expected 1", *flag);
      result = false;
    } else {
      log_info("PE 1: flag value is correct (1)");
    }
  }

  log_info("freeing flag");
  shmem_free(flag);
  return result;
}

int main(int argc, char **argv) {
  shmem_init();
  log_init(__FILE__);

  bool result = test_shmem_fence();
  int rc = EXIT_SUCCESS;

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_fence()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
