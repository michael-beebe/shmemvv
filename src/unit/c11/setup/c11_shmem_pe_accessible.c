/**
 * @file c11_shmem_pe_accessible.c
 * @brief Unit test for shmem_pe_accessible() routine.
 */

#include <shmem.h>
#include <shmemvv.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

bool test_shmem_pe_accessible(void) {
  int pe = shmem_my_pe();
  if (!shmem_pe_accessible(pe)) {
    log_fail("my pe is %d, but shmem_pe_accessible(%d) is false!", pe, pe);
    return false;
  }
  log_info("shmem_pe_accessible(self) is true");
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
    display_test_result("C11 shmem_pe_accessible()", result, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
