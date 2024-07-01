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

  #ifdef _DEBUG_
    printf("PE %d: Initialized flag to %ld\n", mype, *flag);
  #endif
  shmem_barrier_all();

  if (mype == 0) {
    #ifdef _DEBUG_
      printf("PE %d: Setting flag\n", mype);
    #endif
    shmem_long_p(flag, 1, 1);
    shmem_fence();
    #ifdef _DEBUG_
      printf("PE %d: Flag set to %ld\n", mype, *flag);
    #endif
  }

  shmem_barrier_all();

  if (mype != 0) {
    #ifdef _DEBUG_
      printf("PE %d: Waiting for flag to be 1\n", mype);
    #endif
    time_t start_time = time(NULL);
    while (*flag != 1) {
      shmem_long_wait_until(flag, SHMEM_CMP_EQ, 1);
      if (time(NULL) - start_time > 10) {
        #ifdef _DEBUG_
          printf("PE %d: Timeout waiting for flag to be 1\n", mype);
        #endif
        shmem_free(flag);
        return false;
      }
    }
    #ifdef _DEBUG_
      printf("PE %d: Flag is %ld\n", mype, *flag);
    #endif
  }

  shmem_barrier_all();

  shmem_free(flag);
  #ifdef _DEBUG_
    printf("PE %d: Test completed\n", mype);
  #endif
  return true;
}

