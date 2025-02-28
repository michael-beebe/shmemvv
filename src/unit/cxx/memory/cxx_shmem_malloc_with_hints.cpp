/**
 * @file c_shmem_malloc_with_hints.cpp
 * @brief Unit test for shmem_malloc_with_hints
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"
#include "log.h"

bool test_shmem_malloc_with_hints(void) {
  log_routine("shmem_malloc_with_hints()");
  size_t size = 1024;
  long hints = SHMEM_MALLOC_ATOMICS_REMOTE;

  log_info("attempting to allocate %zu bytes with hints %ld", size, hints);
  void *ptr = shmem_malloc_with_hints(size, hints);

  if (ptr == NULL) {
    log_fail("shmem_malloc_with_hints returned NULL");
    return false;
  }
  log_info("successfully allocated memory at %p", ptr);

  log_info("freeing allocated memory");
  shmem_free(ptr);
  return true;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = test_shmem_malloc_with_hints();
  int rc = EXIT_SUCCESS;

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_malloc_with_hints()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
