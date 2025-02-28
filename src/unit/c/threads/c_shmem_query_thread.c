/**
 * @file c_shmem_query_thread.c
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
  bool success =
      (provided == SHMEM_THREAD_SINGLE || provided == SHMEM_THREAD_FUNNELED ||
       provided == SHMEM_THREAD_SERIALIZED ||
       provided == SHMEM_THREAD_MULTIPLE);
  if (!success)
    log_fail("expected shmem_query_thread to return either "
             "(SHMEM_THREAD_MULTIPLE) %d, (SHMEM_THREAD_FUNNELED) %d, or "
             "(SHMEM_THREAD_SERIALIZED) %d\n but got %d",
             SHMEM_THREAD_MULTIPLE, SHMEM_THREAD_FUNNELED,
             SHMEM_THREAD_SERIALIZED, provided);
  else
    log_info("thread query is valid (%d)", provided);
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
    display_test_result("C shmem_query_thread()", result, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
