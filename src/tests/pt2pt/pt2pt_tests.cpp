/**
  @file pt2pt_tests.cpp
  @brief Contains functions definitions with test functions for the pt2pt synchronization routines
*/

#include "pt2pt_tests.hpp"

/**
  @brief Helper function to implement a timeout mechanism.
  @param condition The condition to be checked within the timeout.
  @param timeout The maximum time to wait in seconds.
  @return True if the condition becomes true within the timeout, false otherwise.
*/
bool wait_with_timeout(bool (*condition)(), int timeout) {
  time_t start_time = time(NULL);
  while (!condition()) {
    if (time(NULL) - start_time > timeout) {
      return false;
    }
    sleep(1); // Sleep to avoid busy waiting
  }
  return true;
}

/**
  @brief Helper condition for shmem_long_test.
  @param flag The flag to be tested.
  @param cmp The comparison operator.
  @param value The value to be compared.
  @return True if the test condition is met, false otherwise.
*/
bool condition_shmem_long_test(long *flag, int cmp, long value) {
  return p_shmem_long_test(flag, cmp, value);
}

/**
  @brief Tests the shmem_wait_until() routine.
  @return True if the test is successful, false otherwise.
*/
bool test_shmem_wait_until(void) {
  long *flag = (long *)p_shmem_malloc(sizeof(long));
  *flag = 0;
  int mype = p_shmem_my_pe();

  p_shmem_barrier_all();

  if (mype == 0) {
    p_shmem_long_p(flag, 1, 1);
    p_shmem_quiet();
  }

  p_shmem_barrier_all();

  if (mype != 0) {
    p_shmem_long_wait_until(flag, SHMEM_CMP_EQ, 1);
    if (*flag != 1) {
      p_shmem_free(flag);
      return false;
    }
  }

  p_shmem_free(flag);
  return true;
}

/**
  @brief Tests the shmem_wait_until_all() routine.
  @return True if the test is successful, false otherwise.
*/
bool test_shmem_wait_until_all(void) {
  long *flags = (long *)p_shmem_malloc(2 * sizeof(long));
  flags[0] = 0;
  flags[1] = 0;
  int mype = p_shmem_my_pe();

  p_shmem_barrier_all();

  if (mype == 0) {
    p_shmem_long_p(&flags[0], 1, 1);
    p_shmem_long_p(&flags[1], 1, 1);
    p_shmem_quiet();
  }

  p_shmem_barrier_all();

  if (mype != 0) {
    p_shmem_long_wait_until_all(flags, 2, NULL, SHMEM_CMP_EQ, 1);
    if (flags[0] != 1 || flags[1] != 1) {
      p_shmem_free(flags);
      return false;
    }
  }

  p_shmem_free(flags);
  return true;
}

/**
  @brief Tests the shmem_wait_until_any() routine.
  @return True if the test is successful, false otherwise.
*/
bool test_shmem_wait_until_any(void) {
  long *flags = (long *)p_shmem_malloc(3 * sizeof(long));
  for (int i = 0; i < 3; i++) {
    flags[i] = 0;
  }
  int mype = p_shmem_my_pe();

  p_shmem_barrier_all();

  if (mype == 0) {
    p_shmem_long_p(&flags[2], 1, 1);
    p_shmem_quiet();
  }

  p_shmem_barrier_all();

  if (mype != 0) {
    int status[3] = {SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ};
    size_t index = p_shmem_long_wait_until_any(flags, 3, status, SHMEM_CMP_EQ, 1);
    if (index == SIZE_MAX) {
      p_shmem_free(flags);
      return false;
    }
    if (flags[index] != 1) {
      p_shmem_free(flags);
      return false;
    }
  }

  p_shmem_free(flags);
  return true;
}

/**
  @brief Tests the shmem_wait_until_some() routine.
  @return True if the test is successful, false otherwise.
*/
bool test_shmem_wait_until_some(void) {
  long *flags = (long *)p_shmem_malloc(4 * sizeof(long));
  for (int i = 0; i < 4; ++i) {
    flags[i] = 0;
  }
  int mype = p_shmem_my_pe();

  p_shmem_barrier_all();

  if (mype == 0) {
    p_shmem_long_p(&flags[1], 1, 1);
    p_shmem_long_p(&flags[3], 1, 1);
    p_shmem_quiet();
  }

  p_shmem_barrier_all();

  if (mype != 0) {
    size_t indices[4];
    int status[4] = {SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ};
    size_t count = p_shmem_long_wait_until_some(flags, 4, indices, status, SHMEM_CMP_EQ, 1);
    if (count < 2) {
      p_shmem_free(flags);
      return false;
    }
    for (size_t i = 0; i < count; ++i) {
      if (flags[indices[i]] != 1) {
        p_shmem_free(flags);
        return false;
      }
    }
  }

  p_shmem_free(flags);
  return true;
}

