/**
 * @file c11_shmem_realloc.c
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
  size_t size = 1024;
  void *ptr = shmem_malloc(size);
  log_info("shmem_malloc'd %ld bytes @ %p", (long)size, (void *)ptr);
  if (ptr == NULL) {
    log_fail("shmem_malloc ret'd null ptr!");
    return false;
  }
  size_t new_size = 2048;
  log_info("calling shmem_realloc(old = %p, new_size = %ld)", (void *)ptr,
           (long)new_size);
  void *new_ptr = shmem_realloc(ptr, new_size);
  if (new_ptr == NULL) {
    log_fail("shmem_realloc ret'd null ptr!");
    shmem_free(ptr);
    return false;
  }
  log_info("shmem_realloc ret'd valid ptr");
  shmem_free(new_ptr);
  return true;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = test_shmem_realloc();
  int rc = EXIT_SUCCESS;

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_realloc()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
