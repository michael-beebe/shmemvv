/**
 * @file c11_shmem_malloc_free.c
 * @brief Unit test for shmem_malloc and shmem_free
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"

bool test_shmem_malloc_free(void) {
  size_t size = 1024;
  void *ptr = shmem_malloc(size);
  if (ptr == NULL) {
    return false;
  }
  shmem_free(ptr);
  return true;
}

int main(int argc, char *argv[]) {
  shmem_init();

  bool result = test_shmem_malloc_free();
  int rc = EXIT_SUCCESS;

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_malloc()", result, false);
    display_test_result("C11 shmem_free()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return rc;
}
