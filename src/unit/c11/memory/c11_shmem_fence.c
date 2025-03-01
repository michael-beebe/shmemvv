/**
 * @file c11_shmme_fence.c
 * @brief Unit test for the shmem_fence routine.
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

bool test_shmem_fence(void) {
  log_routine("shmem_fence()");
  log_info("Testing shmem_fence ordering semantics between PE 0 and PE 1");

  log_info("Allocating shared memory for flag variable...");
  long *flag = (long *)shmem_malloc(sizeof(long));
  if (flag == NULL) {
    log_fail("Memory allocation failed: shmem_malloc returned NULL pointer");
    return false;
  }
  log_info("Successfully allocated %zu bytes at address %p", sizeof(long),
           (void *)flag);

  *flag = 0;
  log_info("Initialized flag to 0");
  int mype = shmem_my_pe();

  log_info("Synchronizing all PEs before test...");
  shmem_barrier_all();

  if (mype == 0) {
    log_info("PE 0: Performing put operation to PE 1 (flag = 1)");
    shmem_long_p(flag, 1, 1);

    log_info("PE 0: Calling shmem_fence to ensure ordering");
    shmem_fence();

    log_info("PE 0: Setting local flag value to 2");
    *flag = 2;
  }

  log_info("Synchronizing all PEs after operations...");
  shmem_barrier_all();

  bool result = true;
  if (mype == 1) {
    log_info("PE 1: Validating fence ordering semantics");
    if (*flag != 1) {
      log_fail("Fence ordering violation: Expected flag value 1, but got %ld",
               *flag);
      log_fail("This indicates the fence operation did not maintain proper "
               "ordering");
      result = false;
    } else {
      log_info(
          "PE 1: Validation successful - fence maintained proper ordering");
    }
  }

  log_info("Freeing allocated memory at %p", (void *)flag);
  shmem_free(flag);
  return result;
}

int main(int argc, char **argv) {
  shmem_init();
  log_init(__FILE__);

  bool result = test_shmem_fence();
  int rc = EXIT_SUCCESS;

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_fence", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
