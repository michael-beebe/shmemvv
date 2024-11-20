/**
 * @file c11_shmem_query_thread.c
 * @brief Unit test for the shmem_query_thread() routine.
 */

#include <shmem.h>

#include "log.h"
#include "shmemvv.h"

bool test_shmem_query_thread(void) {
  log_routine("shmem_query_thread()");
  
  int provided;
  log_info("Calling shmem_query_thread()");
  shmem_query_thread(&provided);
  log_info("Thread level returned: %d", provided);

  const char *level_str;
  switch(provided) {
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

  bool success =
      (provided == SHMEM_THREAD_SINGLE || provided == SHMEM_THREAD_FUNNELED ||
       provided == SHMEM_THREAD_SERIALIZED ||
       provided == SHMEM_THREAD_MULTIPLE);

  if (!success) {
    log_fail("Invalid thread level returned: %d (%s)", provided, level_str);
    log_fail("Expected one of: SHMEM_THREAD_MULTIPLE (%d), "
             "SHMEM_THREAD_FUNNELED (%d), SHMEM_THREAD_SERIALIZED (%d) "
             "or SHMEM_THREAD_SINGLE (%d)",
             SHMEM_THREAD_MULTIPLE, SHMEM_THREAD_FUNNELED,
             SHMEM_THREAD_SERIALIZED, SHMEM_THREAD_SINGLE);
  } else {
    log_info("Valid thread level returned: %s (%d)", level_str, provided);
  }

  return success;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  bool result = test_shmem_query_thread();
  int rc = result ? EXIT_SUCCESS : EXIT_FAILURE;

  if (mype == 0) {
    display_test_result("C11 shmem_query_thread()", result, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
