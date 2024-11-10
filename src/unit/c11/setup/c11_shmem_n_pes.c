/**
 * @file c11_shmem_n_pes.c
 * @brief Unit test for shmem_n_pes() routine.
 */

#include <shmem.h>
#include <shmemvv.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

bool test_shmem_n_pes(void) {
  int npes = shmem_n_pes();
  if (!(npes > 0)) {
    log_fail("shmem_n_pes() ret'd 0 or negative! (%d)", npes);
    return false;
  }
  log_info("shmem_n_pes() ret valid (%d)", npes);
  return true;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = test_shmem_n_pes();
  int rc = EXIT_SUCCESS;

  if (!result) {
    rc = EXIT_FAILURE;
  }

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_n_pes()", result, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
