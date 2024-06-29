/**
  @file pt2pt_tests.hpp
  @brief Contains functions declarations with test functions for the pt2pt synchronization routines
 */

#include "pt2pt_tests.hpp"

/**
  @brief Tests the shmem_wait_until() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_wait_until(void) {
  long *flag = (long *)shmem_malloc(sizeof(long));
  *flag = 0;
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  if (npes < 2) {
    printf("This test requires at least 2 PEs\n");
    shmem_free(flag);
    return false;
  }

  printf("PE %d: Initialized flag to %ld\n", mype, *flag);
  shmem_barrier_all(); // Ensure all PEs reach this point before proceeding

  if (mype == 0) {
    printf("PE %d: Setting flag\n", mype);
    shmem_long_p(flag, 1, 0); // Use shmem_long_p to set the flag
    shmem_fence(); // Ensure the update is visible to all PEs
    printf("PE %d: Flag set to %ld\n", mype, *flag);
  }

  shmem_barrier_all(); // Ensure PE 0 has set the flag before others wait

  if (mype != 0) {
    printf("PE %d: Waiting for flag to be 1\n", mype);
    shmem_long_wait_until(flag, SHMEM_CMP_EQ, 1);
    printf("PE %d: Flag is %ld\n", mype, *flag);
  }

  shmem_barrier_all(); // Ensure all PEs have finished waiting

  shmem_free(flag);
  printf("PE %d: Test completed\n", mype);
  return true;
}
// bool test_shmem_wait_until(void) {
//   long *flag = (long *)shmem_malloc(sizeof(long));
//   *flag = 0;
//   int mype = shmem_my_pe();

//   if (mype == 0) {
//     *flag = 1;
//     shmem_quiet();
//   }

//   shmem_wait_until(flag, SHMEM_CMP_EQ, 1);
//   shmem_free(flag);

//   return true;
// }

/**
  @brief Tests the shmem_wait_until_all() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_wait_until_all(void) {
  long *flags = (long *)shmem_malloc(4 * sizeof(long));
  for (int i = 0; i < 4; ++i) {
    flags[i] = 0;
  }
  int mype = shmem_my_pe();

  if (mype == 0) {
    for (int i = 0; i < 4; ++i) {
      flags[i] = 1;
    }
    shmem_quiet();
  }

  shmem_wait_until_all(flags, 4, NULL, SHMEM_CMP_EQ, 1);
  shmem_free(flags);

  return true;
}

/**
  @brief Tests the shmem_wait_until_any() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_wait_until_any(void) {
  long *flags = (long *)shmem_malloc(4 * sizeof(long));
  for (int i = 0; i < 4; ++i) {
    flags[i] = 0;
  }
  int mype = shmem_my_pe();

  if (mype == 0) {
    flags[2] = 1;
    shmem_quiet();
  }

  shmem_wait_until_any(flags, 4, NULL, SHMEM_CMP_EQ, 1);
  shmem_free(flags);

  return true;
}

/**
  @brief Tests the shmem_wait_until_some() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_wait_until_some(void) {
  long *flags = (long *)shmem_malloc(4 * sizeof(long));
  for (int i = 0; i < 4; ++i) {
    flags[i] = 0;
  }
  int mype = shmem_my_pe();

  if (mype == 0) {
    flags[1] = 1;
    flags[3] = 1;
    shmem_quiet();
  }

  size_t indices[4];
  shmem_wait_until_some(flags, 4, indices, NULL, SHMEM_CMP_EQ, 1);
  shmem_free(flags);

  return true;
}

/**
  @brief Tests the shmem_wait_until_all_vector() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_wait_until_all_vector(void) {
  long *flags = (long *)shmem_malloc(4 * sizeof(long));
  for (int i = 0; i < 4; ++i) {
    flags[i] = 0;
  }
  int mype = shmem_my_pe();

  if (mype == 0) {
    for (int i = 0; i < 4; ++i) {
      flags[i] = 1;
    }
    shmem_quiet();
  }

  long cmp_values[4] = {1, 1, 1, 1};
  shmem_wait_until_all_vector(flags, 4, NULL, SHMEM_CMP_EQ, cmp_values);
  shmem_free(flags);

  return true;
}

/**
  @brief Tests the shmem_wait_until_any_vector() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_wait_until_any_vector(void) {
  long *flags = (long *)shmem_malloc(4 * sizeof(long));
  for (int i = 0; i < 4; ++i) {
    flags[i] = 0;
  }
  int mype = shmem_my_pe();

  if (mype == 0) {
    flags[2] = 1;
    shmem_quiet();
  }

  long cmp_values[4] = {1, 1, 1, 1};
  shmem_wait_until_any_vector(flags, 4, NULL, SHMEM_CMP_EQ, cmp_values);
  shmem_free(flags);

  return true;
}

/**
  @brief Tests the shmem_wait_until_some_vector() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_wait_until_some_vector(void) {
  long *flags = (long *)shmem_malloc(4 * sizeof(long));
  for (int i = 0; i < 4; ++i) {
    flags[i] = 0;
  }
  int mype = shmem_my_pe();

  if (mype == 0) {
    flags[1] = 1;
    flags[3] = 1;
    shmem_quiet();
  }

  size_t indices[4];
  long cmp_values[4] = {1, 1, 1, 1};
  shmem_wait_until_some_vector(flags, 4, indices, NULL, SHMEM_CMP_EQ, cmp_values);
  shmem_free(flags);

  return true;
}

/**
  @brief Tests the shmem_test() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_test(void) {
  long *flag = (long *)shmem_malloc(sizeof(long));
  *flag = 0;
  int mype = shmem_my_pe();

  if (mype == 0) {
    *flag = 1;
    shmem_quiet();
  }

  while (!shmem_test(flag, SHMEM_CMP_EQ, 1)) {
    /* Waiting for condition to be met */
  }
  shmem_free(flag);

  return true;
}

