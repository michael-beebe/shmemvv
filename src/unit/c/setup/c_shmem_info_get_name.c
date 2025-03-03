/**
 * @file c_shmem_info_get_name.c
 * @brief Unit test for shmem_info_get_name() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "shmemvv.h"

char *test_shmem_info_get_name(void) {
  log_routine("shmem_info_get_name()");
  log_info("Allocating buffer of size %d bytes", SHMEM_MAX_NAME_LEN);

  char *name = (char *)malloc(SHMEM_MAX_NAME_LEN * sizeof(char));
  if (name == NULL) {
    log_info("Failed to allocate memory for name buffer");
    return NULL;
  }

  log_info("Successfully allocated name buffer at %p", (void *)name);
  log_info("Calling shmem_info_get_name() to get implementation name");
  shmem_info_get_name(name);
  log_info("Implementation name: %s", name);

  return name;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  log_info("Starting test for shmem_info_get_name()");
  char *name = test_shmem_info_get_name();
  int result = true;
  int rc = EXIT_SUCCESS;

  if (name == NULL) {
    log_info("Memory allocation failed in test_shmem_info_get_name()");
    rc = EXIT_FAILURE;
    result = false;
  } else {
    log_info("Successfully retrieved implementation name");
    free(name);
  }

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_info_get_name", result, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}