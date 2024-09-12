/**
 * @file cxx_pt2pt_tests.cpp
 * @brief Contains functions definitions with test functions for the
 * point-to-point synchronization routines.
 */

#include "cxx_pt2pt_tests.h"

#define TIMEOUT 1

/****************************************************************/
/**
 * @brief Tests the shmem_wait_until() routine.
 *
 * This test verifies that the shmem_wait_until() function correctly waits until
 * a condition on a memory location is met.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_WAIT_UNTIL(TYPE, TYPENAME)                              \
  ({                                                                           \
    bool success = true;                                                       \
    TYPE *flag = (TYPE *)shmem_malloc(sizeof(TYPE));                           \
    if (flag == NULL) {                                                        \
      success = false;                                                         \
    } else {                                                                   \
      *flag = 0;                                                               \
      int mype = shmem_my_pe();                                                \
      int npes = shmem_n_pes();                                                \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 0) {                                                         \
        for (int pe = 1; pe < npes; ++pe) {                                    \
          shmem_##TYPENAME##_p(flag, 1, pe);                                   \
        }                                                                      \
        shmem_quiet();                                                         \
      }                                                                        \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype != 0) {                                                         \
        shmem_##TYPENAME##_wait_until(flag, SHMEM_CMP_EQ, 1);                  \
        if (*flag != 1) {                                                      \
          success = false;                                                     \
        }                                                                      \
      }                                                                        \
      shmem_free(flag);                                                        \
    }                                                                          \
    success;                                                                   \
  })

bool test_cxx_shmem_wait_until(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_WAIT_UNTIL(short, short);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL(int, int);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL(long, long);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL(long long, longlong);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL(unsigned short, ushort);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL(unsigned int, uint);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL(int32_t, int32);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL(int64_t, int64);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL(size_t, size);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_test() routine.
 *
 * This test verifies that the shmem_test() function correctly tests whether a
 * condition on a memory location is met without blocking.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_TEST(TYPE, TYPENAME)                                    \
  ({                                                                           \
    bool success = true;                                                       \
    TYPE *flag = (TYPE *)shmem_malloc(sizeof(TYPE));                           \
    if (flag == NULL) {                                                        \
      success = false;                                                         \
    } else {                                                                   \
      *flag = 0;                                                               \
      int mype = shmem_my_pe();                                                \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 0) {                                                         \
        *flag = 1;                                                             \
        shmem_quiet();                                                         \
      }                                                                        \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype != 0) {                                                         \
        time_t start_time = time(NULL);                                        \
        while (!shmem_##TYPENAME##_test(flag, SHMEM_CMP_EQ, 1)) {              \
          if (time(NULL) - start_time > TIMEOUT) {                             \
            break;                                                             \
          }                                                                    \
          usleep(1000);                                                        \
        }                                                                      \
        if (*flag != 1) {                                                      \
          success = false;                                                     \
        }                                                                      \
      }                                                                        \
      shmem_free(flag);                                                        \
    }                                                                          \
    success;                                                                   \
  })

bool test_cxx_shmem_test(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_TEST(short, short);
  result &= TEST_CXX_SHMEM_TEST(int, int);
  result &= TEST_CXX_SHMEM_TEST(long, long);
  result &= TEST_CXX_SHMEM_TEST(long long, longlong);
  result &= TEST_CXX_SHMEM_TEST(unsigned short, ushort);
  result &= TEST_CXX_SHMEM_TEST(unsigned int, uint);
  result &= TEST_CXX_SHMEM_TEST(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_TEST(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_TEST(int32_t, int32);
  result &= TEST_CXX_SHMEM_TEST(int64_t, int64);
  result &= TEST_CXX_SHMEM_TEST(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_TEST(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_TEST(size_t, size);
  result &= TEST_CXX_SHMEM_TEST(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Run all C/CXX pt2pt synchronization tests
 */
/****************************************************************/
void run_cxx_pt2pt_synch_tests(int mype, int npes) {
  if (!(npes > 1)) {
    display_not_enough_pes("POINT-TO-POINT SYNCH OPS");
  } else {
    /* Run shmem_wait_until() test */
    shmem_barrier_all();
    bool result_shmem_wait_until = test_cxx_shmem_wait_until();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_wait_until()", result_shmem_wait_until,
                          false);
    }

    /* Run shmem_test() test */
    shmem_barrier_all();
    bool result_shmem_test = test_cxx_shmem_test();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_test()", result_shmem_test, false);
    }
  }
}
