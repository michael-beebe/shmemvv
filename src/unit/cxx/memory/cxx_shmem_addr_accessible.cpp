/**
 * @file c_shmem_addr_accessible.cpp
 * @brief Unit test for shmem_addr_accessible
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"

bool test_shmem_addr_accessible() {
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
    if (shmem_addr_accessible(ptr, pe) != 1) {
      test_passed = false;
    }
  }

  shmem_free(ptr);
  return test_passed;
}

int main(int argc, char *argv[]) {
  shmem_init();

  bool result = test_shmem_addr_accessible();
  int rc = EXIT_SUCCESS;

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_addr_accessible()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return rc;
}
