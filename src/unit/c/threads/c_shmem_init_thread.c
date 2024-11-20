/**
 * @file c_shmem_init_thread.c
 * @brief Unit test for the shmem_init_thread() routine.
 */

#include <shmem.h>

#include "log.h"
#include "shmemvv.h"

bool test_shmem_init_thread(void) {
  log_routine("shmem_init_thread()");
  
  int provided;
  log_info("Requesting thread level SHMEM_THREAD_MULTIPLE");
  shmem_init_thread(SHMEM_THREAD_MULTIPLE, &provided);
  log_init(__FILE__);

  log_info("Checking returned thread level: %d", provided);
  
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
    default:
      level_str = "INVALID";
  }
  
  bool is_valid = provided == SHMEM_THREAD_MULTIPLE ||
                  provided == SHMEM_THREAD_FUNNELED ||
                  provided == SHMEM_THREAD_SERIALIZED;

  if (!is_valid) {
    log_fail("Invalid thread level returned: %d (%s)", provided, level_str);
    log_fail("Expected one of: SHMEM_THREAD_MULTIPLE (%d), "
             "SHMEM_THREAD_FUNNELED (%d), or SHMEM_THREAD_SERIALIZED (%d)",
             SHMEM_THREAD_MULTIPLE, SHMEM_THREAD_FUNNELED,
             SHMEM_THREAD_SERIALIZED);
  } else {
    log_info("Valid thread level returned: %s (%d)", level_str, provided);
  }

  return is_valid;
}

int main(int argc, char *argv[]) {
  log_init(__FILE__);
  bool result = test_shmem_init_thread();

  int rc = result ? EXIT_SUCCESS : EXIT_FAILURE;

  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  if (mype == 0) {
    display_test_result("C shmem_init_thread()", result, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
