/**
 * @file c11_shmem_malloc_with_hints.c
 * @brief Unit test for shmem_malloc_with_hints
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

bool test_shmem_malloc_with_hints(void) {
  log_routine("shmem_malloc_with_hints()");
  size_t size = 1024;
  long hints = SHMEM_MALLOC_ATOMICS_REMOTE;
  void *ptr = shmem_malloc_with_hints(size, hints);
  log_info("shmem_malloc'd %ld bytes @ %p with ATOMICS_REMOTE hint", (long)size,
           (void *)ptr);
  if (ptr == NULL) {
    log_fail("shmem_malloc_with_hints ret'd null ptr!");
    return false;
  }
  log_info("shmem_malloc_with_hints ret'd valid ptr");
  shmem_free(ptr);
  return true;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = test_shmem_malloc_with_hints();
  int rc = EXIT_SUCCESS;

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_malloc_with_hints()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
