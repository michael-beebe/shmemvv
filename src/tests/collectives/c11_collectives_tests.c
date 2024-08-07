/**
 * @file c11_collectives_tests.c
 * @brief Contains tests for various OpenSHMEM collective routines.
 */

#include "c11_collectives_tests.h"

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************/
/**
 * @brief Tests the shmem_sync() routine.
 *
 * This test verifies that the shmem_sync() routine correctly
 * synchronizes all PEs.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_C11_SHMEM_SYNC()                                                  \
  ({                                                                           \
    bool success = true;                                                       \
    static long pSync[SHMEM_SYNC_SIZE];                                        \
    for (int i = 0; i < SHMEM_SYNC_SIZE; i++) {                                \
      pSync[i] = SHMEM_SYNC_VALUE;                                             \
    }                                                                          \
    shmem_barrier_all();                                                       \
    shmem_sync(0, 0, shmem_n_pes(), pSync);                                    \
    success;                                                                   \
  })

bool test_c11_shmem_sync(void) { return TEST_C11_SHMEM_SYNC(); }

/****************************************************************/
/**
 * @brief Tests the shmem_alltoall() routine.
 *
 * This test verifies that the shmem_alltoall() routine correctly
 * performs an all-to-all data exchange among all PEs.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_C11_SHMEM_ALLTOALL(TYPE, TYPENAME)                                \
  ({                                                                           \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(npes * sizeof(TYPE));                     \
    TYPE *dest = (TYPE *)shmem_malloc(npes * sizeof(TYPE));                    \
                                                                               \
    for (int i = 0; i < npes; ++i) {                                           \
      src[i] = mype + i;                                                       \
    }                                                                          \
                                                                               \
    shmem_alltoall(SHMEM_TEAM_WORLD, dest, src, 1);                            \
                                                                               \
    bool success = true;                                                       \
    for (int i = 0; i < npes; ++i) {                                           \
      if (dest[i] != mype + i) {                                               \
        success = false;                                                       \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
                                                                               \
    shmem_free(src);                                                           \
    shmem_free(dest);                                                          \
                                                                               \
    success;                                                                   \
  })

bool test_c11_shmem_alltoall(void) {
  bool result = true;
  result &= TEST_C11_SHMEM_ALLTOALL(long, long);
  result &= TEST_C11_SHMEM_ALLTOALL(int, int);
  result &= TEST_C11_SHMEM_ALLTOALL(float, float);
  result &= TEST_C11_SHMEM_ALLTOALL(double, double);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_alltoalls() routine.
 *
 * This test verifies that the shmem_alltoalls() routine correctly
 * performs a strided all-to-all data exchange among all PEs.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_C11_SHMEM_ALLTOALLS(TYPE, TYPENAME)                               \
  ({                                                                           \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(npes * npes * sizeof(TYPE));              \
    TYPE *dest = (TYPE *)shmem_malloc(npes * npes * sizeof(TYPE));             \
                                                                               \
    for (int i = 0; i < npes; ++i) {                                           \
      src[i] = mype + i * npes;                                                \
    }                                                                          \
                                                                               \
    shmem_alltoalls(SHMEM_TEAM_WORLD, dest, src, 1, 1, npes);                  \
                                                                               \
    bool success = true;                                                       \
    for (int i = 0; i < npes; ++i) {                                           \
      if (dest[i] != i * npes + mype) {                                        \
        success = false;                                                       \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
                                                                               \
    shmem_free(src);                                                           \
    shmem_free(dest);                                                          \
                                                                               \
    success;                                                                   \
  })

bool test_c11_shmem_alltoalls(void) {
  bool result = true;
  result &= TEST_C11_SHMEM_ALLTOALLS(long, long);
  result &= TEST_C11_SHMEM_ALLTOALLS(int, int);
  result &= TEST_C11_SHMEM_ALLTOALLS(float, float);
  result &= TEST_C11_SHMEM_ALLTOALLS(double, double);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_broadcast() routine.
 *
 * This test verifies that the shmem_broadcast() routine correctly
 * broadcasts data from the root PE to all other PEs.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_C11_SHMEM_BROADCAST(TYPE, TYPENAME)                               \
  ({                                                                           \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(4 * sizeof(TYPE));                        \
    TYPE *dest = (TYPE *)shmem_malloc(4 * sizeof(TYPE));                       \
                                                                               \
    if (mype == 0) {                                                           \
      for (int i = 0; i < 4; ++i) {                                            \
        src[i] = i + 1;                                                        \
      }                                                                        \
    }                                                                          \
                                                                               \
    for (int i = 0; i < 4; ++i) {                                              \
      dest[i] = -1;                                                            \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    shmem_broadcast(SHMEM_TEAM_WORLD, dest, src, 4, 0);                        \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    bool success = true;                                                       \
    for (int i = 0; i < 4; ++i) {                                              \
      if (dest[i] != i + 1) {                                                  \
        success = false;                                                       \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
                                                                               \
    shmem_free(src);                                                           \
    shmem_free(dest);                                                          \
                                                                               \
    success;                                                                   \
  })

bool test_c11_shmem_broadcast(void) {
  bool result = true;
  result &= TEST_C11_SHMEM_BROADCAST(long, long);
  result &= TEST_C11_SHMEM_BROADCAST(int, int);
  result &= TEST_C11_SHMEM_BROADCAST(float, float);
  result &= TEST_C11_SHMEM_BROADCAST(double, double);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_collect() routine.
 *
 * This test verifies that the shmem_collect() routine correctly
 * collects data from all PEs to a single PE.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_C11_SHMEM_COLLECT(TYPE, TYPENAME)                                 \
  ({                                                                           \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(sizeof(TYPE));                            \
    TYPE *dest = (TYPE *)shmem_malloc(npes * sizeof(TYPE));                    \
                                                                               \
    src[0] = mype;                                                             \
                                                                               \
    shmem_collect(SHMEM_TEAM_WORLD, dest, src, 1);                             \
                                                                               \
    bool success = true;                                                       \
    for (int i = 0; i < npes; ++i) {                                           \
      if (dest[i] != i) {                                                      \
        success = false;                                                       \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
                                                                               \
    shmem_free(src);                                                           \
    shmem_free(dest);                                                          \
                                                                               \
    success;                                                                   \
  })

bool test_c11_shmem_collect(void) {
  bool result = true;
  result &= TEST_C11_SHMEM_COLLECT(long, long);
  result &= TEST_C11_SHMEM_COLLECT(int, int);
  result &= TEST_C11_SHMEM_COLLECT(float, float);
  result &= TEST_C11_SHMEM_COLLECT(double, double);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_fcollect() routine.
 *
 * This test verifies that the shmem_fcollect() routine correctly
 * collects data from all PEs to a single PE in a more efficient
 * manner than shmem_collect().
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_C11_SHMEM_FCOLLECT(TYPE, TYPENAME)                                \
  ({                                                                           \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(sizeof(TYPE));                            \
    TYPE *dest = (TYPE *)shmem_malloc(npes * sizeof(TYPE));                    \
                                                                               \
    src[0] = mype;                                                             \
                                                                               \
    shmem_fcollect(SHMEM_TEAM_WORLD, dest, src, 1);                            \
                                                                               \
    bool success = true;                                                       \
    for (int i = 0; i < npes; ++i) {                                           \
      if (dest[i] != i) {                                                      \
        success = false;                                                       \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
                                                                               \
    shmem_free(src);                                                           \
    shmem_free(dest);                                                          \
                                                                               \
    success;                                                                   \
  })

bool test_c11_shmem_fcollect(void) {
  bool result = true;
  result &= TEST_C11_SHMEM_FCOLLECT(long, long);
  result &= TEST_C11_SHMEM_FCOLLECT(int, int);
  result &= TEST_C11_SHMEM_FCOLLECT(float, float);
  result &= TEST_C11_SHMEM_FCOLLECT(double, double);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_sum_reduce() routine.
 *
 * This test verifies that the shmem_sum_reduce() routine
 * correctly computes the sum of data from all PEs and stores
 * it on the root PE.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_C11_SHMEM_SUM_REDUCE(TYPE, TYPENAME)                              \
  ({                                                                           \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(sizeof(TYPE));                            \
    TYPE *dest = (TYPE *)shmem_malloc(sizeof(TYPE));                           \
                                                                               \
    *src = mype;                                                               \
                                                                               \
    shmem_sum_reduce(SHMEM_TEAM_WORLD, dest, src, 1);                          \
                                                                               \
    TYPE expected_sum = npes * (npes - 1) / 2;                                 \
    bool success = (*dest == expected_sum);                                    \
                                                                               \
    shmem_free(src);                                                           \
    shmem_free(dest);                                                          \
                                                                               \
    success;                                                                   \
  })

bool test_c11_shmem_sum_reduce(void) {
  bool result = true;
  result &= TEST_C11_SHMEM_SUM_REDUCE(long, long);
  result &= TEST_C11_SHMEM_SUM_REDUCE(int, int);
  result &= TEST_C11_SHMEM_SUM_REDUCE(float, float);
  result &= TEST_C11_SHMEM_SUM_REDUCE(double, double);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_prod_reduce() routine.
 *
 * This test verifies that the shmem_prod_reduce() routine
 * correctly computes the product of data from all PEs and stores
 * it on the root PE.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_C11_SHMEM_PROD_REDUCE(TYPE, TYPENAME)                             \
  ({                                                                           \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(sizeof(TYPE));                            \
    TYPE *dest = (TYPE *)shmem_malloc(sizeof(TYPE));                           \
                                                                               \
    *src = mype + 1;                                                           \
                                                                               \
    shmem_prod_reduce(SHMEM_TEAM_WORLD, dest, src, 1);                         \
                                                                               \
    TYPE expected_prod = 1;                                                    \
    for (int i = 1; i <= npes; i++) {                                          \
      expected_prod *= i;                                                      \
    }                                                                          \
                                                                               \
    bool success = (*dest == expected_prod);                                   \
                                                                               \
    shmem_free(src);                                                           \
    shmem_free(dest);                                                          \
                                                                               \
    success;                                                                   \
  })

bool test_c11_shmem_prod_reduce(void) {
  bool result = true;
  result &= TEST_C11_SHMEM_PROD_REDUCE(long, long);
  result &= TEST_C11_SHMEM_PROD_REDUCE(int, int);
  result &= TEST_C11_SHMEM_PROD_REDUCE(float, float);
  result &= TEST_C11_SHMEM_PROD_REDUCE(double, double);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_min_reduce() routine.
 *
 * This test verifies that the shmem_min_reduce() routine correctly
 * computes the minimum of data from all PEs and stores it on the root PE.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_C11_SHMEM_MIN_REDUCE(TYPE, TYPENAME)                              \
  ({                                                                           \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(sizeof(TYPE));                            \
    TYPE *dest = (TYPE *)shmem_malloc(sizeof(TYPE));                           \
                                                                               \
    *src = mype;                                                               \
                                                                               \
    shmem_min_reduce(SHMEM_TEAM_WORLD, dest, src, 1);                          \
                                                                               \
    bool success = (*dest == 0);                                               \
                                                                               \
    shmem_free(src);                                                           \
    shmem_free(dest);                                                          \
                                                                               \
    success;                                                                   \
  })

bool test_c11_shmem_min_reduce(void) {
  bool result = true;
  result &= TEST_C11_SHMEM_MIN_REDUCE(long, long);
  result &= TEST_C11_SHMEM_MIN_REDUCE(int, int);
  result &= TEST_C11_SHMEM_MIN_REDUCE(float, float);
  result &= TEST_C11_SHMEM_MIN_REDUCE(double, double);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_max_reduce() routine.
 *
 * This test verifies that the shmem_max_reduce() routine
 * correctly computes the maximum of data from all PEs and stores
 * it on the root PE.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_C11_SHMEM_MAX_REDUCE(TYPE, TYPENAME)                              \
  ({                                                                           \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(sizeof(TYPE));                            \
    TYPE *dest = (TYPE *)shmem_malloc(sizeof(TYPE));                           \
                                                                               \
    *src = mype;                                                               \
                                                                               \
    shmem_max_reduce(SHMEM_TEAM_WORLD, dest, src, 1);                          \
                                                                               \
    bool success = (*dest == npes - 1);                                        \
                                                                               \
    shmem_free(src);                                                           \
    shmem_free(dest);                                                          \
                                                                               \
    success;                                                                   \
  })

bool test_c11_shmem_max_reduce(void) {
  bool result = true;
  result &= TEST_C11_SHMEM_MAX_REDUCE(long, long);
  result &= TEST_C11_SHMEM_MAX_REDUCE(int, int);
  result &= TEST_C11_SHMEM_MAX_REDUCE(float, float);
  result &= TEST_C11_SHMEM_MAX_REDUCE(double, double);
  return result;
}

/****************************************************************/
/**
 * @brief Run all C11 collectives tests
 */
