/**
 * @file c_shmem_pe_accessible.c
 * @brief Unit test for shmem_pe_accessible() routine.
 */

#include <shmem.h>
#include <shmemvv.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

bool test_shmem_pe_accessible(void) {
  log_routine("shmem_pe_accessible()");
  log_info("Testing accessibility of current PE");

  int pe = shmem_my_pe();
  log_info("Current PE number is %d", pe);

  log_info("Checking if PE %d is accessible via shmem_pe_accessible()", pe);
  if (!shmem_pe_accessible(pe)) {
    log_fail(
        "PE accessibility check failed - PE %d is not accessible to itself",
        pe);
    return false;
  }

  log_info("PE accessibility check successful - PE %d is accessible to itself",
           pe);
  return true;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  log_info("Starting test for shmem_pe_accessible()");
  bool result = test_shmem_pe_accessible();
  int rc = EXIT_SUCCESS;

  if (!result) {
    log_info("shmem_pe_accessible() test failed");
    rc = EXIT_FAILURE;
  } else {
    log_info("shmem_pe_accessible() test completed successfully");
  }

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_pe_accessible()", result, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
