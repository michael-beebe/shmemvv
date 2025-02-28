/**
 * @file c11_shmem_addr_accessible.c
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

  log_info("Allocating memory using shmem_malloc...");
  int *ptr = (int *)shmem_malloc(sizeof(int));
  log_info("shmem_malloc allocated %zu bytes at address %p", sizeof(int),
           (void *)ptr);

  if (ptr == NULL) {
    log_fail("Memory allocation failed: shmem_malloc returned NULL pointer");
    return false;
  }

  log_info("Setting value at address %p to PE number %d", (void *)ptr, mype);
  *ptr = mype;

  log_info("Synchronizing all PEs with shmem_barrier_all...");
  shmem_barrier_all();

  bool test_passed = true;

  log_info(
      "Testing shmem_addr_accessible for allocated memory across all PEs...");
  for (int pe = 0; pe < npes; ++pe) {
    int accessible = shmem_addr_accessible(ptr, pe);
    if (accessible != 1) {
      log_fail("Accessibility test failed: memory at %p is not accessible from "
               "PE %d "
               "(expected: 1, got: %d)",
               (void *)ptr, pe, accessible);
      test_passed = false;
    } else {
      log_info("Memory at %p is accessible from PE %d", (void *)ptr, pe);
    }
  }

  if (test_passed) {
    log_info("All accessibility tests passed successfully");
  } else {
    log_fail("One or more accessibility tests failed");
  }

  log_info("Freeing allocated memory at %p...", (void *)ptr);
  shmem_free(ptr);

  return test_passed;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = test_shmem_addr_accessible();
  int rc = EXIT_SUCCESS;

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_addr_accessible()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
