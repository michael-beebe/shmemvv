/**
 * @file cxx_shmem_query_thread.cpp
 * @brief Unit test for the shmem_query_thread() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

bool test_shmem_query_thread(void) {
  int provided;
  log_routine("shmem_query_thread()");

  log_info("Querying thread support level");
  shmem_query_thread(&provided);

  log_info("Received thread support level: %d", provided);

  bool success =
      (provided == SHMEM_THREAD_SINGLE || provided == SHMEM_THREAD_FUNNELED ||
       provided == SHMEM_THREAD_SERIALIZED ||
       provided == SHMEM_THREAD_MULTIPLE);

  if (success) {
    log_info("Received valid thread support level");
  } else {
    log_fail("Received invalid thread support level: %d", provided);
  }

  return success;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  log_info("Running on PE %d of %d total PEs", mype, npes);

  bool result = test_shmem_query_thread();
  int rc = result ? EXIT_SUCCESS : EXIT_FAILURE;

  if (mype == 0) {
    display_test_result("CXX shmem_query_thread()", result, false);
  }

  log_info("Test completed with %s", result ? "SUCCESS" : "FAILURE");
  log_close(rc);
  shmem_finalize();
  return rc;
}
