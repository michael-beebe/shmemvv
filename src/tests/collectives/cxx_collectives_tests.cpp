/**
 * @file cxx_collectives_tests.cpp
 * @brief Contains tests for various OpenSHMEM collective routines.
 */

#include "cxx_collectives_tests.h"

/**
 * @brief Tests the shmem_team_sync() routine.
 *
 * This test verifies that the shmem_team_sync() routine correctly synchronizes all PEs within a team.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_sync(void) {
  static long pSync[SHMEM_SYNC_SIZE];
  static long shared_counter;
  bool success = true;

  for (int i = 0; i < SHMEM_SYNC_SIZE; i++) {
    pSync[i] = SHMEM_SYNC_VALUE;
  }

  shared_counter = 0;
  shmem_barrier_all();

  shmem_team_t team;
  shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(), NULL, 0, &team);

  shmem_atomic_inc(&shared_counter, 0);

  shmem_team_sync(team);

  if (shared_counter != shmem_n_pes()) {
    success = false;
  }

  shmem_team_destroy(team);
  return success;
}

/**
 * @brief Tests the shmem_sync_all() routine.
 *
 * This test verifies that the shmem_sync_all() routine correctly synchronizes all PEs.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_sync_all(void) {
  static long shared_counter;
  bool success = true;

  shared_counter = 0;
  shmem_barrier_all();

  shmem_atomic_inc(&shared_counter, 0);

  shmem_sync_all();

  if (shared_counter != shmem_n_pes()) {
    success = false;
  }

  return success;
}

/**
 * @brief Tests the shmem_alltoall() routine.
 *
 * This test verifies that the shmem_alltoall() routine correctly performs an all-to-all
 * data exchange among all PEs.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_alltoall(void) {
  int npes = shmem_n_pes();
  int mype = shmem_my_pe();
  
  long *src = (long *)shmem_malloc(npes * sizeof(long));
  long *dest = (long *)shmem_malloc(npes * sizeof(long));

  for (int i = 0; i < npes; ++i) {
    src[i] = mype + i;
  }

  shmem_long_alltoall(SHMEM_TEAM_WORLD, dest, src, 1);

  bool success = true;
  for (int i = 0; i < npes; ++i) {
    if (dest[i] != mype + i) {
      success = false;
      break;
    }
  }

  shmem_free(src);
  shmem_free(dest);
  
  return success;
}

/**
 * @brief Tests the shmem_alltoalls() routine.
 *
 * This test verifies that the shmem_alltoalls() routine correctly performs a strided all-to-all
 * data exchange among all PEs.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_alltoalls(void) {
  int npes = shmem_n_pes();
  int mype = shmem_my_pe();

  long *src = (long *)shmem_malloc(npes * npes * sizeof(long));
  long *dest = (long *)shmem_malloc(npes * npes * sizeof(long));

  for (int i = 0; i < npes; ++i) {
    src[i] = mype + i * npes;
  }

  shmem_long_alltoalls(SHMEM_TEAM_WORLD, dest, src, 1, 1, npes);

  bool success = true;
  for (int i = 0; i < npes; ++i) {
    if (dest[i] != i * npes + mype) {
      success = false;
      break;
    }
  }

  shmem_free(src);
  shmem_free(dest);
  
  return success;
}

/**
 * @brief Tests the shmem_broadcast() routine.
 *
 * This test verifies that the shmem_broadcast() routine correctly broadcasts data
 * from the root PE to all other PEs.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_broadcast(void) {
  int npes = shmem_n_pes();
  int mype = shmem_my_pe();
  
  long *src = (long *)shmem_malloc(4 * sizeof(long));
  long *dest = (long *)shmem_malloc(4 * sizeof(long));

  if (mype == 0) {
    for (int i = 0; i < 4; ++i) {
      src[i] = i + 1;
    }
  }

  for (int i = 0; i < 4; ++i) {
    dest[i] = -1;
  }

  shmem_barrier_all();

  shmem_long_broadcast(SHMEM_TEAM_WORLD, dest, src, 4, 0);

  shmem_barrier_all();

  bool success = true;
  for (int i = 0; i < 4; ++i) {
    if (dest[i] != i + 1) {
      success = false;
      break;
    }
  }

  shmem_free(src);
  shmem_free(dest);
  
  return success;
}

/**
 * @brief Tests the shmem_collect() routine.
 *
 * This test verifies that the shmem_collect() routine correctly collects data
 * from all PEs to a single PE.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_collect(void) {
  int npes = shmem_n_pes();
  int mype = shmem_my_pe();
  
  long *src = (long *)shmem_malloc(sizeof(long));
  long *dest = (long *)shmem_malloc(npes * sizeof(long));

  src[0] = mype;

  shmem_long_collect(SHMEM_TEAM_WORLD, dest, src, 1);

  bool success = true;
  for (int i = 0; i < npes; ++i) {
    if (dest[i] != i) {
      success = false;
      break;
    }
  }

  shmem_free(src);
  shmem_free(dest);
  
  return success;
}

/**
 * @brief Tests the shmem_fcollect() routine.
 *
 * This test verifies that the shmem_fcollect() routine correctly collects data
 * from all PEs to a single PE in a more efficient manner than shmem_collect().
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_fcollect(void) {
  int npes = shmem_n_pes();
  int mype = shmem_my_pe();
  
  long *src = (long *)shmem_malloc(sizeof(long));
  long *dest = (long *)shmem_malloc(npes * sizeof(long));

  src[0] = mype;

  shmem_long_fcollect(SHMEM_TEAM_WORLD, dest, src, 1);

  bool success = true;
  for (int i = 0; i < npes; ++i) {
    if (dest[i] != i) {
      success = false;
      break;
    }
  }

  shmem_free(src);
  shmem_free(dest);
  
  return success;
}

/**
 * @brief Tests the shmem_sum_reduce() routine.
 *
 * This test verifies that the shmem_sum_reduce() routine correctly computes the sum
 * of data from all PEs and stores it on the root PE.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_sum_reduce(void) {
  int npes = shmem_n_pes();
  int mype = shmem_my_pe();

  long *src = (long *)shmem_malloc(sizeof(long));
  long *dest = (long *)shmem_malloc(sizeof(long));

  *src = mype;

  shmem_long_sum_reduce(SHMEM_TEAM_WORLD, dest, src, 1);

  long expected_sum = npes * (npes - 1) / 2;
  bool success = (*dest == expected_sum);

  shmem_free(src);
  shmem_free(dest);

  return success;
}

/**
 * @brief Tests the shmem_prod_reduce() routine.
 *
 * This test verifies that the shmem_prod_reduce() routine correctly computes the product
 * of data from all PEs and stores it on the root PE.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_prod_reduce(void) {
  int npes = shmem_n_pes();
  int mype = shmem_my_pe();

  long *src = (long *)shmem_malloc(sizeof(long));
  long *dest = (long *)shmem_malloc(sizeof(long));

  *src = mype + 1;

  shmem_long_prod_reduce(SHMEM_TEAM_WORLD, dest, src, 1);

  long expected_prod = 1;
  for (int i = 1; i <= npes; i++) {
    expected_prod *= i;
  }

  bool success = (*dest == expected_prod);

  shmem_free(src);
  shmem_free(dest);

  return success;
}

/**
 * @brief Tests the shmem_min_reduce() routine.
 *
 * This test verifies that the shmem_min_reduce() routine correctly computes the minimum
 * of data from all PEs and stores it on the root PE.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_min_reduce(void) {
  int npes = shmem_n_pes();
  int mype = shmem_my_pe();

  long *src = (long *)shmem_malloc(sizeof(long));
  long *dest = (long *)shmem_malloc(sizeof(long));

  *src = mype;

  shmem_long_min_reduce(SHMEM_TEAM_WORLD, dest, src, 1);

  bool success = (*dest == 0);

  shmem_free(src);
  shmem_free(dest);

  return success;
}

/**
 * @brief Tests the shmem_max_reduce() routine.
 *
 * This test verifies that the shmem_max_reduce() routine correctly computes the maximum
 * of data from all PEs and stores it on the root PE.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_max_reduce(void) {
  int npes = shmem_n_pes();
  int mype = shmem_my_pe();

  long *src = (long *)shmem_malloc(sizeof(long));
  long *dest = (long *)shmem_malloc(sizeof(long));

  *src = mype;

  shmem_long_max_reduce(SHMEM_TEAM_WORLD, dest, src, 1);

  bool success = (*dest == npes - 1);

  shmem_free(src);
  shmem_free(dest);

  return success;
}

/**
 * @brief Run all C/CXX collectives tests
 */
