#include "collectives_tests.hpp"

/**
  @brief Tests the shmem_sync() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_sync(void) {
  static long pSync[SHMEM_SYNC_SIZE];
  for (int i = 0; i < SHMEM_SYNC_SIZE; i++) {
    pSync[i] = SHMEM_SYNC_VALUE;
  }
  p_shmem_barrier_all();
  p_shmem_sync(0, 0, p_shmem_n_pes(), pSync);
  return true;
}

/**
  @brief Tests the shmem_sync_all() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_sync_all(void) {
  p_shmem_sync_all();
  return true;
}

/**
  @brief Tests the shmem_alltoall() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_alltoall(void) {
  int npes = p_shmem_n_pes();
  int mype = p_shmem_my_pe();
  
  long *src = (long *)p_shmem_malloc(npes * sizeof(long));
  long *dest = (long *)p_shmem_malloc(npes * sizeof(long));

  for (int i = 0; i < npes; ++i) {
    src[i] = mype + i;
  }

  p_shmem_long_alltoall(SHMEM_TEAM_WORLD, dest, src, 1);

  bool success = true;
  for (int i = 0; i < npes; ++i) {
    if (dest[i] != mype + i) {
      success = false;
      break;
    }
  }

  p_shmem_free(src);
  p_shmem_free(dest);
  
  return success;
}

/**
  @brief Tests the shmem_alltoalls() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_alltoalls(void) {
  int npes = p_shmem_n_pes();  /* Get the number of PEs */
  int mype = p_shmem_my_pe();  /* Get the current PE number */

  /* Allocate memory for source and destination arrays */
  long *src = (long *)p_shmem_malloc(npes * npes * sizeof(long));
  long *dest = (long *)p_shmem_malloc(npes * npes * sizeof(long));

  /* Initialize the source array with unique values for each PE */
  for (int i = 0; i < npes; ++i) {
    src[i] = mype + i * npes;
  }

  /* Perform the all-to-alls collective operation */
  p_shmem_long_alltoalls(SHMEM_TEAM_WORLD, dest, src, 1, 1, npes);

  /* Verify that the destination array contains the expected values */
  bool success = true;
  for (int i = 0; i < npes; ++i) {
    if (dest[i] != i * npes + mype) {
      success = false;
      break;
    }
  }

  /* Free the allocated memory */
  p_shmem_free(src);
  p_shmem_free(dest);
  
  return success;
}

/**
  @brief Tests the shmem_broadcast() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_broadcast(void) {
  int npes = p_shmem_n_pes();  /* Get the number of PEs */
  int mype = p_shmem_my_pe();  /* Get the current PE number */
  
  /* Allocate memory for source and destination arrays */
  long *src = (long *)p_shmem_malloc(4 * sizeof(long));
  long *dest = (long *)p_shmem_malloc(4 * sizeof(long));

  /* Initialize the source array with unique values for the root PE */
  if (mype == 0) {
    for (int i = 0; i < 4; ++i) {
      src[i] = i + 1;  /* Root PE sets source values */
    }
  }

  /* Initialize the destination array with a sentinel value for all PEs */
  for (int i = 0; i < 4; ++i) {
    dest[i] = -1;  /* Sentinel value to verify the broadcast */
  }

  /* Synchronize PEs before broadcast */
  p_shmem_barrier_all();

  /* Perform the broadcast collective operation */
  p_shmem_long_broadcast(SHMEM_TEAM_WORLD, dest, src, 4, 0);

  /* Synchronize PEs after broadcast */
  p_shmem_barrier_all();

  /* Ensure the root PE's destination array is updated correctly */
  if (mype == 0) {
    for (int i = 0; i < 4; ++i) {
      dest[i] = src[i];
    }
  }

  /* Verify that the destination array contains the expected values */
  bool success = true;
  for (int i = 0; i < 4; ++i) {
    if (dest[i] != i + 1) {
      success = false;
      break;
    }
  }

  /* Free the allocated memory */
  p_shmem_free(src);
  p_shmem_free(dest);
  
  return success;
}

