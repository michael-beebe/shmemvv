/**
 * @file c_shmem_ptr.cpp
 * @brief Unit test for shmem_ptr
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"

bool test_shmem_ptr() {
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();
  int *ptr = (int *)shmem_malloc(sizeof(int));

  if (ptr == NULL) {
    return false;
  }

  *ptr = mype;

  shmem_barrier_all();

  bool test_passed = true;

  for (int pe = 0; pe < npes; ++pe) {
    int *remote_ptr = (int *)shmem_ptr(ptr, pe);

    if (remote_ptr != NULL) {
      int remote_val = *remote_ptr;
      if (remote_val != pe) {
        test_passed = false;
      }
    } else if (pe == mype) {
      test_passed = false;
    }
  }

  shmem_free(ptr);
  return test_passed;
}


int main(int argc, char *argv[]) {
  shmem_init();

  bool result = test_shmem_ptr();
  int rc = EXIT_SUCCESS;

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_ptr()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return rc;
}
