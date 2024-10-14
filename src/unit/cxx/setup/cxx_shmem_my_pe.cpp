/**
 * @file c_shmem_my_pe.cpp
 * @brief Unit test for shmem_my_pe() routine.
 */

#include <shmem.h>
#include <shmemvv.h>
#include <stdlib.h>

#include "shmemvv.h"

int test_shmem_my_pe() {
  int mype = shmem_my_pe();
  if (mype >= 0) {
    return mype;
  } else {
    return -1;
  }
}

int main(int argc, char *argv[]) {
  shmem_init();

  int mype = test_shmem_my_pe();
  bool result = true;
  int rc = EXIT_SUCCESS;

  if (mype == -1) {
    rc = EXIT_FAILURE;
    result = false;
  }

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_my_pe()", result, false);
  }

  shmem_finalize();
  return rc;
}
