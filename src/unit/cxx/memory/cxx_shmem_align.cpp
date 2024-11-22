/**
 * @file c_shmem_align.cpp
 * @brief Unit test for shmem_align
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"
#include "log.h"

bool test_shmem_align(void) {
  log_routine("shmem_align()");
  size_t alignment = 64;
  size_t size = 1024;
  
  log_info("attempting to allocate %zu bytes with alignment %zu", size, alignment);
  void *ptr = shmem_align(alignment, size);
  
  if (ptr == NULL) {
    log_fail("shmem_align returned NULL");
    return false;
  }
  log_info("successfully allocated aligned memory at %p", ptr);
  
  log_info("freeing allocated memory");
  shmem_free(ptr);
  return true;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = test_shmem_align();
  int rc = EXIT_SUCCESS;

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_align()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
