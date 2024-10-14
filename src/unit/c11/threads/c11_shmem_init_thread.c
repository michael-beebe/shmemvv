/**
 * @file c11_shmem_init_thread.c
 * @brief Unit test for the shmem_init_thread() routine.
 */

#include <shmem.h>

#include "shmemvv.h"

bool test_shmem_init_thread(void) {
  int provided;
  shmem_init_thread(SHMEM_THREAD_MULTIPLE, &provided);
  return (provided == SHMEM_THREAD_MULTIPLE ||
          provided == SHMEM_THREAD_FUNNELED ||
          provided == SHMEM_THREAD_SERIALIZED);
}

int main(int argc, char *argv[]) {
  bool result = test_shmem_init_thread();
  int rc = result ? EXIT_SUCCESS : EXIT_FAILURE;

  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  if (mype == 0) {
    display_test_result("C11 shmem_init_thread()", result, false);
  }

  shmem_finalize();
  return rc;
}
