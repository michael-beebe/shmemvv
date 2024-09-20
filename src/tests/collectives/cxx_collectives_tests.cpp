/**
 * @file cxx_collectives_tests.cpp
 * @brief Contains tests for various OpenSHMEM collective routines.
 */

#include "cxx_collectives_tests.h"
#include <iostream>

/****************************************************************/
/**
 * @brief Tests the shmem_sync_all() routine.
 *
 * This test verifies that the shmem_sync_all() routine correctly
 * synchronizes all PEs.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_SYNC_ALL()                                              \
  ({                                                                           \
    static long shared_counter;                                                \
    bool success = true;                                                       \
                                                                               \
    shared_counter = 0;                                                        \
    shmem_barrier_all();                                                       \
                                                                               \
    shmem_atomic_inc(&shared_counter, 0);                                      \
                                                                               \
    shmem_sync_all();                                                          \
                                                                               \
    if (shared_counter != shmem_n_pes()) {                                     \
      success = false;                                                         \
    }                                                                          \
                                                                               \
    success;                                                                   \
  })

bool test_cxx_shmem_sync_all(void) { return TEST_CXX_SHMEM_SYNC_ALL(); }

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
#define TEST_CXX_SHMEM_ALLTOALL(BITS)                                          \
  ({                                                                           \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
    long *pSync =                                                              \
        (long *)shmem_malloc(sizeof(long) * SHMEM_ALLTOALL_SYNC_SIZE);         \
    for (size_t i = 0; i < SHMEM_ALLTOALL_SYNC_SIZE; ++i) {                    \
      pSync[i] = SHMEM_SYNC_VALUE;                                             \
    }                                                                          \
                                                                               \
    uint##BITS##_t *src =                                                      \
        (uint##BITS##_t *)shmem_malloc(npes * sizeof(uint##BITS##_t));         \
    uint##BITS##_t *dest =                                                     \
        (uint##BITS##_t *)shmem_malloc(npes * sizeof(uint##BITS##_t));         \
                                                                               \
    for (int i = 0; i < npes; ++i) {                                           \
      src[i] = mype + i;                                                       \
    }                                                                          \
                                                                               \
    shmem_alltoall##BITS(dest, src, 1, 0, 0, npes, pSync);                     \
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
    shmem_free(pSync);                                                         \
                                                                               \
    success;                                                                   \
  })

bool test_cxx_shmem_alltoall(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_ALLTOALL(64);
  result &= TEST_CXX_SHMEM_ALLTOALL(32);
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
#define TEST_CXX_SHMEM_ALLTOALLS(BITS)                                         \
  ({                                                                           \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
    long *pSync =                                                              \
        (long *)shmem_malloc(sizeof(long) * SHMEM_ALLTOALLS_SYNC_SIZE);        \
    for (size_t i = 0; i < SHMEM_ALLTOALLS_SYNC_SIZE; ++i) {                   \
      pSync[i] = SHMEM_SYNC_VALUE;                                             \
    }                                                                          \
                                                                               \
    uint##BITS##_t *src =                                                      \
        (uint##BITS##_t *)shmem_malloc(npes * sizeof(uint##BITS##_t));         \
    uint##BITS##_t *dest =                                                     \
        (uint##BITS##_t *)shmem_malloc(npes * sizeof(uint##BITS##_t));         \
                                                                               \
    for (int i = 0; i < npes; ++i) {                                           \
      src[i] = mype + i * npes;                                                \
    }                                                                          \
                                                                               \
    shmem_alltoalls##BITS(dest, src, 1, 1, npes, 0, 0, npes, pSync);           \
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
    shmem_free(pSync);                                                         \
                                                                               \
    success;                                                                   \
  })

bool test_cxx_shmem_alltoalls(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_ALLTOALLS(64);
  result &= TEST_CXX_SHMEM_ALLTOALLS(32);
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
#define TEST_CXX_SHMEM_BROADCAST(BITS)                                         \
  ({                                                                           \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
    long *pSync = (long *)shmem_malloc(sizeof(long) * SHMEM_BCAST_SYNC_SIZE);  \
    for (size_t i = 0; i < SHMEM_BCAST_SYNC_SIZE; ++i) {                       \
      pSync[i] = SHMEM_SYNC_VALUE;                                             \
    }                                                                          \
                                                                               \
    int##BITS##_t *src =                                                       \
        (int##BITS##_t *)shmem_malloc(4 * sizeof(int##BITS##_t));              \
                                                                               \
    if (mype == 0) {                                                           \
      for (int i = 0; i < 4; ++i) {                                            \
        src[i] = i + 1;                                                        \
      }                                                                        \
    } else {                                                                   \
      for (int i = 0; i < 4; ++i) {                                            \
        src[i] = -1;                                                           \
      }                                                                        \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    shmem_broadcast##BITS(src, src, 4, 0, 0, 0, npes, pSync);                  \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    bool success = true;                                                       \
    for (int i = 0; i < 4; ++i) {                                              \
      if (src[i] != i + 1) {                                                   \
        success = false;                                                       \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
                                                                               \
    shmem_free(src);                                                           \
    shmem_free(pSync);                                                         \
                                                                               \
    success;                                                                   \
  })

bool test_cxx_shmem_broadcast(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_BROADCAST(64);
  result &= TEST_CXX_SHMEM_BROADCAST(32);
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
#define TEST_CXX_SHMEM_COLLECT(BITS)                                           \
  ({                                                                           \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
    long *pSync =                                                              \
        (long *)shmem_malloc(sizeof(long) * SHMEM_COLLECT_SYNC_SIZE);          \
    for (size_t i = 0; i < (SHMEM_COLLECT_SYNC_SIZE / sizeof(long)); ++i) {    \
      pSync[i] = SHMEM_SYNC_VALUE;                                             \
    }                                                                          \
                                                                               \
    uint##BITS##_t *src =                                                      \
        (uint##BITS##_t *)shmem_malloc(sizeof(uint##BITS##_t));                \
    uint##BITS##_t *dest =                                                     \
        (uint##BITS##_t *)shmem_malloc(npes * sizeof(uint##BITS##_t));         \
                                                                               \
    src[0] = mype;                                                             \
                                                                               \
    shmem_collect##BITS(dest, src, 1, 0, 0, npes, pSync);                      \
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
    shmem_free(pSync);                                                         \
                                                                               \
    success;                                                                   \
  })

bool test_cxx_shmem_collect(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_COLLECT(64);
  result &= TEST_CXX_SHMEM_COLLECT(32);
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
#define TEST_CXX_SHMEM_FCOLLECT(BITS)                                          \
  ({                                                                           \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
    long *pSync =                                                              \
        (long *)shmem_malloc(sizeof(long) * SHMEM_COLLECT_SYNC_SIZE);          \
    for (size_t i = 0; i < SHMEM_COLLECT_SYNC_SIZE; ++i) {                     \
      pSync[i] = SHMEM_SYNC_VALUE;                                             \
    }                                                                          \
    uint##BITS##_t *src =                                                      \
        (uint##BITS##_t *)shmem_malloc(sizeof(uint##BITS##_t));                \
    uint##BITS##_t *dest =                                                     \
        (uint##BITS##_t *)shmem_malloc(npes * sizeof(uint##BITS##_t));         \
                                                                               \
    src[0] = mype;                                                             \
                                                                               \
    shmem_fcollect##BITS(dest, src, 1, 0, 0, npes, pSync);                     \
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
    shmem_free(pSync);                                                         \
                                                                               \
    success;                                                                   \
  })

bool test_cxx_shmem_fcollect(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_FCOLLECT(64);
  result &= TEST_CXX_SHMEM_FCOLLECT(32);
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
#define TEST_CXX_SHMEM_SUM_REDUCE(TYPE, TYPENAME)                              \
  ({                                                                           \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
    long *pSync = (long *)shmem_malloc(sizeof(long) * SHMEM_REDUCE_SYNC_SIZE); \
    TYPE *pWrk = (TYPE *)shmem_malloc(sizeof(TYPE) * npes);                    \
    for (size_t i = 0; i < SHMEM_REDUCE_SYNC_SIZE; ++i) {                      \
      pSync[i] = SHMEM_SYNC_VALUE;                                             \
    }                                                                          \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(sizeof(TYPE));                            \
    TYPE *dest = (TYPE *)shmem_malloc(sizeof(TYPE));                           \
                                                                               \
    *src = mype;                                                               \
                                                                               \
    shmem_##TYPENAME##_sum_to_all(dest, src, 1, 0, 0, npes, pWrk, pSync);      \
                                                                               \
    TYPE expected_sum = npes * (npes - 1) / 2;                                 \
    bool success = (*dest == expected_sum);                                    \
                                                                               \
    shmem_free(src);                                                           \
    shmem_free(dest);                                                          \
    shmem_free(pSync);                                                         \
    shmem_free(pWrk);                                                          \
                                                                               \
    success;                                                                   \
  })

bool test_cxx_shmem_sum_reduce(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_SUM_REDUCE(long, long);
  result &= TEST_CXX_SHMEM_SUM_REDUCE(int, int);
  result &= TEST_CXX_SHMEM_SUM_REDUCE(float, float);
  result &= TEST_CXX_SHMEM_SUM_REDUCE(double, double);
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
#define TEST_CXX_SHMEM_PROD_REDUCE(TYPE, TYPENAME)                             \
  ({                                                                           \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
    long *pSync = (long *)shmem_malloc(sizeof(long) * SHMEM_REDUCE_SYNC_SIZE); \
    TYPE *pWrk = (TYPE *)shmem_malloc(sizeof(TYPE) * npes);                    \
    for (size_t i = 0; i < SHMEM_REDUCE_SYNC_SIZE; ++i) {                      \
      pSync[i] = SHMEM_SYNC_VALUE;                                             \
    }                                                                          \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(sizeof(TYPE));                            \
    TYPE *dest = (TYPE *)shmem_malloc(sizeof(TYPE));                           \
                                                                               \
    *src = mype + 1;                                                           \
                                                                               \
    shmem_##TYPENAME##_prod_to_all(dest, src, 1, 0, 0, npes, pWrk, pSync);     \
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
    shmem_free(pSync);                                                         \
    shmem_free(pWrk);                                                          \
                                                                               \
    success;                                                                   \
  })

bool test_cxx_shmem_prod_reduce(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_PROD_REDUCE(long, long);
  result &= TEST_CXX_SHMEM_PROD_REDUCE(int, int);
  result &= TEST_CXX_SHMEM_PROD_REDUCE(float, float);
  result &= TEST_CXX_SHMEM_PROD_REDUCE(double, double);
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
#define TEST_CXX_SHMEM_MIN_REDUCE(TYPE, TYPENAME)                              \
  ({                                                                           \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
    long *pSync = (long *)shmem_malloc(sizeof(long) * SHMEM_REDUCE_SYNC_SIZE); \
    TYPE *pWrk = (TYPE *)shmem_malloc(sizeof(TYPE) * npes);                    \
    for (size_t i = 0; i < SHMEM_REDUCE_SYNC_SIZE; ++i) {                      \
      pSync[i] = SHMEM_SYNC_VALUE;                                             \
    }                                                                          \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(sizeof(TYPE));                            \
    TYPE *dest = (TYPE *)shmem_malloc(sizeof(TYPE));                           \
                                                                               \
    *src = mype;                                                               \
                                                                               \
    shmem_##TYPENAME##_min_to_all(dest, src, 1, 0, 0, npes, pWrk, pSync);      \
                                                                               \
    bool success = (*dest == 0);                                               \
                                                                               \
    shmem_free(src);                                                           \
    shmem_free(dest);                                                          \
    shmem_free(pSync);                                                         \
    shmem_free(pWrk);                                                          \
                                                                               \
    success;                                                                   \
  })

bool test_cxx_shmem_min_reduce(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_MIN_REDUCE(long, long);
  result &= TEST_CXX_SHMEM_MIN_REDUCE(int, int);
  result &= TEST_CXX_SHMEM_MIN_REDUCE(float, float);
  result &= TEST_CXX_SHMEM_MIN_REDUCE(double, double);
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
#define TEST_CXX_SHMEM_MAX_REDUCE(TYPE, TYPENAME)                              \
  ({                                                                           \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
    long *pSync = (long *)shmem_malloc(sizeof(long) * SHMEM_REDUCE_SYNC_SIZE); \
    TYPE *pWrk = (TYPE *)shmem_malloc(sizeof(TYPE) * npes);                    \
    for (size_t i = 0; i < SHMEM_REDUCE_SYNC_SIZE; ++i) {                      \
      pSync[i] = SHMEM_SYNC_VALUE;                                             \
    }                                                                          \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(sizeof(TYPE));                            \
    TYPE *dest = (TYPE *)shmem_malloc(sizeof(TYPE));                           \
                                                                               \
    *src = mype;                                                               \
                                                                               \
    shmem_##TYPENAME##_max_to_all(dest, src, 1, 0, 0, npes, pWrk, pSync);      \
                                                                               \
    bool success = (*dest == npes - 1);                                        \
                                                                               \
    shmem_free(src);                                                           \
    shmem_free(dest);                                                          \
    shmem_free(pSync);                                                         \
    shmem_free(pWrk);                                                          \
                                                                               \
    success;                                                                   \
  })

bool test_cxx_shmem_max_reduce(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_MAX_REDUCE(long, long);
  result &= TEST_CXX_SHMEM_MAX_REDUCE(int, int);
  result &= TEST_CXX_SHMEM_MAX_REDUCE(float, float);
  result &= TEST_CXX_SHMEM_MAX_REDUCE(double, double);
  return result;
}

/****************************************************************/
/**
 * @brief Run all C/CXX collectives tests
 */
