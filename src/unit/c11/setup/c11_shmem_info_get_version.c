/**
 * @file c11_shmem_info_get_version.c
 * @brief Unit test for shmem_info_get_version() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "shmemvv.h"

bool test_shmem_info_get_version(void) {
  log_routine("shmem_info_get_version()");
  int major, minor;

  log_info("Calling shmem_info_get_version() to get OpenSHMEM version");
  shmem_info_get_version(&major, &minor);
  log_info("Retrieved version: %d.%d", major, minor);

  log_info("Validating version numbers");
  if (major != 1 || minor != 5) {
    log_fail("Version mismatch: expected OpenSHMEM 1.5, got %d.%d", major,
             minor);
    return false;
  }
  log_info("Version validation successful - OpenSHMEM %d.%d as expected", major,
           minor);

  return true;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  log_info("Starting test for shmem_info_get_version()");
  int rc = EXIT_SUCCESS;
  bool result = true;

  if (!test_shmem_info_get_version()) {
    log_info("shmem_info_get_version() test failed");
    rc = EXIT_FAILURE;
    result = false;
  } else {
    log_info("shmem_info_get_version() test completed successfully");
  }

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_info_get_version()", result, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
