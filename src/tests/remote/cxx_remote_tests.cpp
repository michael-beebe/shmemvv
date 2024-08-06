/**
 * @file cxx_remote_tests.cpp
 * @brief Contains tests for OpenSHMEM RMA routines.
 */

#include "cxx_remote_tests.h"

/****************************************************************/
/**
 * @brief Tests the shmem_put() function.
 *
 * This test verifies that the shmem_put() function correctly
 * transfers data from PE 0 to PE 1.
 *
 * @details PE 0 puts data into an array on PE 1.
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_PUT(TYPE, TYPENAME)        \
  ({                                              \
    bool success = true;                          \
    static TYPE src[10], dest[10];                \
    int mype = shmem_my_pe();                     \
    int npes = shmem_n_pes();                     \
                                                  \
    for (int i = 0; i < 10; i++) {                \
      src[i] = i + mype;                          \
    }                                             \
                                                  \
    shmem_barrier_all();                          \
                                                  \
    if (mype == 0) {                              \
      shmem_##TYPENAME##_put(dest, src, 10, 1);   \
    }                                             \
                                                  \
    shmem_barrier_all();                          \
                                                  \
    if (mype == 1) {                              \
      for (int i = 0; i < 10; i++) {              \
        if (dest[i] != i) {                       \
          success = false;                        \
          break;                                  \
        }                                         \
      }                                           \
    }                                             \
                                                  \
    success;                                      \
  })

bool test_cxx_shmem_put(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_PUT(float, float);
  result &= TEST_CXX_SHMEM_PUT(double, double);
  result &= TEST_CXX_SHMEM_PUT(long double, longdouble);
  result &= TEST_CXX_SHMEM_PUT(char, char);
  result &= TEST_CXX_SHMEM_PUT(signed char, schar);
  result &= TEST_CXX_SHMEM_PUT(short, short);
  result &= TEST_CXX_SHMEM_PUT(int, int);
  result &= TEST_CXX_SHMEM_PUT(long, long);
  result &= TEST_CXX_SHMEM_PUT(long long, longlong);
  result &= TEST_CXX_SHMEM_PUT(unsigned char, uchar);
  result &= TEST_CXX_SHMEM_PUT(unsigned short, ushort);
  result &= TEST_CXX_SHMEM_PUT(unsigned int, uint);
  result &= TEST_CXX_SHMEM_PUT(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_PUT(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_PUT(int8_t, int8);
  result &= TEST_CXX_SHMEM_PUT(int16_t, int16);
  result &= TEST_CXX_SHMEM_PUT(int32_t, int32);
  result &= TEST_CXX_SHMEM_PUT(int64_t, int64);
  result &= TEST_CXX_SHMEM_PUT(uint8_t, uint8);
  result &= TEST_CXX_SHMEM_PUT(uint16_t, uint16);
  result &= TEST_CXX_SHMEM_PUT(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_PUT(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_PUT(size_t, size);
  result &= TEST_CXX_SHMEM_PUT(ptrdiff_t, ptrdiff);

  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_p() function.
 *
 * This test verifies that the shmem_p() function correctly
 * transfers a single data element from PE 0 to PE 1.
 *
 * @details PE 0 puts a single data element into PE 1.
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_P(TYPE, TYPENAME)           \
  ({                                               \
    bool success = true;                           \
    static TYPE src, dest;                         \
    int mype = shmem_my_pe();                      \
    int npes = shmem_n_pes();                      \
                                                   \
    src = mype;                                    \
                                                   \
    shmem_barrier_all();                           \
                                                   \
    if (mype == 0) {                               \
      shmem_##TYPENAME##_p(&dest, src, 1);         \
    }                                              \
                                                   \
    shmem_barrier_all();                           \
                                                   \
    if (mype == 1) {                               \
      if (dest != 0) {                             \
        success = false;                           \
      }                                            \
    }                                              \
                                                   \
    success;                                       \
  })

bool test_cxx_shmem_p(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_P(long, long);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_iput() function.
 *
 * This test verifies that the shmem_iput() function correctly
 * transfers data from PE 0 to PE 1 using an indirect stride.
 *
 * @details PE 0 puts data into an array on PE 1 using an indirect stride.
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_IPUT(TYPE, TYPENAME)        \
  ({                                               \
    bool success = true;                           \
    static TYPE src[10], dest[10];                 \
    int mype = shmem_my_pe();                      \
    int npes = shmem_n_pes();                      \
                                                   \
    for (int i = 0; i < 10; i++) {                 \
      src[i] = i + mype;                           \
    }                                              \
                                                   \
    shmem_barrier_all();                           \
                                                   \
    if (mype == 0) {                               \
      shmem_##TYPENAME##_iput(dest, src, 2, 2, 5, 1); \
    }                                              \
                                                   \
    shmem_barrier_all();                           \
                                                   \
    if (mype == 1) {                               \
      for (int i = 0; i < 10; i += 2) {            \
        if (dest[i] != i / 2) {                    \
          success = false;                         \
          break;                                   \
        }                                          \
      }                                            \
    }                                              \
                                                   \
    success;                                       \
  })

bool test_cxx_shmem_iput(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_IPUT(long, long);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_get() function.
 *
 * This test verifies that the shmem_get() function correctly
 * retrieves data from PE 0 to PE 1.
 *
 * @details PE 1 gets data from an array on PE 0.
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_GET(TYPE, TYPENAME)        \
  ({                                              \
    bool success = true;                          \
    static TYPE src[10], dest[10];                \
    int mype = shmem_my_pe();                     \
    int npes = shmem_n_pes();                     \
                                                  \
    if (mype == 0) {                              \
      for (int i = 0; i < 10; i++) {              \
        src[i] = i;                               \
      }                                           \
    }                                             \
                                                  \
    shmem_barrier_all();                          \
                                                  \
    if (mype == 1) {                              \
      shmem_##TYPENAME##_get(dest, src, 10, 0);   \
    }                                             \
                                                  \
    shmem_barrier_all();                          \
                                                  \
    if (mype == 1) {                              \
      for (int i = 0; i < 10; i++) {              \
        if (dest[i] != i) {                       \
          success = false;                        \
          break;                                  \
        }                                         \
      }                                           \
    }                                             \
                                                  \
    success;                                      \
  })

bool test_cxx_shmem_get(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_GET(long, long);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_g() function.
 *
 * This test verifies that the shmem_g() function correctly
 * retrieves a single data element from PE 0 to PE 1.
 *
 * @details PE 1 gets a single data element from PE 0.
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_G(TYPE, TYPENAME)          \
  ({                                              \
    bool success = true;                          \
    static TYPE src, dest;                        \
    int mype = shmem_my_pe();                     \
    int npes = shmem_n_pes();                     \
                                                  \
    if (mype == 0) {                              \
      src = 10;                                   \
    }                                             \
                                                  \
    shmem_barrier_all();                          \
                                                  \
    if (mype == 1) {                              \
      dest = shmem_##TYPENAME##_g(&src, 0);       \
    }                                             \
                                                  \
    shmem_barrier_all();                          \
                                                  \
    if (mype == 1) {                              \
      if (dest != 10) {                           \
        success = false;                          \
      }                                           \
    }                                             \
                                                  \
    success;                                      \
  })

bool test_cxx_shmem_g(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_G(long, long);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_iget() function.
 *
 * This test verifies that the shmem_iget() function correctly
 * retrieves data from PE 0 to PE 1 using an indirect stride.
 *
 * @details PE 1 gets data from an array on PE 0 using an indirect stride.
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_IGET(TYPE, TYPENAME)       \
  ({                                              \
    bool success = true;                          \
    static TYPE src[10], dest[10];                \
    int mype = shmem_my_pe();                     \
    int npes = shmem_n_pes();                     \
                                                  \
    if (mype == 0) {                              \
      for (int i = 0; i < 10; i++) {              \
        src[i] = i;                               \
      }                                           \
    }                                             \
                                                  \
    shmem_barrier_all();                          \
                                                  \
    if (mype == 1) {                              \
      shmem_##TYPENAME##_iget(dest, src, 2, 2, 5, 0); \
    }                                             \
                                                  \
    shmem_barrier_all();                          \
                                                  \
    if (mype == 1) {                              \
      for (int i = 0; i < 10; i += 2) {           \
        if (dest[i] != i / 2) {                   \
          success = false;                        \
          break;                                  \
        }                                         \
      }                                           \
    }                                             \
                                                  \
    success;                                      \
  })

bool test_cxx_shmem_iget(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_IGET(long, long);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_put_nbi() function.
 *
 * This test verifies that the shmem_put_nbi() function correctly
 * transfers data from PE 0 to PE 1 using non-blocking operations.
 *
 * @details PE 0 puts data into an array on PE 1 using non-blocking operations.
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_PUT_NBI(TYPE, TYPENAME)    \
  ({                                              \
    bool success = true;                          \
    static TYPE src[10], dest[10];                \
    int mype = shmem_my_pe();                     \
    int npes = shmem_n_pes();                     \
                                                  \
    for (int i = 0; i < 10; i++) {                \
      src[i] = i + mype;                          \
    }                                             \
                                                  \
    shmem_barrier_all();                          \
                                                  \
    if (mype == 0) {                              \
      shmem_##TYPENAME##_put_nbi(dest, src, 10, 1); \
      shmem_quiet();                              \
    }                                             \
                                                  \
    shmem_barrier_all();                          \
                                                  \
    if (mype == 1) {                              \
      for (int i = 0; i < 10; i++) {              \
        if (dest[i] != i) {                       \
          success = false;                        \
          break;                                  \
        }                                         \
      }                                           \
    }                                             \
                                                  \
    success;                                      \
  })

bool test_cxx_shmem_put_nbi(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_PUT_NBI(long, long);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_get_nbi() function.
 *
 * This test verifies that the shmem_get_nbi() function correctly
 * retrieves data from PE 0 to PE 1 using non-blocking operations.
 *
 * @details PE 1 gets data from an array on PE 0 using non-blocking operations.
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_GET_NBI(TYPE, TYPENAME)    \
  ({                                              \
    bool success = true;                          \
    static TYPE src[10], dest[10];                \
    int mype = shmem_my_pe();                     \
    int npes = shmem_n_pes();                     \
                                                  \
    if (mype == 0) {                              \
      for (int i = 0; i < 10; i++) {              \
        src[i] = i;                               \
      }                                           \
    }                                             \
                                                  \
    shmem_barrier_all();                          \
                                                  \
    if (mype == 1) {                              \
      shmem_##TYPENAME##_get_nbi(dest, src, 10, 0); \
      shmem_quiet();                              \
    }                                             \
                                                  \
    shmem_barrier_all();                          \
                                                  \
    if (mype == 1) {                              \
      for (int i = 0; i < 10; i++) {              \
        if (dest[i] != i) {                       \
          success = false;                        \
          break;                                  \
        }                                         \
      }                                           \
    }                                             \
                                                  \
    success;                                      \
  })

bool test_cxx_shmem_get_nbi(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_GET_NBI(long, long);
  return result;
}

/****************************************************************/
/**
 * @brief Run all C/CXX RMA tests
 */
