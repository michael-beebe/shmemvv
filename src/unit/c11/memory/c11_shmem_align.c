/**
 * @file c11_shmem_align.c
 * @brief Unit test for shmem_align
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

bool test_shmem_align(void) {
  log_routine("shmem_align()");
  size_t alignment = 64;
  size_t size = 1024;

  log_info("calling shmem_align(align = %ld, size = %ld)...", (long)alignment,
           (long)size);
  void *ptr = shmem_align(alignment, size);
  if (ptr == NULL) {
    log_fail("shmem_align ret'd null ptr!");
    return false;
  }
  log_info("shmem_align ret'd valid ptr (%p)", ptr);
  shmem_free(ptr);
  return true;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = test_shmem_align();
  int rc = EXIT_SUCCESS;

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_align()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
