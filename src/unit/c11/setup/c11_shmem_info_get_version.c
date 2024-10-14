/**
 * @file c11_shmem_info_get_version.c
 * @brief Unit test for shmem_info_get_version() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shmemvv.h"

bool test_shmem_info_get_version(void) {
  int major, minor;
  shmem_info_get_version(&major, &minor);

  if (major != 1 || minor != 5) {
    return false;
  }

  return true;
}

int main(int argc, char *argv[]) {
  shmem_init();

  int rc = EXIT_SUCCESS;
  bool result = true;

  if (!test_shmem_info_get_version()) {
    rc = EXIT_FAILURE;
    result = false;
  }

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_info_get_version()", result, false);
  }

  shmem_finalize();
  return rc;
}
