/**
 * @file c11_shmem_calloc.c
 * @brief Unit test for shmem_calloc
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

bool test_shmem_calloc(void) {
  log_routine("shmem_calloc()");
  size_t count = 256;
  size_t size = sizeof(int);
  log_info("calling shmem_calloc(count = %ld, size = %d)", (long)count,
           (char)size);
  int *ptr = (int *)shmem_calloc(count, size);
  if (ptr == NULL) {
    log_fail("shmem_calloc ret'd null ptr!");
    return false;
  }
  log_info("validating result...");
  for (size_t i = 0; i < count; ++i) {
    if (ptr[i] != 0) {
      log_fail("idx %d (%p) was not zeroed by shmem_calloc!", i,
               (void *)&ptr[i]);
      shmem_free(ptr);
      return false;
    }
  }
  log_info("result validated");
  shmem_free(ptr);
  return true;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = test_shmem_calloc();
  int rc = EXIT_SUCCESS;

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_calloc()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
