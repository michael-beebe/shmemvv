/**
 * @file c_shmem_malloc_free.c
 * @brief Unit test for shmem_malloc and shmem_free
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

bool test_shmem_malloc_free(void) {
  log_routine("shmem_malloc() and shmem_free()");
  log_info("Testing symmetric heap allocation and deallocation");

  size_t size = 1024;
  log_info("Attempting to allocate %zu bytes using shmem_malloc...", size);

  void *ptr = shmem_malloc(size);
  if (ptr == NULL) {
    log_fail("Memory allocation failed: shmem_malloc returned NULL pointer");
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

  bool result = test_shmem_malloc_free();
  int rc = EXIT_SUCCESS;

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_malloc", result, false);
    display_test_result("C shmem_free", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
