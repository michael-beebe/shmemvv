/**
 * @file c_shmem_quiet.c
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
  log_info("Testing shmem_quiet functionality between PEs");

  long *flag = (long *)shmem_malloc(sizeof(long));
  if (flag == NULL) {
    log_fail("Memory allocation failed: shmem_malloc returned NULL pointer");
    return false;
  }
  log_info("Allocated %zu bytes for flag variable at address %p", sizeof(long),
           (void *)flag);

  *flag = 0;
  log_info("Initialized flag at %p to 0", (void *)flag);

  int mype = shmem_my_pe();
  log_info("Current PE: %d", mype);

  log_info("Entering barrier before communication");
  shmem_barrier_all();

  if (mype == 0) {
    log_info("PE 0: Putting value 1 to flag on PE 1");
    shmem_long_p(flag, 1, 1);
    log_info("PE 0: Calling shmem_quiet to ensure completion of put operation");
    shmem_quiet();
  }

  log_info("Entering barrier after communication");
  shmem_barrier_all();

  bool result = true;
  if (mype == 1) {
    log_info("PE 1: Validating received value");
    if (*flag != 1) {
      log_fail("Validation failed: Expected flag value 1, got %ld", *flag);
      result = false;
    } else {
      log_info("Validation successful: Flag value is 1 as expected");
    }
  } else {
    log_info("PE %d: Waiting while PE 1 validates result", mype);
  }

  log_info("Freeing allocated memory at %p", (void *)flag);
  shmem_free(flag);

  log_info("Test completed with %s", result ? "success" : "failure");
  return result;
}

int main(int argc, char **argv) {
  shmem_init();
  log_init(__FILE__);

  bool result = test_shmem_quiet();
  int rc = EXIT_SUCCESS;

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_quiet()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