/**
  @brief Tests the shmem_test_all() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_test_all(void) {
  long *flags = (long *)shmem_malloc(4 * sizeof(long));
  for (int i = 0; i < 4; ++i) {
    flags[i] = 0;
  }
  int mype = shmem_my_pe();

  if (mype == 0) {
    for (int i = 0; i < 4; ++i) {
      flags[i] = 1;
    }
    shmem_quiet();
  }

  while (!shmem_test_all(flags, 4, NULL, SHMEM_CMP_EQ, 1)) {
    /* Waiting for condition to be met */
  }
  shmem_free(flags);

  return true;
}

/**
  @brief Tests the shmem_test_any() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_test_any(void) {
  long *flags = (long *)shmem_malloc(4 * sizeof(long));
  for (int i = 0; i < 4; ++i) {
    flags[i] = 0;
  }
  int mype = shmem_my_pe();

  if (mype == 0) {
    flags[2] = 1;
    shmem_quiet();
  }

  while (!shmem_test_any(flags, 4, NULL, SHMEM_CMP_EQ, 1)) {
    /* Waiting for condition to be met */
  }
  shmem_free(flags);

  return true;
}

/**
  @brief Tests the shmem_test_some() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_test_some(void) {
  long *flags = (long *)shmem_malloc(4 * sizeof(long));
  for (int i = 0; i < 4; ++i) {
    flags[i] = 0;
  }
  int mype = shmem_my_pe();

  if (mype == 0) {
    flags[1] = 1;
    flags[3] = 1;
    shmem_quiet();
  }

  size_t indices[4];
  while (!shmem_test_some(flags, 4, indices, NULL, SHMEM_CMP_EQ, 1)) {
    /* Waiting for condition to be met */
  }
  shmem_free(flags);

  return true;
}

/**
  @brief Tests the shmem_test_all_vector() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_test_all_vector(void) {
  long *flags = (long *)shmem_malloc(4 * sizeof(long));
  for (int i = 0; i < 4; ++i) {
    flags[i] = 0;
  }
  int mype = shmem_my_pe();

  if (mype == 0) {
    for (int i = 0; i < 4; ++i) {
      flags[i] = 1;
    }
    shmem_quiet();
  }

  long cmp_values[4] = {1, 1, 1, 1};
  while (!shmem_test_all_vector(flags, 4, NULL, SHMEM_CMP_EQ, cmp_values)) {
    /* Waiting for condition to be met */
  }
  shmem_free(flags);

  return true;
}

/**
  @brief Tests the shmem_test_any_vector() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_test_any_vector(void) {
  long *flags = (long *)shmem_malloc(4 * sizeof(long));
  for (int i = 0; i < 4; ++i) {
    flags[i] = 0;
  }
  int mype = shmem_my_pe();

  if (mype == 0) {
    flags[2] = 1;
    shmem_quiet();
  }

  long cmp_values[4] = {1, 1, 1, 1};
  while (!shmem_test_any_vector(flags, 4, NULL, SHMEM_CMP_EQ, cmp_values)) {
    /* Waiting for condition to be met */
  }
  shmem_free(flags);

  return true;
}

/**
  @brief Tests the shmem_test_some_vector() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_test_some_vector(void) {
  long *flags = (long *)shmem_malloc(4 * sizeof(long));
  for (int i = 0; i < 4; ++i) {
    flags[i] = 0;
  }
  int mype = shmem_my_pe();

  if (mype == 0) {
    flags[1] = 1;
    flags[3] = 1;
    shmem_quiet();
  }

  size_t indices[4];
  long cmp_values[4] = {1, 1, 1, 1};
  while (!shmem_test_some_vector(flags, 4, indices, NULL, SHMEM_CMP_EQ, cmp_values)) {
    /* Waiting for condition to be met */
  }
  shmem_free(flags);

  return true;
}

/**
  @brief Tests the shmem_signal_wait_until() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_signal_wait_until(void) {
  long *flag = (long *)shmem_malloc(sizeof(long));
  *flag = 0;
  int mype = shmem_my_pe();

  if (mype == 0) {
    shmem_long_p(flag, 1, 1);
  }

  shmem_long_wait_until(flag, SHMEM_CMP_EQ, 1);
  shmem_free(flag);

  return true;
}
