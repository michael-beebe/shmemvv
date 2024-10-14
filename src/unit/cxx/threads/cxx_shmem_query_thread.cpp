/**
 * @file c_shmem_query_thread.cpp
 * @brief Unit test for the shmem_query_thread() routine.
 */

#include <shmem.h>

#include "shmemvv.h"

bool test_shmem_query_thread(void) {
  int provided;
  shmem_query_thread(&provided);
  bool success =
      (provided == SHMEM_THREAD_SINGLE || provided == SHMEM_THREAD_FUNNELED ||
       provided == SHMEM_THREAD_SERIALIZED ||
       provided == SHMEM_THREAD_MULTIPLE);
  return success;
}

int main(int argc, char *argv[]) {
  shmem_init();

  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  bool result = test_shmem_query_thread();
  int rc = result ? EXIT_SUCCESS : EXIT_FAILURE;

  if (mype == 0) {
    display_test_result("CXX shmem_query_thread()", result, false);
  }

  shmem_finalize();
  return rc;
}
