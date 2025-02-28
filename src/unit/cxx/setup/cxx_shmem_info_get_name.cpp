/**
 * @file cxx_shmem_info_get_name.cpp
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
  char *name = (char *)malloc(SHMEM_MAX_NAME_LEN * sizeof(char));
  if (name != NULL) {
    log_info("Allocated name buffer at %p", (void *)name);
    shmem_info_get_name(name);
    log_info("Retrieved vendor name: %s", name);
  } else {
    log_fail("Failed to allocate name buffer");
  }

  return name;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  char *name = test_shmem_info_get_name();
  int result = true;
  int rc = EXIT_SUCCESS;

  if (name == NULL) {
    log_fail("Name buffer is NULL");
    rc = EXIT_FAILURE;
    result = false;
  }

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_info_get_name()", result, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}