/**
 * @file c11_shmem_calloc.c
 * @brief Unit test for shmem_calloc
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"

bool test_shmem_calloc(void) {
  size_t count = 256;
  size_t size = sizeof(int);
  int *ptr = (int *)shmem_calloc(count, size);
  if (ptr == NULL) {
    return false;
  }
  for (size_t i = 0; i < count; ++i) {
    if (ptr[i] != 0) {
      shmem_free(ptr);
      return false;
    }
  }
  shmem_free(ptr);
  return true;
}

int main(int argc, char *argv[]) {
  shmem_init();

  bool result = test_shmem_calloc();
  int rc = EXIT_SUCCESS;

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_calloc()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return rc;
}