void run_cxx_collectives_tests(int mype, int npes) {
  /* Check to make sure there are at least 2 PEs */
  if ( !(npes > 1) ) {
    if (mype == 0) {
      display_not_enough_pes("COLLECTIVE OPS");
    }
  }
  else {
    shmem_barrier_all();

    /* Run shmem_team_sync() test */
    bool result_shmem_team_sync = test_shmem_team_sync();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("shmem_team_sync()", result_shmem_team_sync, false);
    }

    /* Run shmem_sync_all() test */
    shmem_barrier_all();
    bool result_shmem_sync_all = test_cxx_shmem_sync_all();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("shmem_sync_all()", result_shmem_sync_all, false);
    }

    /* Run shmem_alltoall() test */
    shmem_barrier_all();
    bool result_shmem_alltoall = test_cxx_shmem_alltoall();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_alltoall()", result_shmem_alltoall, false);
    }

    /* Run shmem_alltoalls() test */
    shmem_barrier_all();
    bool result_shmem_alltoalls = test_cxx_shmem_alltoalls();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_alltoalls()", result_shmem_alltoalls, false);
    }

    /* Run shmem_broadcast() test */
    shmem_barrier_all();
    bool result_shmem_broadcast = test_cxx_shmem_broadcast();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_broadcast()", result_shmem_broadcast, false);
    }

    /* Run shmem_collect() test */
    shmem_barrier_all();
    bool result_shmem_collect = test_cxx_shmem_collect();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_collect()", result_shmem_collect, false);
    }

    /* Run shmem_fcollect() test */
    shmem_barrier_all();
    bool result_shmem_fcollect = test_cxx_shmem_fcollect();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_fcollect()", result_shmem_fcollect, false);
    }

    /* Run shmem_max_reduce() test */
    shmem_barrier_all();
    bool result_shmem_max_reduce = test_cxx_shmem_max_reduce();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_max_reduce()", result_shmem_max_reduce, false);
    }

    /* Run shmem_min_reduce() test */
    shmem_barrier_all();
    bool result_shmem_min_reduce = test_cxx_shmem_min_reduce();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_min_reduce()", result_shmem_min_reduce, false);
    }

    /* Run shmem_sum_reduce() test */
    shmem_barrier_all();
    bool result_shmem_sum_reduce = test_cxx_shmem_sum_reduce();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_sum_reduce()", result_shmem_sum_reduce, false);
    }

    /* Run shmem_prod_reduce() test */
    shmem_barrier_all();
    bool result_shmem_prod_reduce = test_cxx_shmem_prod_reduce();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_prod_reduce()", result_shmem_prod_reduce, false);
    }
  }
}
