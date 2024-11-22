/**
 * @file c_shmem_calloc.cpp
 * @brief Unit test for shmem_calloc
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"
#include "log.h"

bool test_shmem_calloc(void) {
  log_routine("shmem_calloc()");
  size_t count = 256;
  size_t size = sizeof(int);
  
  log_info("attempting to allocate %zu elements of size %zu", count, size);
  int *ptr = (int *)shmem_calloc(count, size);
  
  if (ptr == NULL) {
    log_fail("shmem_calloc returned NULL");
    return false;
  }
  log_info("successfully allocated memory at %p", (void *)ptr);
  
  log_info("validating memory is zeroed...");
  for (size_t i = 0; i < count; ++i) {
    if (ptr[i] != 0) {
      log_fail("memory at index %zu is not zero: %d", i, ptr[i]);
      shmem_free(ptr);
      return false;
    }
  }
  log_info("all memory validated as zero");
  
  log_info("freeing allocated memory");
  shmem_free(ptr);
  return true;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = test_shmem_calloc();
  int rc = EXIT_SUCCESS;

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_calloc()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