/**
  @brief Tests the shmem_wait_until_all_vector() routine.
  @return True if the test is successful, false otherwise.
*/
bool test_shmem_wait_until_all_vector(void) {
  long *flags = (long *)p_shmem_malloc(4 * sizeof(long));
  for (int i = 0; i < 4; ++i) {
    flags[i] = 0;
  }
  int mype = p_shmem_my_pe();

  p_shmem_barrier_all();

  if (mype == 0) {
    for (int i = 0; i < 4; ++i) {
      p_shmem_long_p(&flags[i], 1, 1);
      p_shmem_quiet();
    }
  }

  p_shmem_barrier_all();

  if (mype != 0) {
    int status[4] = {SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ};
    long cmp_values[4] = {1, 1, 1, 1};
    p_shmem_long_wait_until_all_vector(flags, 4, status, SHMEM_CMP_EQ, cmp_values);
    for (int i = 0; i < 4; ++i) {
      if (flags[i] != 1) {
        p_shmem_free(flags);
        return false;
      }
    }
  }

  p_shmem_free(flags);
  return true;
}

/**
  @brief Tests the shmem_wait_until_any_vector() routine.
  @return True if the test is successful, false otherwise.
*/
bool test_shmem_wait_until_any_vector(void) {
  long *flags = (long *)p_shmem_malloc(4 * sizeof(long));
  for (int i = 0; i < 4; ++i) {
    flags[i] = 0;
  }
  int mype = p_shmem_my_pe();

  p_shmem_barrier_all();

  if (mype == 0) {
    p_shmem_long_p(&flags[2], 1, 1);
    p_shmem_quiet();
  }

  p_shmem_barrier_all();

  if (mype != 0) {
    int status[4] = {SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ};
    long cmp_values[4] = {1, 1, 1, 1};
    size_t index = p_shmem_long_wait_until_any_vector(flags, 4, status, SHMEM_CMP_EQ, cmp_values);
    if (index == SIZE_MAX) {
      p_shmem_free(flags);
      return false;
    }
    if (flags[index] != 1) {
      p_shmem_free(flags);
      return false;
    }
  }

  p_shmem_free(flags);
  return true;
}

/**
  @brief Tests the shmem_wait_until_some_vector() routine.
  @return True if the test is successful, false otherwise.
*/
bool test_shmem_wait_until_some_vector(void) {
  long *flags = (long *)p_shmem_malloc(4 * sizeof(long));
  for (int i = 0; i < 4; ++i) {
    flags[i] = 0;
  }
  int mype = p_shmem_my_pe();

  p_shmem_barrier_all();

  if (mype == 0) {
    p_shmem_long_p(&flags[1], 1, 1);
    p_shmem_long_p(&flags[3], 1, 1);
    p_shmem_quiet();
  }

  p_shmem_barrier_all();

  if (mype != 0) {
    int status[4] = {SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ};
    long cmp_values[4] = {1, 1, 1, 1};
    size_t indices[4];
    size_t num_indices = p_shmem_long_wait_until_some_vector(flags, 4, indices, status, SHMEM_CMP_EQ, cmp_values);
    if (num_indices < 2) {
      p_shmem_free(flags);
      return false;
    }
    for (size_t i = 0; i < num_indices; ++i) {
      if (flags[indices[i]] != 1) {
        p_shmem_free(flags);
        return false;
      }
    }
  }

  p_shmem_free(flags);
  return true;
}

