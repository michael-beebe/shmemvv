/**
 * @file c_shmem_ptr.c
 * @brief Unit test for shmem_ptr
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

bool test_shmem_ptr() {
  log_routine("shmem_ptr()");
  log_info("Testing shmem_ptr functionality between PEs");

  int mype = shmem_my_pe();
  int npes = shmem_n_pes();
  log_info("Current PE: %d, Total PEs: %d", mype, npes);

  int *ptr = (int *)shmem_malloc(sizeof(int));
  log_info("Allocated %zu bytes for test variable at address %p", sizeof(int),
           (void *)ptr);

  if (ptr == NULL) {
    log_fail("Memory allocation failed: shmem_malloc returned NULL pointer");
    return false;
  }

  *ptr = mype;
  log_info("Initialized value at %p to PE number %d", (void *)ptr, mype);

  log_info("Entering barrier before pointer tests");
  shmem_barrier_all();

  bool test_passed = true;

  for (int pe = 0; pe < npes; ++pe) {
    log_info("Testing shmem_ptr for PE %d", pe);
    int *remote_ptr = (int *)shmem_ptr(ptr, pe);
    log_info("shmem_ptr(%p, %d) returned %p", (void *)ptr, pe,
             (void *)remote_ptr);

    if (remote_ptr != NULL) {
      log_info("Validating data through remote pointer on PE %d", pe);
      int remote_val = *remote_ptr;
      if (remote_val != pe) {
        log_fail("Data validation failed on PE %d: Expected %d, got %d at "
                 "address %p",
                 pe, pe, remote_val, (void *)remote_ptr);
        test_passed = false;
      } else {
        log_info("Successfully validated data on PE %d: value %d at address %p",
                 pe, remote_val, (void *)remote_ptr);
      }
    } else if (pe == mype) {
      log_fail("shmem_ptr failed to return valid pointer for local PE %d",
               mype);
      test_passed = false;
    } else {
      log_warn("shmem_ptr returned NULL for remote PE %d - Implementation may "
               "not support remote pointers",
               pe);
    }
  }

  if (test_passed) {
    log_info("All shmem_ptr tests completed successfully");
  } else {
    log_fail("shmem_ptr validation failed on one or more PEs");
  }

  log_info("Freeing allocated memory at %p", (void *)ptr);
  shmem_free(ptr);
  return test_passed;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = test_shmem_ptr();
  int rc = EXIT_SUCCESS;

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_ptr", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
