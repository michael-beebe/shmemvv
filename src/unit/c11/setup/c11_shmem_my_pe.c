/**
 * @file c11_shmem_my_pe.c
 * @brief Unit test for shmem_my_pe() routine.
 */

#include <shmem.h>
#include <shmemvv.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

int test_shmem_my_pe() {
  log_routine("shmem_my_pe()");
  log_info("Calling shmem_my_pe() to get processing element number");

  int mype = shmem_my_pe();
  log_info("Retrieved PE number: %d", mype);

  log_info("Validating PE number is non-negative");
  if (mype >= 0) {
    log_info("PE number validation successful - got valid PE number %d", mype);
    return mype;
  } else {
    log_fail("PE number validation failed - got invalid negative PE number %d",
             mype);
    return -1;
  }
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  log_info("Starting test for shmem_my_pe()");
  int mype = test_shmem_my_pe();
  bool result = true;
  int rc = EXIT_SUCCESS;

  if (mype == -1) {
    log_info("shmem_my_pe() test failed");
    rc = EXIT_FAILURE;
    result = false;
  } else {
    log_info("shmem_my_pe() test completed successfully");
  }

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_my_pe", result, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