/****************************************************************/
void run_cxx_collectives_tests(int mype, int npes) {
  /* Check to make sure there are at least 2 PEs */
  if (!(npes > 1)) {
    if (mype == 0) {
      display_not_enough_pes("COLLECTIVE OPS");
    }
  } else {
    shmem_barrier_all();

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
      display_test_result("C/CXX shmem_alltoall()", result_shmem_alltoall,
                          false);
    }

    /* Run shmem_alltoalls() test */
    shmem_barrier_all();
    bool result_shmem_alltoalls = test_cxx_shmem_alltoalls();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_alltoalls()", result_shmem_alltoalls,
                          false);
    }

    /* Run shmem_broadcast() test */
    shmem_barrier_all();
    bool result_shmem_broadcast = test_cxx_shmem_broadcast();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_broadcast()", result_shmem_broadcast,
                          false);
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
      display_test_result("C/CXX shmem_fcollect()", result_shmem_fcollect,
                          false);
    }

    /* Run shmem_max_reduce() test */
    shmem_barrier_all();
    bool result_shmem_max_reduce = test_cxx_shmem_max_reduce();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_max_reduce()", result_shmem_max_reduce,
                          false);
    }

    /* Run shmem_min_reduce() test */
    shmem_barrier_all();
    bool result_shmem_min_reduce = test_cxx_shmem_min_reduce();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_min_reduce()", result_shmem_min_reduce,
                          false);
    }

    /* Run shmem_sum_reduce() test */
    shmem_barrier_all();
    bool result_shmem_sum_reduce = test_cxx_shmem_sum_reduce();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_sum_reduce()", result_shmem_sum_reduce,
                          false);
    }

    /* Run shmem_prod_reduce() test */
    shmem_barrier_all();
    bool result_shmem_prod_reduce = test_cxx_shmem_prod_reduce();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_prod_reduce()", result_shmem_prod_reduce,
                          false);
    }
  }
}
