/**
 * @file c_shmem_info_get_name.c
 * @brief Unit test for shmem_info_get_name() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shmemvv.h"

char *test_shmem_info_get_name(void) {
  char *name = (char *)malloc(SHMEM_MAX_NAME_LEN * sizeof(char));
  if (name != NULL) {
    shmem_info_get_name(name);
  }

  return name;
}

int main(int argc, char *argv[]) {
  shmem_init();

  char *name = test_shmem_info_get_name();
  int result = true;
  int rc = EXIT_SUCCESS;

  if (name == NULL) {
    rc = EXIT_FAILURE;
    result = false;
  }

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_info_get_name()", result, false);
  }

  shmem_finalize();
  return rc;
}