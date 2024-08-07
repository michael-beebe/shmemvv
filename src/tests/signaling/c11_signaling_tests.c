/**
 * @file c11_signaling_tests.c
 * @brief Contains OpenSHMEM signaling tests.
 */

#include "c11_signaling_tests.h"

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************/
/**
 * @brief Tests the shmem_put_signal() routine.
 *
 * This test verifies that the shmem_put_signal() function correctly
 * transfers a value and sets a signal on the target PE.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_C11_SHMEM_PUT_SIGNAL(TYPE, TYPENAME)               \
  ({                                                            \
    bool success = true;                                        \
    static TYPE dest = 0;                                       \
    static TYPE value = (TYPE)123;                              \
    static uint64_t signal = 0;                                 \
    int mype = shmem_my_pe();                                   \
    int npes = shmem_n_pes();                                   \
                                                                \
    if (npes < 2) {                                             \
      success = false;                                          \
    } else {                                                    \
      int target_pe = (mype + 1) % npes;                        \
                                                                \
      shmem_barrier_all();                                      \
                                                                \
      if (mype == 0) {                                          \
        shmem_put_signal(&dest, &value, 1, &signal,             \
                                      1, target_pe,             \
                                      SHMEM_SIGNAL_SET);        \
      }                                                         \
                                                                \
      shmem_barrier_all();                                      \
                                                                \
      if (mype == 1) {                                          \
        if (dest != 123 || signal != 1) {                       \
          success = false;                                      \
        }                                                       \
      }                                                         \
    }                                                           \
    success;                                                    \
  })

bool test_c11_shmem_put_signal(void) {
  bool result = true;
  result &= TEST_C11_SHMEM_PUT_SIGNAL(float, float);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(double, double);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(long double, longdouble);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(char, char);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(signed char, schar);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(short, short);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(int, int);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(long, long);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(long long, longlong);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(unsigned char, uchar);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(unsigned short, ushort);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(unsigned int, uint);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(unsigned long, ulong);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(unsigned long long, ulonglong);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(int8_t, int8);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(int16_t, int16);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(int32_t, int32);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(int64_t, int64);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(uint8_t, uint8);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(uint16_t, uint16);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(uint32_t, uint32);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(uint64_t, uint64);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(size_t, size);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_put_signal_nbi() routine.
 *
 * This test verifies that the shmem_put_signal_nbi() function correctly
 * transfers a value and sets a signal on the target PE using non-blocking
 * operations.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_C11_SHMEM_PUT_SIGNAL_NBI(TYPE, TYPENAME)               \
  ({                                                                \
    bool success = true;                                            \
    static TYPE dest = 0;                                           \
    static TYPE value = (TYPE)123;                                  \
    static uint64_t signal = 0;                                     \
    int mype = shmem_my_pe();                                       \
    int npes = shmem_n_pes();                                       \
                                                                    \
    if (npes < 2) {                                                 \
      success = false;                                              \
    } else {                                                        \
      int target_pe = (mype + 1) % npes;                            \
                                                                    \
      shmem_barrier_all();                                          \
                                                                    \
      if (mype == 0) {                                              \
        shmem_put_signal_nbi(&dest, &value, 1,                      \
                                          &signal, 1, target_pe,    \
                                          SHMEM_SIGNAL_SET);        \
        shmem_quiet();                                              \
      }                                                             \
                                                                    \
      shmem_barrier_all();                                          \
                                                                    \
      if (mype == 1) {                                              \
        if (dest != 123 || signal != 1) {                           \
          success = false;                                          \
        }                                                           \
      }                                                             \
    }                                                               \
    success;                                                        \
  })

bool test_c11_shmem_put_signal_nbi(void) {
  bool result = true;
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(float, float);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(double, double);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(long double, longdouble);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(char, char);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(signed char, schar);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(short, short);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(int, int);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(long, long);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(long long, longlong);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(unsigned char, uchar);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(unsigned short, ushort);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(unsigned int, uint);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(unsigned long, ulong);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(unsigned long long, ulonglong);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(int8_t, int8);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(int16_t, int16);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(int32_t, int32);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(int64_t, int64);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(uint8_t, uint8);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(uint16_t, uint16);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(uint32_t, uint32);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(uint64_t, uint64);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(size_t, size);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_signal_fetch() routine.
 *
 * This test verifies that the shmem_signal_fetch() function correctly
 * fetches the signal value from the target PE.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_C11_SHMEM_SIGNAL_FETCH()                         \
  ({                                                          \
    bool success = true;                                      \
    static uint64_t signal = 1;                               \
    uint64_t fetched_signal = 0;                              \
    int mype = shmem_my_pe();                                 \
    int npes = shmem_n_pes();                                 \
                                                              \
    if (npes < 2) {                                           \
      success = false;                                        \
    } else {                                                  \
      shmem_barrier_all();                                    \
                                                              \
      if (mype == 1) {                                        \
        fetched_signal = shmem_signal_fetch(&signal);         \
        if (fetched_signal != 1) {                            \
          success = false;                                    \
        }                                                     \
      }                                                       \
    }                                                         \
    success;                                                  \
  })

bool test_c11_shmem_signal_fetch(void) {
  bool result = true;
  result &= TEST_C11_SHMEM_SIGNAL_FETCH();
  return result;
}


/****************************************************************/
/**
 * @brief Run all C11 signaling tests
 */
/****************************************************************/
void run_c11_signaling_tests(int mype, int npes) {
  /* Check to make sure there are at least 2 PEs */
  if (!(npes > 1)) {
    display_not_enough_pes("SIGNALING OPS");
  }
  else {
    /* Run shmem_put_signal() test */
    shmem_barrier_all();
    bool result_shmem_put_signal = test_c11_shmem_put_signal();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_put_signal()", result_shmem_put_signal, false);
    }

    /* Run shmem_put_signal_nbi() test */
    shmem_barrier_all();
    bool result_shmem_put_signal_nbi = test_c11_shmem_put_signal_nbi();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_put_signal_nbi()", result_shmem_put_signal_nbi, false);
    }

    /* Run shmem_signal_fetch() test */
    shmem_barrier_all();
    bool result_shmem_signal_fetch = test_c11_shmem_signal_fetch();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_signal_fetch()", result_shmem_signal_fetch, false);
    }
  }
}

#ifdef __cplusplus
}
#endif

