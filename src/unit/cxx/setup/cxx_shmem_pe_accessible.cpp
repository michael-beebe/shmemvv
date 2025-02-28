/**
 * @file c_shmem_pe_accessible.cpp
 * @brief Unit test for shmem_pe_accessible() routine.
 */

#include <shmem.h>
#include <shmemvv.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

bool test_shmem_pe_accessible(void) {
  log_routine("shmem_pe_accessible()");
  int pe = shmem_my_pe();
  log_info("Testing accessibility of PE %d", pe);
  if (!shmem_pe_accessible(pe)) {
    log_fail("PE %d is not accessible", pe);
    return false;
  }
  log_info("PE %d is accessible", pe);
  return true;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = test_shmem_pe_accessible();
  int rc = EXIT_SUCCESS;

  if (!result) {
    rc = EXIT_FAILURE;
  }

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_pe_accessible()", result, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
