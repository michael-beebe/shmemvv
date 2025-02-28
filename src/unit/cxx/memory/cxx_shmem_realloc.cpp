/**
 * @file c_shmem_realloc.cpp
 * @brief Unit test for shmem_realloc
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"
#include "log.h"

bool test_shmem_realloc(void) {
  log_routine("shmem_realloc()");
  size_t size = 1024;

  log_info("attempting to allocate %zu bytes", size);
  void *ptr = shmem_malloc(size);
  if (ptr == NULL) {
    log_fail("shmem_malloc returned NULL");
    return false;
  }
  log_info("successfully allocated memory at %p", ptr);

  size_t new_size = 2048;
  log_info("attempting to reallocate to %zu bytes", new_size);
  void *new_ptr = shmem_realloc(ptr, new_size);
  if (new_ptr == NULL) {
    log_fail("shmem_realloc returned NULL");
    shmem_free(ptr);
    return false;
  }
  log_info("successfully reallocated memory at %p", new_ptr);

  log_info("freeing allocated memory");
  shmem_free(new_ptr);
  return true;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = test_shmem_realloc();
  int rc = EXIT_SUCCESS;

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_realloc()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
