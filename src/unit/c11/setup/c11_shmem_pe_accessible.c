/**
 * @file c11_shmem_pe_accessible.c
 * @brief Unit test for shmem_pe_accessible() routine.
 */

#include <shmem.h>
#include <shmemvv.h>
#include <stdlib.h>

#include "shmemvv.h"

bool test_shmem_pe_accessible(void) {
  int pe = shmem_my_pe();
  if (!shmem_pe_accessible(pe)) {
    return false;
  }
  return true;
}

int main(int argc, char *argv[]) {
  shmem_init();

  bool result = test_shmem_pe_accessible();
  int rc = EXIT_SUCCESS;

  if (!result) {
    rc = EXIT_FAILURE;
  }

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_pe_accessible()", result, false);
  }

  shmem_finalize();
  return rc;
}
