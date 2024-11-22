/**
 * @file cxx_shmem_n_pes.cpp
 * @brief Unit test for shmem_n_pes() routine.
 */

#include <shmem.h>
#include <shmemvv.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

bool test_shmem_n_pes(void) {
  log_routine("shmem_n_pes()");
  int npes = shmem_n_pes();
  log_info("Retrieved number of PEs: %d", npes);
  if (!(npes > 0)) {
    log_fail("Invalid number of PEs: %d", npes);
    return false;
  }
  log_info("Number of PEs is valid (> 0)");
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
    display_test_result("CXX shmem_n_pes()", result, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