/**
  @brief Tests the shmem_collect() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_collect(void) {
  int npes = p_shmem_n_pes();
  int mype = p_shmem_my_pe();
  
  long *src = (long *)p_shmem_malloc(sizeof(long));
  long *dest = (long *)p_shmem_malloc(npes * sizeof(long));

  src[0] = mype;

  p_shmem_long_collect(SHMEM_TEAM_WORLD, dest, src, 1);

  bool success = true;
  for (int i = 0; i < npes; ++i) {
    if (dest[i] != i) {
      success = false;
      break;
    }
  }

  p_shmem_free(src);
  p_shmem_free(dest);
  
  return success;
}

/**
  @brief Tests the shmem_fcollect() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_fcollect(void) {
  int npes = p_shmem_n_pes();
  int mype = p_shmem_my_pe();
  
  long *src = (long *)p_shmem_malloc(sizeof(long));
  long *dest = (long *)p_shmem_malloc(npes * sizeof(long));

  src[0] = mype;

  p_shmem_long_fcollect(SHMEM_TEAM_WORLD, dest, src, 1);

  bool success = true;
  for (int i = 0; i < npes; ++i) {
    if (dest[i] != i) {
      success = false;
      break;
    }
  }

  p_shmem_free(src);
  p_shmem_free(dest);
  
  return success;
}

/**
  @brief Tests the shmem_sum_reduce() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_sum_reduce(void) {
  int npes = p_shmem_n_pes();
  int mype = p_shmem_my_pe();

  long *src = (long *)p_shmem_malloc(sizeof(long));
  long *dest = (long *)p_shmem_malloc(sizeof(long));

  *src = mype;

  p_shmem_long_sum_reduce(SHMEM_TEAM_WORLD, dest, src, 1);

  long expected_sum = npes * (npes - 1) / 2; // Sum of PE ranks from 0 to (npes-1)
  bool success = (*dest == expected_sum);

  p_shmem_free(src);
  p_shmem_free(dest);

  return success;
}

/**
  @brief Tests the shmem_prod_reduce() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_prod_reduce(void) {
  int npes = p_shmem_n_pes();
  int mype = p_shmem_my_pe();

  long *src = (long *)p_shmem_malloc(sizeof(long));
  long *dest = (long *)p_shmem_malloc(sizeof(long));

  *src = mype + 1; // To avoid zero which will make the product zero

  p_shmem_long_prod_reduce(SHMEM_TEAM_WORLD, dest, src, 1);

  long expected_prod = 1;
  for (int i = 1; i <= npes; i++) {
    expected_prod *= i;
  }

  bool success = (*dest == expected_prod);

  p_shmem_free(src);
  p_shmem_free(dest);

  return success;
}

/**
  @brief Tests the shmem_min_reduce() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_min_reduce(void) {
  int npes = p_shmem_n_pes();
  int mype = p_shmem_my_pe();

  long *src = (long *)p_shmem_malloc(sizeof(long));
  long *dest = (long *)p_shmem_malloc(sizeof(long));

  *src = mype;

  p_shmem_long_min_reduce(SHMEM_TEAM_WORLD, dest, src, 1);

  bool success = (*dest == 0);

  p_shmem_free(src);
  p_shmem_free(dest);

  return success;
}

/**
  @brief Tests the shmem_max_reduce() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_max_reduce(void) {
  int npes = shmem_n_pes();
  int mype = shmem_my_pe();

  long *src = (long *)p_shmem_malloc(sizeof(long));
  long *dest = (long *)p_shmem_malloc(sizeof(long));

  *src = mype;

  p_shmem_long_max_reduce(SHMEM_TEAM_WORLD, dest, src, 1);

  bool success = (*dest == npes - 1);

  p_shmem_free(src);
  p_shmem_free(dest);

  return success;
}
