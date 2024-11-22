/**
 * @file c_shmem_malloc_free.cpp
 * @brief Unit test for shmem_malloc and shmem_free
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"
#include "log.h"

bool test_shmem_malloc_free(void) {
  log_routine("shmem_malloc() and shmem_free()");
  size_t size = 1024;
  
  log_info("attempting to allocate %zu bytes", size);
  void *ptr = shmem_malloc(size);
  
  if (ptr == NULL) {
    log_fail("shmem_malloc returned NULL");
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

  bool result = test_shmem_malloc_free();
  int rc = EXIT_SUCCESS;

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_malloc()", result, false);
    display_test_result("CXX shmem_free()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
