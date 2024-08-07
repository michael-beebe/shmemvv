/**
 * @file c11_pt2pt_tests.c 
 * @brief Contains functions definitions with test functions for the point-to-point synchronization routines.
 */

#include "c11_pt2pt_tests.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TIMEOUT 1

/**
 * @brief Tests the shmem_wait_until() routine.
 *
 * This test verifies that the shmem_wait_until() function correctly waits until a condition
 * on a memory location is met.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_wait_until(void) {
  long *flag = (long *)shmem_malloc(sizeof(long));
  if (flag == NULL) {
    return false;
  }

  *flag = 0;
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  shmem_barrier_all();

  if (mype == 0) {
    for (int pe = 1; pe < npes; ++pe) {
      shmem_p(flag, 1, pe);
    }
    shmem_quiet();
  }

  shmem_barrier_all();

  if (mype != 0) {
    shmem_wait_until(flag, SHMEM_CMP_EQ, 1);
    if (*flag != 1) {
      shmem_free(flag);
      return false;
    }
  }

  shmem_free(flag);
  return true;
}

/**
 * @brief Tests the shmem_wait_until_all() routine.
 *
 * This test verifies that the shmem_wait_until_all() function correctly waits until all specified
 * conditions on an array of memory locations are met.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_wait_until_all(void) {
  long *flags = (long *)shmem_malloc(2 * sizeof(long));
  if (flags == NULL) {
    return false;
  }

  flags[0] = 0;
  flags[1] = 0;
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  shmem_barrier_all();

  if (mype == 0) {
    for (int pe = 1; pe < npes; ++pe) {
      shmem_p(&flags[0], 1, pe);
      shmem_p(&flags[1], 1, pe);
    }
    shmem_quiet();
  }

  shmem_barrier_all();

  if (mype != 0) {
    shmem_wait_until_all(flags, 2, NULL, SHMEM_CMP_EQ, 1);
    if (flags[0] != 1 || flags[1] != 1) {
      shmem_free(flags);
      return false;
    }
  }

  shmem_free(flags);
  return true;
}

/**
 * @brief Tests the shmem_wait_until_any() routine.
 *
 * This test verifies that the shmem_wait_until_any() function correctly waits until any one
 * of the specified conditions on an array of memory locations is met.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_wait_until_any(void) {
  long *flags = (long *)shmem_malloc(3 * sizeof(long));
  for (int i = 0; i < 3; i++) {
    flags[i] = 0;
  }
  int mype = shmem_my_pe();

  shmem_barrier_all();

  if (mype == 0) {
    shmem_p(&flags[2], 1, 1);
    shmem_quiet();
  }

  shmem_barrier_all();

  if (mype != 0) {
    int status[3] = {SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ};
    size_t index = shmem_wait_until_any(flags, 3, status, SHMEM_CMP_EQ, 1);
    if (index == SIZE_MAX) {
      shmem_free(flags);
      return false;
    }
    if (flags[index] != 1) {
      shmem_free(flags);
      return false;
    }
  }

  shmem_free(flags);
  return true;
}

/**
 * @brief Tests the shmem_wait_until_some() routine.
 *
 * This test verifies that the shmem_wait_until_some() function correctly waits until some
 * of the specified conditions on an array of memory locations are met.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_wait_until_some(void) {
  long *flags = (long *)shmem_malloc(4 * sizeof(long));
  for (int i = 0; i < 4; ++i) {
    flags[i] = 0;
  }
  int mype = shmem_my_pe();

  shmem_barrier_all();

  if (mype == 0) {
    shmem_p(&flags[1], 1, 1);
    shmem_p(&flags[3], 1, 1);
    shmem_quiet();
  }

  shmem_barrier_all();

  if (mype != 0) {
    size_t indices[4];
    int status[4] = {SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ};
    size_t count = shmem_wait_until_some(flags, 4, indices, status, SHMEM_CMP_EQ, 1);
    if (count < 2) {
      shmem_free(flags);
      return false;
    }
    for (size_t i = 0; i < count; ++i) {
      if (flags[indices[i]] != 1) {
        shmem_free(flags);
        return false;
      }
    }
  }

  shmem_free(flags);
  return true;
}

/**
 * @brief Tests the shmem_test() routine.
 *
 * This test verifies that the shmem_test() function correctly tests whether a condition
 * on a memory location is met without blocking.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_test(void) {
  long *flag = (long *)shmem_malloc(sizeof(long));
  if (flag == NULL) {
    return false;
  }

  *flag = 0;
  int mype = shmem_my_pe();

  shmem_barrier_all();

  if (mype == 0) {
    *flag = 1;
    shmem_quiet();
  }

  shmem_barrier_all();

  if (mype != 0) {
    time_t start_time = time(NULL);
    while (!shmem_test(flag, SHMEM_CMP_EQ, 1)) {
      if (time(NULL) - start_time > TIMEOUT) {
        break;
      }
      usleep(1000);
    }
    if (*flag != 1) {
      shmem_free(flag);
      return false;
    }
  }

  shmem_free(flag);
  return true;
}

/**
 * @brief Tests the shmem_test_all() routine.
 *
 * This test verifies that the shmem_test_all() function correctly tests whether all specified
 * conditions on an array of memory locations are met without blocking.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_test_all(void) {
  long *flags = (long *)shmem_malloc(4 * sizeof(long));
  if (flags == NULL) {
    return false;
  }

  for (int i = 0; i < 4; ++i) {
    flags[i] = 0;
  }
  int mype = shmem_my_pe();

  shmem_barrier_all();

  if (mype == 0) {
    for (int i = 0; i < 4; ++i) {
      flags[i] = 1;
    }
    shmem_quiet();
  }

  shmem_barrier_all();

  if (mype != 0) {
    time_t start_time = time(NULL);
    while (!shmem_test_all(flags, 4, NULL, SHMEM_CMP_EQ, 1)) {
      if (time(NULL) - start_time > TIMEOUT) {
        break;
      }
      usleep(1000);
    }
    for (int i = 0; i < 4; ++i) {
      if (flags[i] != 1) {
        shmem_free(flags);
        return false;
      }
    }
  }

  shmem_free(flags);
  return true;
}

/**
 * @brief Tests the shmem_test_any() routine.
 *
 * This test verifies that the shmem_test_any() function correctly tests whether any one
 * of the specified conditions on an array of memory locations is met without blocking.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_test_any(void) {
  long *flags = (long *)shmem_malloc(4 * sizeof(long));
  if (flags == NULL) {
    return false;
  }

  for (int i = 0; i < 4; ++i) {
    flags[i] = 0;
  }
  int mype = shmem_my_pe();

  shmem_barrier_all();

  if (mype == 0) {
    flags[2] = 1;
    shmem_quiet();
  }

  shmem_barrier_all();

  if (mype != 0) {
    time_t start_time = time(NULL);
    while (!shmem_test_any(flags, 4, NULL, SHMEM_CMP_EQ, 1)) {
      if (time(NULL) - start_time > TIMEOUT) {
        break;
      }
      usleep(1000);
    }
    if (flags[2] != 1) {
      shmem_free(flags);
      return false;
    }
  }

  shmem_free(flags);
  return true;
}

/**
 * @brief Tests the shmem_test_some() routine.
 *
 * This test verifies that the shmem_test_some() function correctly tests whether some
 * of the specified conditions on an array of memory locations are met without blocking.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_test_some(void) {
  long *flags = (long *)shmem_malloc(4 * sizeof(long));
  if (flags == NULL) {
    return false;
  }

  for (int i = 0; i < 4; ++i) {
    flags[i] = 0;
  }
  int mype = shmem_my_pe();

  shmem_barrier_all();

  if (mype == 0) {
    flags[1] = 1;
    flags[3] = 1;
    shmem_quiet();
  }

  shmem_barrier_all();

  if (mype != 0) {
    size_t indices[4];
    time_t start_time = time(NULL);
    while (!shmem_test_some(flags, 4, indices, NULL, SHMEM_CMP_EQ, 1)) {
      if (time(NULL) - start_time > TIMEOUT) {
        break;
      }
      usleep(1000);
    }
    if (flags[1] != 1 || flags[3] != 1) {
      shmem_free(flags);
      return false;
    }
  }

  shmem_free(flags);
  return true;
}

/**
 * @brief Tests the shmem_wait_until_all_vector() routine.
 *
 * This test verifies that the shmem_wait_until_all_vector() function correctly waits until all specified
 * conditions on a vector of memory locations are met.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_wait_until_all_vector(void) {
  long *flags = (long *)shmem_malloc(4 * sizeof(long));
  for (int i = 0; i < 4; ++i) {
    flags[i] = 0;
  }
  int mype = shmem_my_pe();

  shmem_barrier_all();

  if (mype == 0) {
    for (int i = 0; i < 4; ++i) {
      shmem_p(&flags[i], 1, 1);
      shmem_quiet();
    }
  }

  shmem_barrier_all();

  if (mype != 0) {
    int status[4] = {SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ};
    long cmp_values[4] = {1, 1, 1, 1};
    shmem_wait_until_all_vector(flags, 4, status, SHMEM_CMP_EQ, cmp_values);
    for (int i = 0; i < 4; ++i) {
      if (flags[i] != 1) {
        shmem_free(flags);
        return false;
      }
    }
  }

  shmem_free(flags);
  return true;
}

/**
 * @brief Tests the shmem_wait_until_any_vector() routine.
 *
 * This test verifies that the shmem_wait_until_any_vector() function correctly waits until any one
 * of the specified conditions on a vector of memory locations is met.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_wait_until_any_vector(void) {
  long *flags = (long *)shmem_malloc(4 * sizeof(long));
  for (int i = 0; i < 4; ++i) {
    flags[i] = 0;
  }
  int mype = shmem_my_pe();

  shmem_barrier_all();

  if (mype == 0) {
    shmem_p(&flags[2], 1, 1);
    shmem_quiet();
  }

  shmem_barrier_all();

  if (mype != 0) {
    int status[4] = {SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ};
    long cmp_values[4] = {1, 1, 1, 1};
    size_t index = shmem_wait_until_any_vector(flags, 4, status, SHMEM_CMP_EQ, cmp_values);
    if (index == SIZE_MAX) {
      shmem_free(flags);
      return false;
    }
    if (flags[index] != 1) {
      shmem_free(flags);
      return false;
    }
  }

  shmem_free(flags);
  return true;
}

/**
 * @brief Tests the shmem_wait_until_some_vector() routine.
 *
 * This test verifies that the shmem_wait_until_some_vector() function correctly waits until some
 * of the specified conditions on a vector of memory locations are met.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_wait_until_some_vector(void) {
  long *flags = (long *)shmem_malloc(4 * sizeof(long));
  for (int i = 0; i < 4; ++i) {
    flags[i] = 0;
  }
  int mype = shmem_my_pe();

  shmem_barrier_all();

  if (mype == 0) {
    shmem_p(&flags[1], 1, 1);
    shmem_p(&flags[3], 1, 1);
    shmem_quiet();
  }

  shmem_barrier_all();

  if (mype != 0) {
    int status[4] = {SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ};
    long cmp_values[4] = {1, 1, 1, 1};
    size_t indices[4];
    size_t num_indices = shmem_wait_until_some_vector(flags, 4, indices, status, SHMEM_CMP_EQ, cmp_values);
    if (num_indices < 2) {
      shmem_free(flags);
      return false;
    }
    for (size_t i = 0; i < num_indices; ++i) {
      if (flags[indices[i]] != 1) {
        shmem_free(flags);
        return false;
      }
    }
  }

  shmem_free(flags);
  return true;
}

/**
 * @brief Tests the shmem_test_all_vector() routine.
 *
 * This test verifies that the shmem_test_all_vector() function correctly tests whether all specified
 * conditions on a vector of memory locations are met without blocking.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_test_all_vector(void) {
  long *flags = (long *)shmem_malloc(4 * sizeof(long));
  if (flags == NULL) {
    return false;
  }

  for (int i = 0; i < 4; ++i) {
    flags[i] = 0;
  }
  int mype = shmem_my_pe();

  shmem_barrier_all();

  if (mype == 0) {
    for (int i = 0; i < 4; ++i) {
      flags[i] = 1;
    }
    shmem_quiet();
  }

  shmem_barrier_all();

  if (mype != 0) {
    long cmp_values[4] = {1, 1, 1, 1};
    time_t start_time = time(NULL);
    while (!shmem_test_all_vector(flags, 4, NULL, SHMEM_CMP_EQ, cmp_values)) {
      if (time(NULL) - start_time > TIMEOUT) {
        break;
      }
      usleep(1000);
    }
    for (int i = 0; i < 4; ++i) {
      if (flags[i] != 1) {
        shmem_free(flags);
        return false;
      }
    }
  }

  shmem_free(flags);
  return true;
}

/**
 * @brief Tests the shmem_test_any_vector() routine.
 *
 * This test verifies that the shmem_test_any_vector() function correctly tests whether any one
 * of the specified conditions on a vector of memory locations is met without blocking.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_test_any_vector(void) {
  long *flags = (long *)shmem_malloc(4 * sizeof(long));
  if (flags == NULL) {
    return false;
  }

  for (int i = 0; i < 4; ++i) {
    flags[i] = 0;
  }
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  shmem_barrier_all();

  if (mype == 0) {
    long one = 1;
    for (int pe = 1; pe < npes; ++pe) {
      shmem_p(&flags[2], one, pe);
    }
    shmem_quiet();
  }

  shmem_barrier_all();

  if (mype != 0) {
    long cmp_values[4] = {1, 1, 1, 1};
    time_t start_time = time(NULL);
    while (!shmem_test_any_vector(flags, 4, NULL, SHMEM_CMP_EQ, cmp_values)) {
      if (time(NULL) - start_time > TIMEOUT) {
        break;
      }
      usleep(1000);
    }
    if (flags[2] != 1) {
      shmem_free(flags);
      return false;
    }
  }

  shmem_free(flags);
  return true;
}

/**
 * @brief Tests the shmem_test_some_vector() routine.
 *
 * This test verifies that the shmem_test_some_vector() function correctly tests whether some
 * of the specified conditions on a vector of memory locations are met without blocking.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_test_some_vector(void) {
  long *flags = (long *)shmem_malloc(4 * sizeof(long));
  if (flags == NULL) {
    return false;
  }

  for (int i = 0; i < 4; ++i) {
    flags[i] = 0;
  }
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  shmem_barrier_all();

  if (mype == 0) {
    long one = 1;
    for (int pe = 1; pe < npes; ++pe) {
      shmem_p(&flags[1], one, pe);
      shmem_p(&flags[3], one, pe);
    }
    shmem_quiet();
  }

  shmem_barrier_all();

  if (mype != 0) {
    long cmp_values[4] = {0, 1, 0, 1};
    size_t indices[4];
    size_t num_indices;
    time_t start_time = time(NULL);
    while ((num_indices = shmem_test_some_vector(flags, 4, indices, NULL, SHMEM_CMP_EQ, cmp_values)) == 0) {
      if (time(NULL) - start_time > TIMEOUT) {
        break;
      }
      usleep(1000);
    }
    if (flags[1] != 1 || flags[3] != 1) {
      shmem_free(flags);
      return false;
    }
  }

  shmem_free(flags);
  return true;
}

/**
 * @brief Tests the shmem_signal_wait_until() routine.
 *
 * This test verifies that the shmem_signal_wait_until() function correctly waits until a signal
 * on a memory location meets a specified condition.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_signal_wait_until(void) {
  uint64_t *flag = (uint64_t *)shmem_malloc(sizeof(uint64_t));
  if (flag == NULL) {
    return false;
  }

  *flag = 0;
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();
  uint64_t value = 1;

  shmem_barrier_all();

  if (mype == 0) {
    for (int pe = 1; pe < npes; ++pe) {
      shmem_uint64_p(flag, value, pe);
    }
    shmem_quiet();
  }

  shmem_barrier_all();

  if (mype != 0) {
    time_t start_time = time(NULL);
    while (!shmem_test(flag, SHMEM_CMP_EQ, value) && time(NULL) - start_time < TIMEOUT) {
      shmem_signal_wait_until(flag, SHMEM_CMP_EQ, value);
    }
    if (*flag != value) {
      shmem_free(flag);
      return false;
    }
  }

  shmem_free(flag);
  return true;
}

/**
 * @brief Run all C11 tests
 */
