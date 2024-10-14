/**
 * @file c_shmem_n_pes.cpp
 * @brief Unit test for shmem_n_pes() routine.
 */

#include <shmem.h>
#include <shmemvv.h>
#include <stdlib.h>

#include "shmemvv.h"

bool test_shmem_n_pes(void) {
  int npes = shmem_n_pes();
  if (!(npes > 0)) {
    return false;
  }
  return true;
}

int main(int argc, char *argv[]) {
  shmem_init();

  bool result = test_shmem_n_pes();
  int rc = EXIT_SUCCESS;

  if (!result) {
    rc = EXIT_FAILURE;
  }

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_n_pes()", result, false);
  }

  shmem_finalize();
  return rc;
}
