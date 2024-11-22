/**
 * @file cxx_shmem_init_thread.cpp
 * @brief Unit test for the shmem_init_thread() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

bool test_shmem_init_thread(void) {
  int provided;
  log_routine("shmem_init_thread()");

  log_info("Requesting SHMEM_THREAD_MULTIPLE support");
  shmem_init_thread(SHMEM_THREAD_MULTIPLE, &provided);

  log_info("Received thread support level: %d", provided);

  bool result = (provided == SHMEM_THREAD_MULTIPLE ||
                provided == SHMEM_THREAD_FUNNELED ||
                provided == SHMEM_THREAD_SERIALIZED);

  if (result) {
    log_info("Received valid thread support level");
  } else {
    log_fail("Received invalid thread support level: %d", provided);
  }

  return result;
}

int main(int argc, char *argv[]) {
  log_init(__FILE__);

  bool result = test_shmem_init_thread();
  int rc = result ? EXIT_SUCCESS : EXIT_FAILURE;

  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  log_info("Running on PE %d of %d total PEs", mype, npes);

  if (mype == 0) {
    display_test_result("CXX shmem_init_thread()", result, false);
  }

  log_info("Test completed with %s", result ? "SUCCESS" : "FAILURE");
  log_close(rc);
  shmem_finalize();
  return rc;
}
