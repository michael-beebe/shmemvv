/**
 * @file shmemvv.c
 *
 */

#include "shmemvv.h"

/**
 * @brief Print error message saying that there needs to be at least
 *        2 PEs for the given test type.
 *
 * This function prints an error message indicating that at least 2 PEs
 * are required to run the specified test type.
 *
 * @param test_type Category of tests.
 */
void display_not_enough_pes(const char *test_type) {
  fprintf(stderr,
          RED_COLOR "ERROR" RESET_COLOR
                    ": The %s tests require at least 2 PEs!\n",
          test_type);
}

/**
 * @brief Displays the result of a test.
 *
 * This function prints out the result of a specific OpenSHMEM routine test.
 *
 * @param routine_name Name of the OpenSHMEM routine that was tested.
 * @param passed True if the test passed, false if the test failed.
 * @param required True if the test is required, false otherwise.
 */
void display_test_result(const char *routine_name, bool passed, bool required) {
  if (passed) {
    printf(GREEN_COLOR "PASSED" RESET_COLOR ": %s\n", routine_name);
  } else {
    if (required) {
      fprintf(stderr,
              RED_COLOR "FAILED" RESET_COLOR ": %s" RED_COLOR
                        " This test must pass to continue!" RESET_COLOR "\n",
              routine_name);
    } else {
      fprintf(stderr, RED_COLOR "FAILED" RESET_COLOR ": %s\n", routine_name);
    }
  }
}

void reduce_test_result(const char *routine_name, bool *result, bool required) {
  int npes = shmem_n_pes();
  bool passed = true;
  if (shmem_my_pe() == 0) {
    for (int i = 0; i < npes; i ++){
      passed &= shmem_g((char *)result, i);
    }
    display_test_result(routine_name, passed, required);
  }
}