/**
  @brief Tests the shmem_test() routine.
  @return True if the test is successful, false otherwise.
*/
bool test_shmem_test(void) {
  long *flag = (long *)p_shmem_malloc(sizeof(long));
  if (flag == NULL) {
    return false;
  }

  *flag = 0;
  int mype = p_shmem_my_pe();

  p_shmem_barrier_all();

  if (mype == 0) {
    *flag = 1;
    p_shmem_quiet();
  }

  p_shmem_barrier_all();

  if (mype != 0) {
    time_t start_time = time(NULL);
    while (!p_shmem_long_test(flag, SHMEM_CMP_EQ, 1)) {
      if (time(NULL) - start_time > 10) {
        break;
      }
      sleep(1);
    }
    if (*flag != 1) {
      p_shmem_free(flag);
      return false;
    }
  }

  p_shmem_free(flag);
  return true;
}

/**
  @brief Tests the shmem_test_all() routine.
  @return True if the test is successful, false otherwise.
*/
bool test_shmem_test_all(void) {
  long *flags = (long *)p_shmem_malloc(4 * sizeof(long));
  if (flags == NULL) {
    return false;
  }

  for (int i = 0; i < 4; ++i) {
    flags[i] = 0;
  }
  int mype = p_shmem_my_pe();

  p_shmem_barrier_all();

  if (mype == 0) {
    for (int i = 0; i < 4; ++i) {
      flags[i] = 1;
    }
    p_shmem_quiet();
  }

  p_shmem_barrier_all();

  if (mype != 0) {
    time_t start_time = time(NULL);
    while (!p_shmem_long_test_all(flags, 4, NULL, SHMEM_CMP_EQ, 1)) {
      if (time(NULL) - start_time > 10) {
        break;
      }
      sleep(1);
    }
    for (int i = 0; i < 4; ++i) {
      if (flags[i] != 1) {
        p_shmem_free(flags);
        return false;
      }
    }
  }

  p_shmem_free(flags);
  return true;
}

/**
  @brief Tests the shmem_test_any() routine.
  @return True if the test is successful, false otherwise.
*/
bool test_shmem_test_any(void) {
  long *flags = (long *)p_shmem_malloc(4 * sizeof(long));
  if (flags == NULL) {
    return false;
  }

  for (int i = 0; i < 4; ++i) {
    flags[i] = 0;
  }
  int mype = p_shmem_my_pe();

  p_shmem_barrier_all();

  if (mype == 0) {
    flags[2] = 1;
    p_shmem_quiet();
  }

  p_shmem_barrier_all();

  if (mype != 0) {
    time_t start_time = time(NULL);
    while (!p_shmem_long_test_any(flags, 4, NULL, SHMEM_CMP_EQ, 1)) {
      if (time(NULL) - start_time > 10) {
        break;
      }
      sleep(1);
    }
    if (flags[2] != 1) {
      p_shmem_free(flags);
      return false;
    }
  }

  p_shmem_free(flags);
  return true;
}

/**
  @brief Tests the shmem_test_some() routine.
  @return True if the test is successful, false otherwise.
*/
bool test_shmem_test_some(void) {
  long *flags = (long *)p_shmem_malloc(4 * sizeof(long));
  if (flags == NULL) {
    return false;
  }

  for (int i = 0; i < 4; ++i) {
    flags[i] = 0;
  }
  int mype = p_shmem_my_pe();

  p_shmem_barrier_all();

  if (mype == 0) {
    flags[1] = 1;
    flags[3] = 1;
    p_shmem_quiet();
  }

  p_shmem_barrier_all();

  if (mype != 0) {
    size_t indices[4];
    time_t start_time = time(NULL);
    while (!p_shmem_long_test_some(flags, 4, indices, NULL, SHMEM_CMP_EQ, 1)) {
      if (time(NULL) - start_time > 10) {
        break;
      }
      sleep(1);
    }
    if (flags[1] != 1 || flags[3] != 1) {
      p_shmem_free(flags);
      return false;
    }
  }

  p_shmem_free(flags);
  return true;
}

/**
  @brief Tests the shmem_test_all_vector() routine.
  @return True if the test is successful, false otherwise.
*/
bool test_shmem_test_all_vector(void) {
  long *flags = (long *)p_shmem_malloc(4 * sizeof(long));
  if (flags == NULL) {
    return false;
  }

  for (int i = 0; i < 4; ++i) {
    flags[i] = 0;
  }
  int mype = p_shmem_my_pe();

  p_shmem_barrier_all();

  if (mype == 0) {
    for (int i = 0; i < 4; ++i) {
      flags[i] = 1;
    }
    p_shmem_quiet();
  }

  p_shmem_barrier_all();

  if (mype != 0) {
    long cmp_values[4] = {1, 1, 1, 1};
    time_t start_time = time(NULL);
    while (!p_shmem_long_test_all_vector(flags, 4, NULL, SHMEM_CMP_EQ, cmp_values)) {
      if (time(NULL) - start_time > 10) {
        break;
      }
      sleep(1);
    }
    for (int i = 0; i < 4; ++i) {
      if (flags[i] != 1) {
        p_shmem_free(flags);
        return false;
      }
    }
  }

  p_shmem_free(flags);
  return true;
}

