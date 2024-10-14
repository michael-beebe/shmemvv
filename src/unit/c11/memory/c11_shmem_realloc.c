/**
 * @file c11_shmem_realloc.c
 * @brief Unit test for shmem_realloc
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"

bool test_shmem_realloc(void) {
  size_t size = 1024;
  void *ptr = shmem_malloc(size);
  if (ptr == NULL) {
    return false;
  }
  size_t new_size = 2048;
  void *new_ptr = shmem_realloc(ptr, new_size);
  if (new_ptr == NULL) {
    shmem_free(ptr);
    return false;
  }
  shmem_free(new_ptr);
  return true;
}

int main(int argc, char *argv[]) {
  shmem_init();

  bool result = test_shmem_realloc();
  int rc = EXIT_SUCCESS;

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_realloc()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return rc;
}
