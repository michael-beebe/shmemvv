/**
 * @file c11_shmem_malloc_free.c
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
  log_routine("shmem_malloc_free()");
  size_t size = 1024;
  void *ptr = shmem_malloc(size);
  log_info("shmem_malloc'd %ld bytes @ %p", (long)size, (void *)ptr);
  if (ptr == NULL) {
    log_fail("shmem_malloc ret'd null ptr!");
    return false;
  }
  log_info("shmem_free'ing %p...", (void *)ptr);
  shmem_free(ptr);
  log_info("shmem_free'd %p...", (void *)ptr);
  return true;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = test_shmem_malloc_free();
  int rc = EXIT_SUCCESS;

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_malloc()", result, false);
    display_test_result("C11 shmem_free()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
