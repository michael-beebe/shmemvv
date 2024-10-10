/**
 * @file c_shmem_align.c
 * @brief Unit test for shmem_align
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"

bool test_shmem_align(void) {
  size_t alignment = 64;
  size_t size = 1024;
  void *ptr = shmem_align(alignment, size);
  if (ptr == NULL) {
    return false;
  }
  shmem_free(ptr);
  return true;
}

int main(int argc, char *argv[]) {
  shmem_init();

  bool result = test_shmem_align();
  int rc = EXIT_SUCCESS;

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_align()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return rc;
}
