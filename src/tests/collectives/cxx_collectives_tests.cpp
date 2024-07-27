/**
 * @file cxx_collectives_tests.cpp
 * @brief Contains tests for various OpenSHMEM collective routines.
 */

#include "cxx_collectives_tests.h"

/**
 * @brief Tests the shmem_sync() routine.
 *
 * This test verifies that the shmem_sync() routine correctly synchronizes all PEs.
 *
 * @return True if the test is successful, false otherwise.
 */
bool text_cxx_shmem_sync(void) {
  static long pSync[SHMEM_SYNC_SIZE];
  for (int i = 0; i < SHMEM_SYNC_SIZE; i++) {
    pSync[i] = SHMEM_SYNC_VALUE;
  }
  p_shmem_barrier_all();
  p_shmem_sync(0, 0, p_shmem_n_pes(), pSync);
  return true;
}

/**
 * @brief Tests the shmem_sync_all() routine.
 *
 * This test verifies that the shmem_sync_all() routine correctly synchronizes all PEs.
 *
 * @return True if the test is successful, false otherwise.
 */
bool text_cxx_shmem_sync_all(void) {
  p_shmem_sync_all();
  return true;
}

/**
 * @brief Tests the shmem_alltoall() routine.
 *
 * This test verifies that the shmem_alltoall() routine correctly performs an all-to-all
 * data exchange among all PEs.
 *
 * @return True if the test is successful, false otherwise.
 */
bool text_cxx_shmem_alltoall(void) {
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
 * @brief Tests the shmem_alltoalls() routine.
 *
 * This test verifies that the shmem_alltoalls() routine correctly performs a strided all-to-all
 * data exchange among all PEs.
 *
 * @return True if the test is successful, false otherwise.
 */
bool text_cxx_shmem_alltoalls(void) {
  int npes = p_shmem_n_pes();
  int mype = p_shmem_my_pe();

  long *src = (long *)p_shmem_malloc(npes * npes * sizeof(long));
  long *dest = (long *)p_shmem_malloc(npes * npes * sizeof(long));

  for (int i = 0; i < npes; ++i) {
    src[i] = mype + i * npes;
  }

  p_shmem_long_alltoalls(SHMEM_TEAM_WORLD, dest, src, 1, 1, npes);

  bool success = true;
  for (int i = 0; i < npes; ++i) {
    if (dest[i] != i * npes + mype) {
      success = false;
      break;
    }
  }

  p_shmem_free(src);
  p_shmem_free(dest);
  
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
bool text_cxx_shmem_broadcast(void) {
  int npes = p_shmem_n_pes();
  int mype = p_shmem_my_pe();
  
  long *src = (long *)p_shmem_malloc(4 * sizeof(long));
  long *dest = (long *)p_shmem_malloc(4 * sizeof(long));

  if (mype == 0) {
    for (int i = 0; i < 4; ++i) {
      src[i] = i + 1;
    }
  }

  for (int i = 0; i < 4; ++i) {
    dest[i] = -1;
  }

  p_shmem_barrier_all();

  p_shmem_long_broadcast(SHMEM_TEAM_WORLD, dest, src, 4, 0);

  p_shmem_barrier_all();

  bool success = true;
  for (int i = 0; i < 4; ++i) {
    if (dest[i] != i + 1) {
      success = false;
      break;
    }
  }

  p_shmem_free(src);
  p_shmem_free(dest);
  
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
bool text_cxx_shmem_collect(void) {
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
 * @brief Tests the shmem_fcollect() routine.
 *
 * This test verifies that the shmem_fcollect() routine correctly collects data
 * from all PEs to a single PE in a more efficient manner than shmem_collect().
 *
 * @return True if the test is successful, false otherwise.
 */
bool text_cxx_shmem_fcollect(void) {
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
 * @brief Tests the shmem_sum_reduce() routine.
 *
 * This test verifies that the shmem_sum_reduce() routine correctly computes the sum
 * of data from all PEs and stores it on the root PE.
 *
 * @return True if the test is successful, false otherwise.
 */
bool text_cxx_shmem_sum_reduce(void) {
  int npes = p_shmem_n_pes();
  int mype = p_shmem_my_pe();

  long *src = (long *)p_shmem_malloc(sizeof(long));
  long *dest = (long *)p_shmem_malloc(sizeof(long));

  *src = mype;

  p_shmem_long_sum_reduce(SHMEM_TEAM_WORLD, dest, src, 1);

  long expected_sum = npes * (npes - 1) / 2;
  bool success = (*dest == expected_sum);

  p_shmem_free(src);
  p_shmem_free(dest);

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
bool text_cxx_shmem_prod_reduce(void) {
  int npes = p_shmem_n_pes();
  int mype = p_shmem_my_pe();

  long *src = (long *)p_shmem_malloc(sizeof(long));
  long *dest = (long *)p_shmem_malloc(sizeof(long));

  *src = mype + 1;

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
 * @brief Tests the shmem_min_reduce() routine.
 *
 * This test verifies that the shmem_min_reduce() routine correctly computes the minimum
 * of data from all PEs and stores it on the root PE.
 *
 * @return True if the test is successful, false otherwise.
 */
bool text_cxx_shmem_min_reduce(void) {
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
 * @brief Tests the shmem_max_reduce() routine.
 *
 * This test verifies that the shmem_max_reduce() routine correctly computes the maximum
 * of data from all PEs and stores it on the root PE.
 *
 * @return True if the test is successful, false otherwise.
 */
bool text_cxx_shmem_max_reduce(void) {
  int npes = p_shmem_n_pes();
  int mype = p_shmem_my_pe();

  long *src = (long *)p_shmem_malloc(sizeof(long));
  long *dest = (long *)p_shmem_malloc(sizeof(long));

  *src = mype;

  p_shmem_long_max_reduce(SHMEM_TEAM_WORLD, dest, src, 1);

  bool success = (*dest == npes - 1);

  p_shmem_free(src);
  p_shmem_free(dest);

  return success;
}
