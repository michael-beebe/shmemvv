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
  log_info("Testing symmetric heap allocation with hints");

  size_t size = 1024;
  long hints = SHMEM_MALLOC_ATOMICS_REMOTE;

  log_info("Attempting to allocate %zu bytes with SHMEM_MALLOC_ATOMICS_REMOTE "
           "hint...",
           size);
  void *ptr = shmem_malloc_with_hints(size, hints);

  if (ptr == NULL) {
    log_fail("Memory allocation failed: shmem_malloc_with_hints returned NULL "
             "pointer");
    return false;
  }

  log_info("Successfully allocated %zu bytes at address %p", size, (void *)ptr);
  log_info("Verifying allocation is accessible...");

  *((char *)ptr) = 1;
  log_info("Memory write test successful");

  log_info("Deallocating memory at address %p using shmem_free...",
           (void *)ptr);
  shmem_free(ptr);
  log_info("Memory successfully deallocated");

  return true;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = test_shmem_malloc_with_hints();
  int rc = EXIT_SUCCESS;

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_malloc_with_hints", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
