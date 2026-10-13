/**
 * @file c_shmem_info_get_version.c
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
  if (major != SHMEM_MAJOR_VERSION || minor != SHMEM_MINOR_VERSION) {
    log_fail("Version mismatch: headers specify OpenSHMEM %d.%d, got %d.%d",
             SHMEM_MAJOR_VERSION, SHMEM_MINOR_VERSION, major, minor);
    return false;
  }
  log_info("Version validation successful - OpenSHMEM %d.%d matches headers",
           major, minor);

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
    display_test_result("C shmem_info_get_version", result, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