/**
  @brief Tests the shmem_wait_until_all() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_wait_until_all(void) {
  long *flags = (long *)shmem_malloc(sizeof(long) * 2);
  flags[0] = 0;
  flags[1] = 0;
  int mype = shmem_my_pe();

  #ifdef _DEBUG_
    printf("PE %d: Initialized flags to %ld, %ld\n", mype, flags[0], flags[1]);
  #endif
  shmem_barrier_all();

  if (mype == 0) {
    #ifdef _DEBUG_
      printf("PE %d: Setting flags\n", mype);
    #endif
    shmem_long_p(&flags[0], 1, 1);
    shmem_long_p(&flags[1], 1, 1);
    shmem_fence();
    #ifdef _DEBUG_
      printf("PE %d: Flags set to %ld, %ld\n", mype, flags[0], flags[1]);
    #endif
  }

  shmem_barrier_all();

  if (mype != 0) {
    #ifdef _DEBUG_
      printf("PE %d: Waiting for flags to be 1\n", mype);
    #endif
    time_t start_time = time(NULL);
    shmem_long_wait_until_all(flags, 2, NULL, SHMEM_CMP_EQ, 1);
    if (time(NULL) - start_time > 10) {
      #ifdef _DEBUG_
        printf("PE %d: Timeout waiting for flags to be 1\n", mype);
      #endif
      shmem_free(flags);
      return false;
    }
    #ifdef _DEBUG_
      printf("PE %d: Flags are %ld, %ld\n", mype, flags[0], flags[1]);
    #endif
  }

  shmem_barrier_all();

  shmem_free(flags);
  #ifdef _DEBUG_
    printf("PE %d: Test completed\n", mype);
  #endif
  return true;
}

/**
  @brief Tests the shmem_wait_until_any() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_wait_until_any(void) {
  long *flags = (long *)shmem_malloc(3 * sizeof(long));
  for (int i = 0; i < 3; i++) {
    flags[i] = 0;
  }

  int mype = shmem_my_pe();
  long value = 1;

  #ifdef _DEBUG_
    printf("PE %d: Initialized flags to 0, 0, 0\n", mype);
  #endif

  shmem_barrier_all();

  if (mype == 0) {
    #ifdef _DEBUG_
      printf("PE %d: Setting flags[2]\n", mype);
    #endif
    shmem_long_p(&flags[2], value, 1);
    shmem_quiet();
    #ifdef _DEBUG_
      printf("PE %d: flags[2] set to %ld\n", mype, flags[2]);
    #endif
  }

  shmem_barrier_all();

  if (mype != 0) {
    #ifdef _DEBUG_
      printf("PE %d: Waiting for any flag to be 1\n", mype);
    #endif
    int status[3] = {SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ};
    size_t index = shmem_long_wait_until_any(flags, 3, status, SHMEM_CMP_EQ, 1);
    if (index == -1) {
      #ifdef _DEBUG_
        printf("PE %d: Timeout waiting for any flag to be 1\n", mype);
      #endif
      shmem_free(flags);
      return false;
    }
    #ifdef _DEBUG_
      printf("PE %d: Flag %lu is 1\n", mype, index);
    #endif
  }

  shmem_barrier_all();
  shmem_free(flags);
  #ifdef _DEBUG_
    printf("PE %d: Test completed\n", mype);
  #endif
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
  long value = 1;

  #ifdef _DEBUG_
    printf("PE %d: Initialized flags to 0\n", mype);
  #endif

  shmem_barrier_all();

  if (mype == 0) {
    #ifdef _DEBUG_
      printf("PE %d: Setting flags[1] and flags[3]\n", mype);
    #endif
    shmem_long_p(&flags[1], value, 1);
    shmem_long_p(&flags[3], value, 1);
    shmem_quiet();
    #ifdef _DEBUG_
      printf("PE %d: flags[1] and flags[3] set to 1\n", mype);
    #endif
  }

  shmem_barrier_all();

  if (mype != 0) {
    #ifdef _DEBUG_
      printf("PE %d: Waiting for some flags to be 1\n", mype);
    #endif
    time_t start_time = time(NULL);
    size_t indices[4];
    int status[4] = {SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ};
    size_t count = shmem_long_wait_until_some(flags, 4, indices, status, SHMEM_CMP_EQ, 1);
    if (count < 2 || time(NULL) - start_time > 10) {
      #ifdef _DEBUG_
        printf("PE %d: Timeout waiting for some flags to be 1\n", mype);
      #endif
      shmem_free(flags);
      return false;
    }
    #ifdef _DEBUG_
      printf("PE %d: Some of the flags are 1\n", mype);
    #endif
  }

  shmem_barrier_all();
  shmem_free(flags);
  #ifdef _DEBUG_
    printf("PE %d: Test completed\n", mype);
  #endif
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

  #ifdef _DEBUG_
    printf("PE %d: Initialized flags to 0\n", mype);
  #endif

  shmem_barrier_all();

  if (mype == 0) {
    #ifdef _DEBUG_
      printf("PE %d: Setting all flags\n", mype);
    #endif
    for (int i = 0; i < 4; ++i) {
      shmem_long_p(&flags[i], 1, 1);
    }
    shmem_quiet();
    #ifdef _DEBUG_
      printf("PE %d: All flags set to 1\n", mype);
    #endif
  }

  shmem_barrier_all();

  if (mype != 0) {
    #ifdef _DEBUG_
      printf("PE %d: Waiting for all flags to be 1\n", mype);
    #endif
    time_t start_time = time(NULL);
    int status[4] = {SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ};
    long cmp_values[4] = {1, 1, 1, 1};
    shmem_long_wait_until_all_vector(flags, 4, status, SHMEM_CMP_EQ, cmp_values);
    if (time(NULL) - start_time > 10) {
      #ifdef _DEBUG_
        printf("PE %d: Timeout waiting for all flags to be 1\n", mype);
      #endif
      shmem_free(flags);
      return false;
    }
    #ifdef _DEBUG_
      printf("PE %d: All flags are 1\n", mype);
    #endif
  }

  shmem_barrier_all();
  shmem_free(flags);
  #ifdef _DEBUG_
    printf("PE %d: Test completed\n", mype);
  #endif
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

  #ifdef _DEBUG_
    printf("PE %d: Initialized flags to 0\n", mype);
  #endif

  shmem_barrier_all();

  if (mype == 0) {
    #ifdef _DEBUG_
      printf("PE %d: Setting flags[2]\n", mype);
    #endif
    shmem_long_p(&flags[2], 1, 1);
    shmem_quiet();
    #ifdef _DEBUG_
      printf("PE %d: flags[2] set to 1\n", mype);
    #endif
  }

  shmem_barrier_all();

  if (mype != 0) {
    #ifdef _DEBUG_
      printf("PE %d: Waiting for any flag to be 1\n", mype);
    #endif
    time_t start_time = time(NULL);
    int status[4] = {SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ};
    long cmp_values[4] = {1, 1, 1, 1};
    size_t index = shmem_long_wait_until_any_vector(flags, 4, status, SHMEM_CMP_EQ, cmp_values);
    if (time(NULL) - start_time > 10) {
      #ifdef _DEBUG_
        printf("PE %d: Timeout waiting for any flag to be 1\n", mype);
      #endif
      shmem_free(flags);
      return false;
    }
    #ifdef _DEBUG_
      printf("PE %d: One of the flags is 1 (index %lu)\n", mype, index);
    #endif
  }

  shmem_barrier_all();
  shmem_free(flags);
  #ifdef _DEBUG_
    printf("PE %d: Test completed\n", mype);
  #endif
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

  #ifdef _DEBUG_
    printf("PE %d: Initialized flags to 0\n", mype);
  #endif

  shmem_barrier_all();

  if (mype == 0) {
    #ifdef _DEBUG_
      printf("PE %d: Setting flags[1] and flags[3]\n", mype);
    #endif
    shmem_long_p(&flags[1], 1, 1);
    shmem_long_p(&flags[3], 1, 1);
    shmem_quiet();
    #ifdef _DEBUG_
      printf("PE %d: flags[1] and flags[3] set to 1\n", mype);
    #endif
  }

  shmem_barrier_all();

  if (mype != 0) {
    #ifdef _DEBUG_
      printf("PE %d: Waiting for some flags to be 1\n", mype);
    #endif
    time_t start_time = time(NULL);
    int status[4] = {SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ};
    long cmp_values[4] = {1, 1, 1, 1};
    size_t indices[4];
    size_t num_indices;
    while ((num_indices = shmem_long_wait_until_some_vector(flags, 4, indices, status, SHMEM_CMP_EQ, cmp_values)) == 0) {
      if (time(NULL) - start_time > 10) {
        #ifdef _DEBUG_
          printf("PE %d: Timeout waiting for some flags to be 1\n", mype);
        #endif
        shmem_free(flags);
        return false;
      }
    }
    #ifdef _DEBUG_
      printf("PE %d: Some of the flags are 1 (indices: %zu)\n", mype, num_indices);
    #endif
  }

  shmem_barrier_all();
  shmem_free(flags);
  #ifdef _DEBUG_
    printf("PE %d: Test completed\n", mype);
  #endif
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

  #ifdef _DEBUG_
    printf("PE %d: Initialized flag to 0\n", mype);
  #endif

  shmem_barrier_all();

  if (mype == 0) {
    #ifdef _DEBUG_
      printf("PE %d: Setting flag\n", mype);
    #endif
    *flag = 1;
    shmem_quiet();
    #ifdef _DEBUG_
      printf("PE %d: Flag set to 1\n", mype);
    #endif
  }

  shmem_barrier_all();

  if (mype != 0) {
    #ifdef _DEBUG_
      printf("PE %d: Waiting for flag to be 1\n", mype);
    #endif
    time_t start_time = time(NULL);
    while (!shmem_test(flag, SHMEM_CMP_EQ, 1)) {
      if (time(NULL) - start_time > 10) {
        #ifdef _DEBUG_
          printf("PE %d: Timeout waiting for flag to be 1\n", mype);
        #endif
        shmem_free(flag);
        return false;
      }
    }
    #ifdef _DEBUG_
      printf("PE %d: Flag is 1\n", mype);
    #endif
  }

  shmem_barrier_all();
  shmem_free(flag);
  #ifdef _DEBUG_
    printf("PE %d: Test completed\n", mype);
  #endif
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

  #ifdef _DEBUG_
    printf("PE %d: Initialized flags to 0\n", mype);
  #endif

  shmem_barrier_all();

  if (mype == 0) {
    #ifdef _DEBUG_
      printf("PE %d: Setting all flags\n", mype);
    #endif
    for (int i = 0; i < 4; ++i) {
      flags[i] = 1;
    }
    shmem_quiet();
    #ifdef _DEBUG_
      printf("PE %d: All flags set to 1\n", mype);
    #endif
  }

  shmem_barrier_all();

  if (mype != 0) {
    #ifdef _DEBUG_
      printf("PE %d: Waiting for all flags to be 1\n", mype);
    #endif
    time_t start_time = time(NULL);
    while (!shmem_test_all(flags, 4, NULL, SHMEM_CMP_EQ, 1)) {
      if (time(NULL) - start_time > 10) {
        #ifdef _DEBUG_
          printf("PE %d: Timeout waiting for all flags to be 1\n", mype);
        #endif
        shmem_free(flags);
        return false;
      }
    }
    #ifdef _DEBUG_
      printf("PE %d: All flags are 1\n", mype);
    #endif
  }

  shmem_barrier_all();
  shmem_free(flags);
  #ifdef _DEBUG_
    printf("PE %d: Test completed\n", mype);
  #endif
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

  #ifdef _DEBUG_
    printf("PE %d: Initialized flags to 0\n", mype);
  #endif

  shmem_barrier_all();

  if (mype == 0) {
    #ifdef _DEBUG_
      printf("PE %d: Setting flags[2]\n", mype);
    #endif
    flags[2] = 1;
    shmem_quiet();
    #ifdef _DEBUG_
      printf("PE %d: flags[2] set to 1\n", mype);
    #endif
  }

  shmem_barrier_all();

  if (mype != 0) {
    #ifdef _DEBUG_
      printf("PE %d: Waiting for any flag to be 1\n", mype);
    #endif
    time_t start_time = time(NULL);
    while (!shmem_test_any(flags, 4, NULL, SHMEM_CMP_EQ, 1)) {
      if (time(NULL) - start_time > 10) {
        #ifdef _DEBUG_
          printf("PE %d: Timeout waiting for any flag to be 1\n", mype);
        #endif
        shmem_free(flags);
        return false;
      }
    }
    #ifdef _DEBUG_
      printf("PE %d: One of the flags is 1\n", mype);
    #endif
  }

  shmem_barrier_all();
  shmem_free(flags);
  #ifdef _DEBUG_
    printf("PE %d: Test completed\n", mype);
  #endif
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

  #ifdef _DEBUG_
    printf("PE %d: Initialized flags to 0\n", mype);
  #endif

  shmem_barrier_all();

  if (mype == 0) {
    #ifdef _DEBUG_
      printf("PE %d: Setting flags[1] and flags[3]\n", mype);
    #endif
    flags[1] = 1;
    flags[3] = 1;
    shmem_quiet();
    #ifdef _DEBUG_
      printf("PE %d: flags[1] and flags[3] set to 1\n", mype);
    #endif
  }

  shmem_barrier_all();

  if (mype != 0) {
    #ifdef _DEBUG_
      printf("PE %d: Waiting for some flags to be 1\n", mype);
    #endif
    time_t start_time = time(NULL);
    size_t indices[4];
    while (!shmem_test_some(flags, 4, indices, NULL, SHMEM_CMP_EQ, 1)) {
      if (time(NULL) - start_time > 10) {
        #ifdef _DEBUG_
          printf("PE %d: Timeout waiting for some flags to be 1\n", mype);
        #endif
        shmem_free(flags);
        return false;
      }
    }
    #ifdef _DEBUG_
      printf("PE %d: Some of the flags are 1\n", mype);
    #endif
  }

  shmem_barrier_all();
  shmem_free(flags);
  #ifdef _DEBUG_
    printf("PE %d: Test completed\n", mype);
  #endif
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

  #ifdef _DEBUG_
    printf("PE %d: Initialized flags to 0\n", mype);
  #endif

  shmem_barrier_all();

  if (mype == 0) {
    #ifdef _DEBUG_
      printf("PE %d: Setting all flags\n", mype);
    #endif
    for (int i = 0; i < 4; ++i) {
      flags[i] = 1;
    }
    shmem_quiet();
    #ifdef _DEBUG_
      printf("PE %d: All flags set to 1\n", mype);
    #endif
  }

  shmem_barrier_all();

  if (mype != 0) {
    #ifdef _DEBUG_
      printf("PE %d: Waiting for all flags to be 1\n", mype);
    #endif
    time_t start_time = time(NULL);
    long cmp_values[4] = {1, 1, 1, 1};
    while (!shmem_test_all_vector(flags, 4, NULL, SHMEM_CMP_EQ, cmp_values)) {
      if (time(NULL) - start_time > 10) {
        #ifdef _DEBUG_
          printf("PE %d: Timeout waiting for all flags to be 1\n", mype);
        #endif
        shmem_free(flags);
        return false;
      }
    }
    #ifdef _DEBUG_
      printf("PE %d: All flags are 1\n", mype);
    #endif
  }

  shmem_barrier_all();
  shmem_free(flags);
  #ifdef _DEBUG_
    printf("PE %d: Test completed\n", mype);
  #endif
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
  int npes = shmem_n_pes();

  #ifdef _DEBUG_
    printf("PE %d: Initialized flags to 0\n", mype);
  #endif

  shmem_barrier_all();

  if (mype == 0) {
    #ifdef _DEBUG_
      printf("PE %d: Setting flags[2] for all PEs\n", mype);
    #endif
    long one = 1;
    for (int pe = 1; pe < npes; ++pe) {
      shmem_long_p(&flags[2], one, pe);
    }
    shmem_quiet();
    #ifdef _DEBUG_
      printf("PE %d: flags[2] set to 1 for all PEs\n", mype);
    #endif
  }

  shmem_barrier_all();

  #ifdef _DEBUG_
    printf("PE %d: Flags after setting: %ld %ld %ld %ld\n", mype, flags[0], flags[1], flags[2], flags[3]);
  #endif

  if (mype != 0) {
    #ifdef _DEBUG_
      printf("PE %d: Waiting for any flag to be 1\n", mype);
    #endif
    time_t start_time = time(NULL);
    long cmp_values[4] = {1, 1, 1, 1};
    while (!shmem_test_any_vector(flags, 4, NULL, SHMEM_CMP_EQ, cmp_values)) {
      if (time(NULL) - start_time > 10) {
        #ifdef _DEBUG_
          printf("PE %d: Timeout waiting for any flag to be 1\n", mype);
          printf("PE %d: Current flags: %ld %ld %ld %ld\n", mype, flags[0], flags[1], flags[2], flags[3]);
        #endif
        shmem_free(flags);
        return false;
      }
      #ifdef _DEBUG_
        printf("PE %d: Still waiting... Flags: %ld %ld %ld %ld\n", mype, flags[0], flags[1], flags[2], flags[3]);
      #endif
      sleep(1); // Sleep for a short duration to avoid busy waiting
    }
    #ifdef _DEBUG_
      printf("PE %d: One of the flags is 1\n", mype);
    #endif
  }

  shmem_barrier_all();
  shmem_free(flags);
  #ifdef _DEBUG_
    printf("PE %d: Test completed\n", mype);
  #endif
  return true;
}

/** FIXME: getting stuck here
  @brief Tests the shmem_test_some_vector() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_test_some_vector(void) {
  long *flags = (long *)shmem_malloc(4 * sizeof(long));
  for (int i = 0; i < 4; ++i) {
    flags[i] = 0;
  }
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  #ifdef _DEBUG_
    printf("PE %d: Initialized flags to 0\n", mype);
  #endif

  shmem_barrier_all();

  if (mype == 0) {
    #ifdef _DEBUG_
      printf("PE %d: Setting flags[1] and flags[3] for all PEs\n", mype);
    #endif
    long one = 1;
    for (int pe = 1; pe < npes; ++pe) {
      shmem_long_p(&flags[1], one, pe);
      shmem_long_p(&flags[3], one, pe);
    }
    shmem_quiet();
    #ifdef _DEBUG_
      printf("PE %d: flags[1] and flags[3] set to 1 for all PEs\n", mype);
    #endif
  }

  shmem_barrier_all();

  #ifdef _DEBUG_
    printf("PE %d: Flags after setting: %ld %ld %ld %ld\n", mype, flags[0], flags[1], flags[2], flags[3]);
  #endif

  if (mype != 0) {
    #ifdef _DEBUG_
      printf("PE %d: Waiting for some flags to be 1\n", mype);
    #endif
    time_t start_time = time(NULL);
    long cmp_values[4] = {1, 1, 1, 1};
    size_t indices[4];
    size_t num_indices;
    while ((num_indices = shmem_long_test_some_vector(flags, 4, indices, NULL, SHMEM_CMP_EQ, cmp_values)) == 0) {
      if (time(NULL) - start_time > 10) {
        #ifdef _DEBUG_
          printf("PE %d: Timeout waiting for some flags to be 1\n", mype);
          printf("PE %d: Current flags: %ld %ld %ld %ld\n", mype, flags[0], flags[1], flags[2], flags[3]);
        #endif
        shmem_free(flags);
        return false;
      }
      #ifdef _DEBUG_
        printf("PE %d: Still waiting... Flags: %ld %ld %ld %ld\n", mype, flags[0], flags[1], flags[2], flags[3]);
      #endif
      sleep(1); // Sleep for a short duration to avoid busy waiting
    }
    #ifdef _DEBUG_
      printf("PE %d: Some of the flags are 1 (indices: %zu)\n", mype, num_indices);
    #endif
  }

  shmem_barrier_all();
  shmem_free(flags);
  #ifdef _DEBUG_
    printf("PE %d: Test completed\n", mype);
  #endif
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
  long value = 1;
  uint64_t dummy_value = 0;
  uint64_t *dummy_ptr = &dummy_value;

  #ifdef _DEBUG_
    printf("PE %d: Initialized flag to 0\n", mype);
  #endif

  shmem_barrier_all();

  if (mype == 0) {
    #ifdef _DEBUG_
      printf("PE %d: Setting flag\n", mype);
    #endif
    shmem_put_signal(flag, &value, 1, (uint64_t *)dummy_ptr, dummy_value, SHMEM_SIGNAL_SET, 0);
    shmem_quiet();
    #ifdef _DEBUG_
      printf("PE %d: Flag set to 1\n", mype);
    #endif
  }

  shmem_barrier_all();

  if (mype != 0) {
    #ifdef _DEBUG_
      printf("PE %d: Waiting for flag to be 1\n", mype);
    #endif
    time_t start_time = time(NULL);
    while (*flag != 1) {
      shmem_long_wait_until(flag, SHMEM_CMP_EQ, 1);
      if (time(NULL) - start_time > 10) {
        #ifdef _DEBUG_
          printf("PE %d: Timeout waiting for flag to be 1\n", mype);
        #endif
        shmem_free(flag);
        return false;
      }
    }
    #ifdef _DEBUG_
      printf("PE %d: Flag is 1\n", mype);
    #endif
  }

  shmem_barrier_all();
  shmem_free(flag);
  #ifdef _DEBUG_
    printf("PE %d: Test completed\n", mype);
  #endif
  return true;
}