/**
  @brief Tests the shmem_test_any_vector() routine.
  @return True if the test is successful, false otherwise.
*/
bool test_shmem_test_any_vector(void) {
  long *flags = (long *)p_shmem_malloc(4 * sizeof(long));
  if (flags == NULL) {
    return false;
  }

  for (int i = 0; i < 4; ++i) {
    flags[i] = 0;
  }
  int mype = p_shmem_my_pe();
  int npes = p_shmem_n_pes();

  p_shmem_barrier_all();

  if (mype == 0) {
    long one = 1;
    for (int pe = 1; pe < npes; ++pe) {
      p_shmem_long_p(&flags[2], one, pe);
    }
    p_shmem_quiet();
  }

  p_shmem_barrier_all();

  if (mype != 0) {
    long cmp_values[4] = {1, 1, 1, 1};
    time_t start_time = time(NULL);
    while (!p_shmem_long_test_any_vector(flags, 4, NULL, SHMEM_CMP_EQ, cmp_values)) {
      if (time(NULL) - start_time > 10) {
        break;
      }
      sleep(1);
    }
    if (flags[2] != 1) {
      p_shmem_free(flags);
      return false;
    }
  }

  p_shmem_free(flags);
  return true;
}

/**
  @brief Tests the shmem_test_some_vector() routine.
  @return True if the test is successful, false otherwise.
*/
bool test_shmem_test_some_vector(void) {
  long *flags = (long *)p_shmem_malloc(4 * sizeof(long));
  if (flags == NULL) {
    return false;
  }

  for (int i = 0; i < 4; ++i) {
    flags[i] = 0;
  }
  int mype = p_shmem_my_pe();
  int npes = p_shmem_n_pes();

  p_shmem_barrier_all();

  if (mype == 0) {
    long one = 1;
    for (int pe = 1; pe < npes; ++pe) {
      p_shmem_long_p(&flags[1], one, pe);
      p_shmem_long_p(&flags[3], one, pe);
    }
    p_shmem_quiet();
  }

  p_shmem_barrier_all();

  if (mype != 0) {
    long cmp_values[4] = {0, 1, 0, 1};
    size_t indices[4];
    size_t num_indices;
    time_t start_time = time(NULL);
    while ((num_indices = p_shmem_long_test_some_vector(flags, 4, indices, NULL, SHMEM_CMP_EQ, cmp_values)) == 0) {
      if (time(NULL) - start_time > 10) {
        break;
      }
      sleep(1);
    }
    if (flags[1] != 1 || flags[3] != 1) {
      p_shmem_free(flags);
      return false;
    }
  }

  p_shmem_free(flags);
  return true;
}

/**
  @brief Tests the shmem_signal_wait_until() routine.
  @return True if the test is successful, false otherwise.
*/
bool test_shmem_signal_wait_until(void) {
  uint64_t *flag = (uint64_t *)p_shmem_malloc(sizeof(uint64_t));
  if (flag == NULL) {
    return false;
  }

  *flag = 0;
  int mype = p_shmem_my_pe();
  uint64_t value = 1;

  p_shmem_barrier_all();

  if (mype == 0) {
    shmem_uint64_p(flag, value, 1); // Change p_shmem_uint64_p to p_shmem_uint64_p FIXME: figure out how to deal with pointers for different types
    p_shmem_quiet();
  }

  p_shmem_barrier_all();

  if (mype != 0) {
    time_t start_time = time(NULL);
    while (*flag != value && time(NULL) - start_time < 10) {
      p_shmem_signal_wait_until(flag, SHMEM_CMP_EQ, value);
    }
    if (*flag != value) {
      p_shmem_free(flag);
      return false;
    }
  }

  p_shmem_free(flag);
  return true;
}
