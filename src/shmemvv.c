/**
 * @file shmemvv.c
 *
 */

#include "shmemvv.h"
#include <sys/time.h>

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


/**
 * @brief print success or failure depending on test results
 *
 * Test results are gathered from all PEs in a reduce and operation.
 * If all PEs are successful, prints successful. 
 * If single PE fails, prints out failure.
 *
 * @param routine_name routine name to output to user
 * @param result pointer to result of each PE 
 * @param required True if the test is required, false otherwise.
 */
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

/**
 * @brief return the current time in miliseconds
 *
 */
long current_time_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return tv.tv_sec * 1000L + tv.tv_usec / 1000L;
}
