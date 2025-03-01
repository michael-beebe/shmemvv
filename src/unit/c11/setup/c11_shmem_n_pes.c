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
  log_routine("shmem_n_pes()");
  log_info("Calling shmem_n_pes() to get number of processing elements");

  int npes = shmem_n_pes();
  log_info("Retrieved number of PEs: %d", npes);

  log_info("Validating number of PEs is positive");
  if (!(npes > 0)) {
    log_fail("Number of PEs validation failed - got invalid value %d", npes);
    return false;
  }
  log_info("Number of PEs validation successful - got valid count %d", npes);
  return true;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  log_info("Starting test for shmem_n_pes()");
  bool result = test_shmem_n_pes();
  int rc = EXIT_SUCCESS;

  if (!result) {
    log_info("shmem_n_pes() test failed");
    rc = EXIT_FAILURE;
  } else {
    log_info("shmem_n_pes() test completed successfully");
  }

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_n_pes", result, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