/****************************************************************/
void run_cxx_remote_tests(int mype, int npes) {
  /* Check to make sure there are at least 2 PEs */
  if ( !(npes > 1) ) {
    if (mype == 0) {
      display_not_enough_pes("REMOTE MEMORY ACCESS"); 
    }
  }
  else {
    /* Run shmem_put() test */
    shmem_barrier_all();
    bool result_shmem_put = test_cxx_shmem_put();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_put()", result_shmem_put, false);
    }

    /* Run shmem_p() test */
    shmem_barrier_all();
    bool result_shmem_p = test_cxx_shmem_p();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_p()", result_shmem_p, false);
    }

    /* Run shmem_iput() test */
    shmem_barrier_all();
    bool result_shmem_iput = test_cxx_shmem_iput();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_iput()", result_shmem_iput, false);
    }

    /* Run shmem_get() test */
    shmem_barrier_all();
    bool result_shmem_get = test_cxx_shmem_get();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_get()", result_shmem_get, false);
    }

    /* Run shmem_g() test */
    shmem_barrier_all();
    bool result_shmem_g = test_cxx_shmem_g();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_g()", result_shmem_g, false);
    }

    /* Run shmem_iget() test */
    shmem_barrier_all();
    bool result_shmem_iget = test_cxx_shmem_iget();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_iget()", result_shmem_iget, false);
    }

    /* Run shmem_put_nbi() test */
    shmem_barrier_all();
    bool result_shmem_put_nbi = test_cxx_shmem_put_nbi();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_put_nbi()", result_shmem_put_nbi, false);
    }

    /* Run shmem_get_nbi() test */
    shmem_barrier_all();
    bool result_shmem_get_nbi = test_cxx_shmem_get_nbi();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_get_nbi()", result_shmem_get_nbi, false);
    }
  }
}
