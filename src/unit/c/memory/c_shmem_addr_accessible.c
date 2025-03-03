/**
 * @file c_shmem_addr_accessible.c
 * @brief Unit test for shmem_addr_accessible
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

bool test_shmem_addr_accessible() {
  log_routine("shmem_addr_accessible()");
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();
  int *ptr = (int *)shmem_malloc(sizeof(int));
  log_info("shmem_malloc'd %d bytes @ %p", sizeof(int), (void *)ptr);

  if (ptr == NULL) {
    log_fail("shmem_malloc ret'd null!");
    return false;
  }

  log_info("set %p to %d", (void *)ptr, (char)shmem_my_pe());
  *ptr = mype;

  shmem_barrier_all();

  bool test_passed = true;

  log_info("validating...");
  for (int pe = 0; pe < npes; ++pe) {
    if (shmem_addr_accessible(ptr, pe) != 1) {
      log_fail(
          "failed: expected shmem_addr_accessible(%p on pe %d) to be true!",
          ptr, pe);
      test_passed = false;
    }
  }

  if (test_passed)
    log_info("shmem_addr_accessible produced expected result.");
  else
    log_fail("at least one result was unexpected in validation of "
             "shmem_addr_accessible");
  shmem_free(ptr);
  return test_passed;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = test_shmem_addr_accessible();
  int rc = EXIT_SUCCESS;

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_addr_accessible", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
