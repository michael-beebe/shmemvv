/**
 * @file c_shmem_quiet.c
 * @brief Unit test for the shmem_quiet routine.
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"

bool test_shmem_quiet(void) {
  long *flag = (long *)shmem_malloc(sizeof(long));
  *flag = 0;
  int mype = shmem_my_pe();

  shmem_barrier_all();

  if (mype == 0) {
    shmem_long_p(flag, 1, 1);
    shmem_quiet();
  }

  shmem_barrier_all();

  bool result = true;
  if (mype == 1) {
    if (*flag != 1) {
      result = false;
    }
  }

  shmem_free(flag);
  return result;
}

int main(int argc, char **argv) {
  shmem_init();

  bool result = test_shmem_quiet();
  int rc = EXIT_SUCCESS;

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_quiet()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return rc;
}
