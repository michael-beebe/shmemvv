/**
 * @file cxx_shmem_my_pe.cpp
 * @brief Unit test for shmem_my_pe() routine.
 */

#include <shmem.h>
#include <shmemvv.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

int test_shmem_my_pe() {
  log_routine("shmem_my_pe()");
  int mype = shmem_my_pe();
  log_info("Retrieved PE number: %d", mype);
  if (mype >= 0) {
    log_info("PE number is valid (>= 0)");
    return mype;
  } else {
    log_fail("Invalid PE number: %d", mype);
    return -1;
  }
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  int mype = test_shmem_my_pe();
  bool result = true;
  int rc = EXIT_SUCCESS;

  if (mype == -1) {
    rc = EXIT_FAILURE;
    result = false;
  }

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_my_pe()", result, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
