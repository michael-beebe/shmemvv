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
  log_info("calling shmem_info_get_version()");
  shmem_info_get_version(&major, &minor);

  if (major != 1 || minor != 5) {
    log_fail("version mismatch: expected (1, 5), got (%d, %d)", major, minor);
    return false;
  }
  log_fail("version valid");

  return true;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  int rc = EXIT_SUCCESS;
  bool result = true;

  if (!test_shmem_info_get_version()) {
    rc = EXIT_FAILURE;
    result = false;
  }

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_info_get_version()", result, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
