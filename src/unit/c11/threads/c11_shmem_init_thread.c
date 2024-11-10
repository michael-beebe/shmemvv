/**
 * @file c11_shmem_init_thread.c
 * @brief Unit test for the shmem_init_thread() routine.
 */

#include <shmem.h>

#include "log.h"
#include "shmemvv.h"

bool test_shmem_init_thread(void) {
  int provided;
  shmem_init_thread(SHMEM_THREAD_MULTIPLE, &provided);
  log_init(__FILE__);
  log_info("called shmem_init_thread");
  bool is_valid = provided == SHMEM_THREAD_MULTIPLE ||
                  provided == SHMEM_THREAD_FUNNELED ||
                  provided == SHMEM_THREAD_SERIALIZED;
  if (!is_valid) {
    log_fail("expected shmem_init_thread to return either "
             "(SHMEM_THREAD_MULTIPLE) %d, (SHMEM_THREAD_FUNNELED) %d, or "
             "(SHMEM_THREAD_SERIALIZED) %d\n but got %d",
             SHMEM_THREAD_MULTIPLE, SHMEM_THREAD_FUNNELED,
             SHMEM_THREAD_SERIALIZED, provided);
  } else {
    log_info("shmem_init_thread returned valid threading level (%d)", provided);
  }
  return is_valid;
}

int main(int argc, char *argv[]) {
  bool result = test_shmem_init_thread();
  int rc = result ? EXIT_SUCCESS : EXIT_FAILURE;

  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  if (mype == 0) {
    display_test_result("C11 shmem_init_thread()", result, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
