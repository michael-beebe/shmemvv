/**
 * @file c_shmem_init_thread.c
 * @brief Unit test for the shmem_init_thread() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

bool test_shmem_init_thread(void) {
  int provided = -1;
  int status = shmem_init_thread(SHMEM_THREAD_MULTIPLE, &provided);
  if (status != 0) {
    fprintf(stderr, "shmem_init_thread failed: %d\n", status);
    exit(EXIT_FAILURE);
  }

  // Now OpenSHMEM is initialized, we can set up logging
  log_init(__FILE__);
  log_routine("shmem_init_thread()");
  log_info("Requested thread level SHMEM_THREAD_MULTIPLE");
  log_info("Checking returned thread level: %d", provided);

  const char *level_str;
  switch (provided) {
  case SHMEM_THREAD_MULTIPLE:
    level_str = "SHMEM_THREAD_MULTIPLE";
    break;
  case SHMEM_THREAD_FUNNELED:
    level_str = "SHMEM_THREAD_FUNNELED";
    break;
  case SHMEM_THREAD_SERIALIZED:
    level_str = "SHMEM_THREAD_SERIALIZED";
    break;
  case SHMEM_THREAD_SINGLE:
    level_str = "SHMEM_THREAD_SINGLE";
    break;
  default:
    level_str = "INVALID";
  }

  bool is_valid = provided == SHMEM_THREAD_MULTIPLE ||
                  provided == SHMEM_THREAD_FUNNELED ||
                  provided == SHMEM_THREAD_SERIALIZED ||
                  provided == SHMEM_THREAD_SINGLE;

  if (!is_valid) {
    log_fail("Invalid thread level returned: %d (%s)", provided, level_str);
    log_fail("Expected one of: SHMEM_THREAD_MULTIPLE (%d), "
             "SHMEM_THREAD_FUNNELED (%d), SHMEM_THREAD_SERIALIZED (%d), "
             "or SHMEM_THREAD_SINGLE (%d)",
             SHMEM_THREAD_MULTIPLE, SHMEM_THREAD_FUNNELED,
             SHMEM_THREAD_SERIALIZED, SHMEM_THREAD_SINGLE);
  } else {
    log_info("Valid thread level returned: %s (%d)", level_str, provided);
  }
  int queried = -1;
  shmem_query_thread(&queried);
  if (queried != provided) {
    log_fail("shmem_query_thread returned %d, but initialization provided %d",
             queried, provided);
    return false;
  }

  return is_valid;
}

int main(int argc, char *argv[]) {
  bool result = test_shmem_init_thread();

  int rc = result ? EXIT_SUCCESS : EXIT_FAILURE;

  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  if (mype == 0) {
    display_test_result("C shmem_init_thread", result, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