void run_c11_pt2pt_synch_tests(int mype, int npes) {
  if (!(npes > 1)) {
    display_not_enough_pes("POINT-TO-POINT SYNCH OPS");
  }
  else {
    /* Run shmem_wait_until() test */
    shmem_barrier_all();
    bool result_shmem_wait_until = test_c11_shmem_wait_until();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_wait_until()", result_shmem_wait_until, false);
    }

    /* Run shmem_wait_until_all() test */
    shmem_barrier_all();
    bool result_shmem_wait_until_all = test_c11_shmem_wait_until_all();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_wait_until_all()", result_shmem_wait_until_all, false);
    }

    /* Run shmem_wait_until_any() test */
    shmem_barrier_all();
    bool result_shmem_wait_until_any = test_c11_shmem_wait_until_any();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_wait_until_any()", result_shmem_wait_until_any, false);
    }

    /* Run shmem_wait_until_some() test */
    shmem_barrier_all();
    bool result_shmem_wait_until_some = test_c11_shmem_wait_until_some();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_wait_until_some()", result_shmem_wait_until_some, false);
    }

    /* Run shmem_wait_until_all_vector() test */
    shmem_barrier_all();
    bool result_shmem_wait_until_all_vector = test_c11_shmem_wait_until_all_vector();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_wait_until_all_vector()", result_shmem_wait_until_all_vector, false);
    }

    /* Run shmem_wait_until_any_vector() test */
    shmem_barrier_all();
    bool result_shmem_wait_until_any_vector = test_c11_shmem_wait_until_any_vector();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_wait_until_any_vector()", result_shmem_wait_until_any_vector, false);
    }

    /* Run shmem_wait_until_some_vector() test */
    shmem_barrier_all();
    bool result_shmem_wait_until_some_vector = test_c11_shmem_wait_until_some_vector();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_wait_until_some_vector()", result_shmem_wait_until_some_vector, false);
    }

    /* Run shmem_test() test */
    shmem_barrier_all();
    bool result_shmem_test = test_c11_shmem_test();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_test()", result_shmem_test, false);
    }

    /* Run shmem_test_all() test */
    shmem_barrier_all();
    bool result_shmem_test_all = test_c11_shmem_test_all();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_test_all()", result_shmem_test_all, false);
    }

    /* Run shmem_test_any() test */
    shmem_barrier_all();
    bool result_shmem_test_any = test_c11_shmem_test_any();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_test_any()", result_shmem_test_any, false);
    }

    /* Run shmem_test_some() test */
    shmem_barrier_all();
    bool result_shmem_test_some = test_c11_shmem_test_some();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_test_some()", result_shmem_test_some, false);
    }

    /* Run shmem_test_all_vector() test */
    shmem_barrier_all();
    bool result_shmem_test_all_vector = test_c11_shmem_test_all_vector();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_test_all_vector()", result_shmem_test_all_vector, false);
    }

    /* Run shmem_test_any_vector() test */
    shmem_barrier_all();
    bool result_shmem_test_any_vector = test_c11_shmem_test_any_vector();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_test_any_vector()", result_shmem_test_any_vector, false);
    }

    /* Run shmem_test_some_vector() test */
    shmem_barrier_all();
    bool result_shmem_test_some_vector = test_c11_shmem_test_some_vector();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_test_some_vector()", result_shmem_test_some_vector, false);
    }

    /* Run shmem_signal_wait_until() test */
    shmem_barrier_all();
    bool result_shmem_signal_wait_until = test_c11_shmem_signal_wait_until();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_signal_wait_until()", result_shmem_signal_wait_until, false); 
    }
  }
}


#ifdef __cplusplus
}
#endif
