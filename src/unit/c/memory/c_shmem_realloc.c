/**
 * @file c_shmem_realloc.c
 * @brief Unit test for shmem_realloc
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

bool test_shmem_realloc(void) {
  log_routine("shmem_realloc()");
  log_info("Testing symmetric heap reallocation functionality");

  size_t size = 1024;
  log_info("Attempting initial allocation of %zu bytes", size);
  void *ptr = shmem_malloc(size);
  log_info("Initial allocation: shmem_malloc(%zu) returned %p", size,
           (void *)ptr);

  if (ptr == NULL) {
    log_fail("Initial allocation failed: shmem_malloc returned NULL pointer");
    return false;
  }
  log_info("Successfully allocated initial memory block");

  size_t new_size = 2048;
  log_info("Attempting to reallocate memory from %zu to %zu bytes", size,
           new_size);
  log_info("Calling shmem_realloc(ptr=%p, new_size=%zu)", (void *)ptr,
           new_size);

  void *new_ptr = shmem_realloc(ptr, new_size);
  if (new_ptr == NULL) {
    log_fail("Reallocation failed: shmem_realloc returned NULL pointer");
    log_info("Freeing original allocation at %p", (void *)ptr);
    shmem_free(ptr);
    return false;
  }

  log_info("Reallocation successful: new pointer at %p", (void *)new_ptr);
  log_info("Freeing reallocated memory at %p", (void *)new_ptr);
  shmem_free(new_ptr);

  log_info("Test completed successfully");
  return true;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = test_shmem_realloc();
  int rc = EXIT_SUCCESS;

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_realloc()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
