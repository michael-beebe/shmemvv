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

  log_info("Testing shmem_align with alignment=%zu bytes, size=%zu bytes",
           alignment, size);
  log_info("Attempting to allocate %zu bytes of memory aligned to %zu byte "
           "boundary...",
           size, alignment);

  void *ptr = shmem_align(alignment, size);
  if (ptr == NULL) {
    log_fail("shmem_align returned NULL pointer - allocation failed!");
    return false;
  }

  // Verify alignment
  uintptr_t addr = (uintptr_t)ptr;
  if (addr % alignment != 0) {
    log_fail("Memory at %p is not properly aligned to %zu bytes!", ptr,
             alignment);
    shmem_free(ptr);
    return false;
  }

  log_info("Successfully allocated aligned memory at address %p", ptr);
  log_info("Verifying alignment: address %p mod %zu = %zu (expected 0)", ptr,
           alignment, addr % alignment);
  log_info("Freeing aligned memory...");

  shmem_free(ptr);
  log_info("Memory freed successfully");
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
