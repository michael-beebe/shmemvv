#include "collectives_tests.hpp"
#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>

/**
  @brief Tests the shmem_sync() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_sync(void) {
  static long pSync[SHMEM_SYNC_SIZE];
  for (int i = 0; i < SHMEM_SYNC_SIZE; i++) {
    pSync[i] = SHMEM_SYNC_VALUE;
  }
  shmem_barrier_all();
  shmem_sync(0, 0, shmem_n_pes(), pSync);
  return true;
}

/**
  @brief Tests the shmem_sync_all() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_sync_all(void) {
  shmem_sync_all();
  return true;
}

/**
  @brief Tests the shmem_alltoall() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_alltoall(void) {
  int npes = shmem_n_pes();
  int mype = shmem_my_pe();
  
  int *src = (int *)shmem_malloc(npes * sizeof(int));
  int *dest = (int *)shmem_malloc(npes * sizeof(int));

  for (int i = 0; i < npes; ++i) {
    src[i] = mype + i;
  }

  shmem_int_alltoall(SHMEM_TEAM_WORLD, dest, src, 1);

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
  @brief Tests the shmem_alltoalls() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_alltoalls(void) {
  int npes = shmem_n_pes();
  int mype = shmem_my_pe();
  
  int *src = (int *)shmem_malloc(npes * npes * sizeof(int));
  int *dest = (int *)shmem_malloc(npes * npes * sizeof(int));

  for (int i = 0; i < npes * npes; ++i) {
    src[i] = mype + i;
  }

  shmem_int_alltoalls(SHMEM_TEAM_WORLD, dest, src, 1, 1, npes);

  bool success = true;
  for (int i = 0; i < npes * npes; ++i) {
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
  @brief Tests the shmem_broadcast() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_broadcast(void) {
  int npes = shmem_n_pes();
  int mype = shmem_my_pe();
  
  int *src = (int *)shmem_malloc(4 * sizeof(int));
  int *dest = (int *)shmem_malloc(4 * sizeof(int));

  for (int i = 0; i < 4; ++i) {
    src[i] = mype;
  }

  shmem_int_broadcast(SHMEM_TEAM_WORLD, dest, src, 4, 0);

  bool success = true;
  for (int i = 0; i < 4; ++i) {
    if (dest[i] != src[i]) {
      success = false;
      break;
    }
  }

  shmem_free(src);
  shmem_free(dest);
  
  return success;
}

/**
  @brief Tests the shmem_collect() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_collect(void) {
  int npes = shmem_n_pes();
  int mype = shmem_my_pe();
  
  int *src = (int *)shmem_malloc(sizeof(int));
  int *dest = (int *)shmem_malloc(npes * sizeof(int));

  src[0] = mype;

  shmem_int_collect(SHMEM_TEAM_WORLD, dest, src, 1);

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
  @brief Tests the shmem_fcollect() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_fcollect(void) {
  int npes = shmem_n_pes();
  int mype = shmem_my_pe();
  
  int *src = (int *)shmem_malloc(sizeof(int));
  int *dest = (int *)shmem_malloc(npes * sizeof(int));

  src[0] = mype;

  shmem_int_fcollect(SHMEM_TEAM_WORLD, dest, src, 1);

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
  @brief Tests the shmem_sum_reduce() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_sum_reduce(void) {
  int npes = shmem_n_pes();
  int mype = shmem_my_pe();

  long *src = (long *)shmem_malloc(sizeof(long));
  long *dest = (long *)shmem_malloc(sizeof(long));

  *src = mype;

  shmem_long_sum_reduce(SHMEM_TEAM_WORLD, dest, src, 1);

  long expected_sum = npes * (npes - 1) / 2; // Sum of PE ranks from 0 to (npes-1)
  bool success = (*dest == expected_sum);

  shmem_free(src);
  shmem_free(dest);

  return success;
}

/**
  @brief Tests the shmem_prod_reduce() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_prod_reduce(void) {
  int npes = shmem_n_pes();
  int mype = shmem_my_pe();

  long *src = (long *)shmem_malloc(sizeof(long));
  long *dest = (long *)shmem_malloc(sizeof(long));

  *src = mype + 1; // To avoid zero which will make the product zero

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
  @brief Tests the shmem_min_reduce() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_min_reduce(void) {
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
  @brief Tests the shmem_max_reduce() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_max_reduce(void) {
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