/****************************************************************/
void run_c11_collectives_tests(int mype, int npes) {
  /* Check to make sure there are at least 2 PEs */
  if (!(npes > 1)) {
    if (mype == 0) {
      display_not_enough_pes("COLLECTIVE OPS");
    }
  } else {
    /* Run shmem_sync() test */
    shmem_barrier_all();
    bool result_shmem_sync = test_c11_shmem_sync();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("shmem_sync()", result_shmem_sync, false);
    }

    /* Run shmem_alltoall() test */
    shmem_barrier_all();
    bool result_shmem_alltoall = test_c11_shmem_alltoall();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_alltoall()", result_shmem_alltoall, false);
    }

    /* Run shmem_alltoalls() test */
    shmem_barrier_all();
    bool result_shmem_alltoalls = test_c11_shmem_alltoalls();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_alltoalls()", result_shmem_alltoalls,
                          false);
    }

    /* Run shmem_broadcast() test */
    shmem_barrier_all();
    bool result_shmem_broadcast = test_c11_shmem_broadcast();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_broadcast()", result_shmem_broadcast,
                          false);
    }

    /* Run shmem_collect() test */
    shmem_barrier_all();
    bool result_shmem_collect = test_c11_shmem_collect();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_collect()", result_shmem_collect, false);
    }

    /* Run shmem_fcollect() test */
    shmem_barrier_all();
    bool result_shmem_fcollect = test_c11_shmem_fcollect();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_fcollect()", result_shmem_fcollect, false);
    }

    /* Run shmem_max_reduce() test */
    shmem_barrier_all();
    bool result_shmem_max_reduce = test_c11_shmem_max_reduce();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_max_reduce()", result_shmem_max_reduce,
                          false);
    }

    /* Run shmem_min_reduce() test */
    shmem_barrier_all();
    bool result_shmem_min_reduce = test_c11_shmem_min_reduce();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_min_reduce()", result_shmem_min_reduce,
                          false);
    }

    /* Run shmem_sum_reduce() test */
    shmem_barrier_all();
    bool result_shmem_sum_reduce = test_c11_shmem_sum_reduce();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_sum_reduce()", result_shmem_sum_reduce,
                          false);
    }

    /* Run shmem_prod_reduce() test */
    shmem_barrier_all();
    bool result_shmem_prod_reduce = test_c11_shmem_prod_reduce();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_prod_reduce()", result_shmem_prod_reduce,
                          false);
    }
  }
}

#ifdef __cplusplus
}
#endif
