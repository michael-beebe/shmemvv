/**
 * @file c_shmem_align.c
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

  log_info("calling shmem_align with alignment=%zu, size=%zu", alignment, size);
  void *ptr = shmem_align(alignment, size);

  if (ptr == NULL) {
    log_fail("shmem_align returned NULL!");
    return false;
  }

  log_info("shmem_align returned %p", ptr);
  log_info("freeing aligned memory");
  shmem_free(ptr);

  log_info("shmem_align test completed successfully");
  return true;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = test_shmem_align();
  int rc = EXIT_SUCCESS;

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_align()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
