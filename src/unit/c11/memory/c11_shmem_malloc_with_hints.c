/**
 * @file c11_shmem_malloc_with_hints.c
 * @brief Unit test for shmem_malloc_with_hints
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"

bool test_shmem_malloc_with_hints(void) {
  size_t size = 1024;
  long hints = SHMEM_MALLOC_ATOMICS_REMOTE;
  void *ptr = shmem_malloc_with_hints(size, hints);
  if (ptr == NULL) {
    return false;
  }
  shmem_free(ptr);
  return true;
}

int main(int argc, char *argv[]) {
  shmem_init();

  bool result = test_shmem_malloc_with_hints();
  int rc = EXIT_SUCCESS;

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_malloc_with_hints()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return rc;
}
