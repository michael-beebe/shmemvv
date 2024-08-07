/**
 * @file c11_remote_tests.c
 * @brief Contains OpenSHMEM remote memory access tests.
 */

#include "c11_remote_tests.h"

#ifdef __cplusplus
extern "C" {
#endif

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
#define TEST_C11_SHMEM_PUT(TYPE, TYPENAME)                                     \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE src[10], dest[10];                                             \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
                                                                               \
    for (int i = 0; i < 10; i++) {                                             \
      src[i] = i + mype;                                                       \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    if (mype == 0) {                                                           \
      shmem_put(dest, src, 10, 1);                                             \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    if (mype == 1) {                                                           \
      for (int i = 0; i < 10; i++) {                                           \
        if (dest[i] != i) {                                                    \
          success = false;                                                     \
          break;                                                               \
        }                                                                      \
      }                                                                        \
    }                                                                          \
                                                                               \
    success;                                                                   \
  })

bool test_c11_shmem_put(void) {
  bool result = true;
  result &= TEST_C11_SHMEM_PUT(float, float);
  result &= TEST_C11_SHMEM_PUT(double, double);
  result &= TEST_C11_SHMEM_PUT(long double, longdouble);
  result &= TEST_C11_SHMEM_PUT(char, char);
  result &= TEST_C11_SHMEM_PUT(signed char, schar);
  result &= TEST_C11_SHMEM_PUT(short, short);
  result &= TEST_C11_SHMEM_PUT(int, int);
  result &= TEST_C11_SHMEM_PUT(long, long);
  result &= TEST_C11_SHMEM_PUT(long long, longlong);
  result &= TEST_C11_SHMEM_PUT(unsigned char, uchar);
  result &= TEST_C11_SHMEM_PUT(unsigned short, ushort);
  result &= TEST_C11_SHMEM_PUT(unsigned int, uint);
  result &= TEST_C11_SHMEM_PUT(unsigned long, ulong);
  result &= TEST_C11_SHMEM_PUT(unsigned long long, ulonglong);
  result &= TEST_C11_SHMEM_PUT(int8_t, int8);
  result &= TEST_C11_SHMEM_PUT(int16_t, int16);
  result &= TEST_C11_SHMEM_PUT(int32_t, int32);
  result &= TEST_C11_SHMEM_PUT(int64_t, int64);
  result &= TEST_C11_SHMEM_PUT(uint8_t, uint8);
  result &= TEST_C11_SHMEM_PUT(uint16_t, uint16);
  result &= TEST_C11_SHMEM_PUT(uint32_t, uint32);
  result &= TEST_C11_SHMEM_PUT(uint64_t, uint64);
  result &= TEST_C11_SHMEM_PUT(size_t, size);
  result &= TEST_C11_SHMEM_PUT(ptrdiff_t, ptrdiff);

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
#define TEST_C11_SHMEM_P(TYPE, TYPENAME)                                       \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE src, dest;                                                     \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
                                                                               \
    src = mype;                                                                \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    if (mype == 0) {                                                           \
      shmem_p(&dest, src, 1);                                                  \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    if (mype == 1) {                                                           \
      if (dest != 0) {                                                         \
        success = false;                                                       \
      }                                                                        \
    }                                                                          \
                                                                               \
    success;                                                                   \
  })

bool test_c11_shmem_p(void) {
  bool result = true;
  result &= TEST_C11_SHMEM_P(long, long);
  result &= TEST_C11_SHMEM_P(double, double);
  result &= TEST_C11_SHMEM_P(long double, longdouble);
  result &= TEST_C11_SHMEM_P(char, char);
  result &= TEST_C11_SHMEM_P(signed char, schar);
  result &= TEST_C11_SHMEM_P(short, short);
  result &= TEST_C11_SHMEM_P(int, int);
  result &= TEST_C11_SHMEM_P(long, long);
  result &= TEST_C11_SHMEM_P(long long, longlong);
  result &= TEST_C11_SHMEM_P(unsigned char, uchar);
  result &= TEST_C11_SHMEM_P(unsigned short, ushort);
  result &= TEST_C11_SHMEM_P(unsigned int, uint);
  result &= TEST_C11_SHMEM_P(unsigned long, ulong);
  result &= TEST_C11_SHMEM_P(unsigned long long, ulonglong);
  result &= TEST_C11_SHMEM_P(int8_t, int8);
  result &= TEST_C11_SHMEM_P(int16_t, int16);
  result &= TEST_C11_SHMEM_P(int32_t, int32);
  result &= TEST_C11_SHMEM_P(int64_t, int64);
  result &= TEST_C11_SHMEM_P(uint8_t, uint8);
  result &= TEST_C11_SHMEM_P(uint16_t, uint16);
  result &= TEST_C11_SHMEM_P(uint32_t, uint32);
  result &= TEST_C11_SHMEM_P(uint64_t, uint64);
  result &= TEST_C11_SHMEM_P(size_t, size);
  result &= TEST_C11_SHMEM_P(ptrdiff_t, ptrdiff);
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
#define TEST_C11_SHMEM_IPUT(TYPE, TYPENAME)                                    \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE src[10], dest[10];                                             \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
                                                                               \
    for (int i = 0; i < 10; i++) {                                             \
      src[i] = i + mype;                                                       \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    if (mype == 0) {                                                           \
      shmem_iput(dest, src, 2, 2, 5, 1);                                       \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    if (mype == 1) {                                                           \
      for (int i = 0; i < 10; i += 2) {                                        \
        if (dest[i] != i / 2) {                                                \
          success = false;                                                     \
          break;                                                               \
        }                                                                      \
      }                                                                        \
    }                                                                          \
                                                                               \
    success;                                                                   \
  })

bool test_c11_shmem_iput(void) {
  bool result = true;
  result &= TEST_C11_SHMEM_IPUT(long, long);
  result &= TEST_C11_SHMEM_IPUT(double, double);
  result &= TEST_C11_SHMEM_IPUT(long double, longdouble);
  result &= TEST_C11_SHMEM_IPUT(char, char);
  result &= TEST_C11_SHMEM_IPUT(signed char, schar);
  result &= TEST_C11_SHMEM_IPUT(short, short);
  result &= TEST_C11_SHMEM_IPUT(int, int);
  result &= TEST_C11_SHMEM_IPUT(long, long);
  result &= TEST_C11_SHMEM_IPUT(long long, longlong);
  result &= TEST_C11_SHMEM_IPUT(unsigned char, uchar);
  result &= TEST_C11_SHMEM_IPUT(unsigned short, ushort);
  result &= TEST_C11_SHMEM_IPUT(unsigned int, uint);
  result &= TEST_C11_SHMEM_IPUT(unsigned long, ulong);
  result &= TEST_C11_SHMEM_IPUT(unsigned long long, ulonglong);
  result &= TEST_C11_SHMEM_IPUT(int8_t, int8);
  result &= TEST_C11_SHMEM_IPUT(int16_t, int16);
  result &= TEST_C11_SHMEM_IPUT(int32_t, int32);
  result &= TEST_C11_SHMEM_IPUT(int64_t, int64);
  result &= TEST_C11_SHMEM_IPUT(uint8_t, uint8);
  result &= TEST_C11_SHMEM_IPUT(uint16_t, uint16);
  result &= TEST_C11_SHMEM_IPUT(uint32_t, uint32);
  result &= TEST_C11_SHMEM_IPUT(uint64_t, uint64);
  result &= TEST_C11_SHMEM_IPUT(size_t, size);
  result &= TEST_C11_SHMEM_IPUT(ptrdiff_t, ptrdiff);
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
#define TEST_C11_SHMEM_GET(TYPE, TYPENAME)                                     \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE src[10], dest[10];                                             \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
                                                                               \
    if (mype == 0) {                                                           \
      for (int i = 0; i < 10; i++) {                                           \
        src[i] = i;                                                            \
      }                                                                        \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    if (mype == 1) {                                                           \
      shmem_get(dest, src, 10, 0);                                             \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    if (mype == 1) {                                                           \
      for (int i = 0; i < 10; i++) {                                           \
        if (dest[i] != i) {                                                    \
          success = false;                                                     \
          break;                                                               \
        }                                                                      \
      }                                                                        \
    }                                                                          \
                                                                               \
    success;                                                                   \
  })

bool test_c11_shmem_get(void) {
  bool result = true;
  result &= TEST_C11_SHMEM_GET(long, long);
  result &= TEST_C11_SHMEM_GET(double, double);
  result &= TEST_C11_SHMEM_GET(long double, longdouble);
  result &= TEST_C11_SHMEM_GET(char, char);
  result &= TEST_C11_SHMEM_GET(signed char, schar);
  result &= TEST_C11_SHMEM_GET(short, short);
  result &= TEST_C11_SHMEM_GET(int, int);
  result &= TEST_C11_SHMEM_GET(long, long);
  result &= TEST_C11_SHMEM_GET(long long, longlong);
  result &= TEST_C11_SHMEM_GET(unsigned char, uchar);
  result &= TEST_C11_SHMEM_GET(unsigned short, ushort);
  result &= TEST_C11_SHMEM_GET(unsigned int, uint);
  result &= TEST_C11_SHMEM_GET(unsigned long, ulong);
  result &= TEST_C11_SHMEM_GET(unsigned long long, ulonglong);
  result &= TEST_C11_SHMEM_GET(int8_t, int8);
  result &= TEST_C11_SHMEM_GET(int16_t, int16);
  result &= TEST_C11_SHMEM_GET(int32_t, int32);
  result &= TEST_C11_SHMEM_GET(int64_t, int64);
  result &= TEST_C11_SHMEM_GET(uint8_t, uint8);
  result &= TEST_C11_SHMEM_GET(uint16_t, uint16);
  result &= TEST_C11_SHMEM_GET(uint32_t, uint32);
  result &= TEST_C11_SHMEM_GET(uint64_t, uint64);
  result &= TEST_C11_SHMEM_GET(size_t, size);
  result &= TEST_C11_SHMEM_GET(ptrdiff_t, ptrdiff);
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
#define TEST_C11_SHMEM_G(TYPE, TYPENAME)                                       \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE src, dest;                                                     \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
                                                                               \
    if (mype == 0) {                                                           \
      src = 10;                                                                \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    if (mype == 1) {                                                           \
      dest = shmem_g(&src, 0);                                                 \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    if (mype == 1) {                                                           \
      if (dest != 10) {                                                        \
        success = false;                                                       \
      }                                                                        \
    }                                                                          \
                                                                               \
    success;                                                                   \
  })

bool test_c11_shmem_g(void) {
  bool result = true;
  result &= TEST_C11_SHMEM_G(long, long);
  result &= TEST_C11_SHMEM_G(double, double);
  result &= TEST_C11_SHMEM_G(long double, longdouble);
  result &= TEST_C11_SHMEM_G(char, char);
  result &= TEST_C11_SHMEM_G(signed char, schar);
  result &= TEST_C11_SHMEM_G(short, short);
  result &= TEST_C11_SHMEM_G(int, int);
  result &= TEST_C11_SHMEM_G(long, long);
  result &= TEST_C11_SHMEM_G(long long, longlong);
  result &= TEST_C11_SHMEM_G(unsigned char, uchar);
  result &= TEST_C11_SHMEM_G(unsigned short, ushort);
  result &= TEST_C11_SHMEM_G(unsigned int, uint);
  result &= TEST_C11_SHMEM_G(unsigned long, ulong);
  result &= TEST_C11_SHMEM_G(unsigned long long, ulonglong);
  result &= TEST_C11_SHMEM_G(int8_t, int8);
  result &= TEST_C11_SHMEM_G(int16_t, int16);
  result &= TEST_C11_SHMEM_G(int32_t, int32);
  result &= TEST_C11_SHMEM_G(int64_t, int64);
  result &= TEST_C11_SHMEM_G(uint8_t, uint8);
  result &= TEST_C11_SHMEM_G(uint16_t, uint16);
  result &= TEST_C11_SHMEM_G(uint32_t, uint32);
  result &= TEST_C11_SHMEM_G(uint64_t, uint64);
  result &= TEST_C11_SHMEM_G(size_t, size);
  result &= TEST_C11_SHMEM_G(ptrdiff_t, ptrdiff);
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
#define TEST_C11_SHMEM_IGET(TYPE, TYPENAME)                                    \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE src[10], dest[10];                                             \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
                                                                               \
    if (mype == 0) {                                                           \
      for (int i = 0; i < 10; i++) {                                           \
        src[i] = i;                                                            \
      }                                                                        \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    if (mype == 1) {                                                           \
      shmem_iget(dest, src, 2, 2, 5, 0);                                       \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    if (mype == 1) {                                                           \
      for (int i = 0; i < 10; i += 2) {                                        \
        if (dest[i] != i / 2) {                                                \
          success = false;                                                     \
          break;                                                               \
        }                                                                      \
      }                                                                        \
    }                                                                          \
                                                                               \
    success;                                                                   \
  })

bool test_c11_shmem_iget(void) {
  bool result = true;
  result &= TEST_C11_SHMEM_IGET(long, long);
  result &= TEST_C11_SHMEM_IGET(double, double);
  result &= TEST_C11_SHMEM_IGET(long double, longdouble);
  result &= TEST_C11_SHMEM_IGET(char, char);
  result &= TEST_C11_SHMEM_IGET(signed char, schar);
  result &= TEST_C11_SHMEM_IGET(short, short);
  result &= TEST_C11_SHMEM_IGET(int, int);
  result &= TEST_C11_SHMEM_IGET(long, long);
  result &= TEST_C11_SHMEM_IGET(long long, longlong);
  result &= TEST_C11_SHMEM_IGET(unsigned char, uchar);
  result &= TEST_C11_SHMEM_IGET(unsigned short, ushort);
  result &= TEST_C11_SHMEM_IGET(unsigned int, uint);
  result &= TEST_C11_SHMEM_IGET(unsigned long, ulong);
  result &= TEST_C11_SHMEM_IGET(unsigned long long, ulonglong);
  result &= TEST_C11_SHMEM_IGET(int8_t, int8);
  result &= TEST_C11_SHMEM_IGET(int16_t, int16);
  result &= TEST_C11_SHMEM_IGET(int32_t, int32);
  result &= TEST_C11_SHMEM_IGET(int64_t, int64);
  result &= TEST_C11_SHMEM_IGET(uint8_t, uint8);
  result &= TEST_C11_SHMEM_IGET(uint16_t, uint16);
  result &= TEST_C11_SHMEM_IGET(uint32_t, uint32);
  result &= TEST_C11_SHMEM_IGET(uint64_t, uint64);
  result &= TEST_C11_SHMEM_IGET(size_t, size);
  result &= TEST_C11_SHMEM_IGET(ptrdiff_t, ptrdiff);
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
#define TEST_C11_SHMEM_PUT_NBI(TYPE, TYPENAME)                                 \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE src[10], dest[10];                                             \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
                                                                               \
    for (int i = 0; i < 10; i++) {                                             \
      src[i] = i + mype;                                                       \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    if (mype == 0) {                                                           \
      shmem_put_nbi(dest, src, 10, 1);                                         \
      shmem_quiet();                                                           \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    if (mype == 1) {                                                           \
      for (int i = 0; i < 10; i++) {                                           \
        if (dest[i] != i) {                                                    \
          success = false;                                                     \
          break;                                                               \
        }                                                                      \
      }                                                                        \
    }                                                                          \
                                                                               \
    success;                                                                   \
  })

bool test_c11_shmem_put_nbi(void) {
  bool result = true;
  result &= TEST_C11_SHMEM_PUT_NBI(long, long);
  result &= TEST_C11_SHMEM_PUT_NBI(double, double);
  result &= TEST_C11_SHMEM_PUT_NBI(long double, longdouble);
  result &= TEST_C11_SHMEM_PUT_NBI(char, char);
  result &= TEST_C11_SHMEM_PUT_NBI(signed char, schar);
  result &= TEST_C11_SHMEM_PUT_NBI(short, short);
  result &= TEST_C11_SHMEM_PUT_NBI(int, int);
  result &= TEST_C11_SHMEM_PUT_NBI(long, long);
  result &= TEST_C11_SHMEM_PUT_NBI(long long, longlong);
  result &= TEST_C11_SHMEM_PUT_NBI(unsigned char, uchar);
  result &= TEST_C11_SHMEM_PUT_NBI(unsigned short, ushort);
  result &= TEST_C11_SHMEM_PUT_NBI(unsigned int, uint);
  result &= TEST_C11_SHMEM_PUT_NBI(unsigned long, ulong);
  result &= TEST_C11_SHMEM_PUT_NBI(unsigned long long, ulonglong);
  result &= TEST_C11_SHMEM_PUT_NBI(int8_t, int8);
  result &= TEST_C11_SHMEM_PUT_NBI(int16_t, int16);
  result &= TEST_C11_SHMEM_PUT_NBI(int32_t, int32);
  result &= TEST_C11_SHMEM_PUT_NBI(int64_t, int64);
  result &= TEST_C11_SHMEM_PUT_NBI(uint8_t, uint8);
  result &= TEST_C11_SHMEM_PUT_NBI(uint16_t, uint16);
  result &= TEST_C11_SHMEM_PUT_NBI(uint32_t, uint32);
  result &= TEST_C11_SHMEM_PUT_NBI(uint64_t, uint64);
  result &= TEST_C11_SHMEM_PUT_NBI(size_t, size);
  result &= TEST_C11_SHMEM_PUT_NBI(ptrdiff_t, ptrdiff);
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
#define TEST_C11_SHMEM_GET_NBI(TYPE, TYPENAME)                                 \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE src[10], dest[10];                                             \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
                                                                               \
    if (mype == 0) {                                                           \
      for (int i = 0; i < 10; i++) {                                           \
        src[i] = i;                                                            \
      }                                                                        \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    if (mype == 1) {                                                           \
      shmem_get_nbi(dest, src, 10, 0);                                         \
      shmem_quiet();                                                           \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    if (mype == 1) {                                                           \
      for (int i = 0; i < 10; i++) {                                           \
        if (dest[i] != i) {                                                    \
          success = false;                                                     \
          break;                                                               \
        }                                                                      \
      }                                                                        \
    }                                                                          \
                                                                               \
    success;                                                                   \
  })

bool test_c11_shmem_get_nbi(void) {
  bool result = true;
  result &= TEST_C11_SHMEM_GET_NBI(long, long);
  result &= TEST_C11_SHMEM_GET_NBI(double, double);
  result &= TEST_C11_SHMEM_GET_NBI(long double, longdouble);
  result &= TEST_C11_SHMEM_GET_NBI(char, char);
  result &= TEST_C11_SHMEM_GET_NBI(signed char, schar);
  result &= TEST_C11_SHMEM_GET_NBI(short, short);
  result &= TEST_C11_SHMEM_GET_NBI(int, int);
  result &= TEST_C11_SHMEM_GET_NBI(long, long);
  result &= TEST_C11_SHMEM_GET_NBI(long long, longlong);
  result &= TEST_C11_SHMEM_GET_NBI(unsigned char, uchar);
  result &= TEST_C11_SHMEM_GET_NBI(unsigned short, ushort);
  result &= TEST_C11_SHMEM_GET_NBI(unsigned int, uint);
  result &= TEST_C11_SHMEM_GET_NBI(unsigned long, ulong);
  result &= TEST_C11_SHMEM_GET_NBI(unsigned long long, ulonglong);
  result &= TEST_C11_SHMEM_GET_NBI(int8_t, int8);
  result &= TEST_C11_SHMEM_GET_NBI(int16_t, int16);
  result &= TEST_C11_SHMEM_GET_NBI(int32_t, int32);
  result &= TEST_C11_SHMEM_GET_NBI(int64_t, int64);
  result &= TEST_C11_SHMEM_GET_NBI(uint8_t, uint8);
  result &= TEST_C11_SHMEM_GET_NBI(uint16_t, uint16);
  result &= TEST_C11_SHMEM_GET_NBI(uint32_t, uint32);
  result &= TEST_C11_SHMEM_GET_NBI(uint64_t, uint64);
  result &= TEST_C11_SHMEM_GET_NBI(size_t, size);
  result &= TEST_C11_SHMEM_GET_NBI(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Run all C11 RMA tests
 */
/****************************************************************/
void run_c11_remote_tests(int mype, int npes) {
  /* Check to make sure there are at least 2 PEs */
  if (!(npes > 1)) {
    if (mype == 0) {
      display_not_enough_pes("REMOTE MEMORY ACCESS");
    }
  } else {
    /* Run shmem_put() test */
    shmem_barrier_all();
    bool result_shmem_put = test_c11_shmem_put();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_put()", result_shmem_put, false);
    }

    /* Run shmem_p() test */
    shmem_barrier_all();
    bool result_shmem_p = test_c11_shmem_p();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_p()", result_shmem_p, false);
    }

    /* Run shmem_iput() test */
    shmem_barrier_all();
    bool result_shmem_iput = test_c11_shmem_iput();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_iput()", result_shmem_iput, false);
    }

    /* Run shmem_get() test */
    shmem_barrier_all();
    bool result_shmem_get = test_c11_shmem_get();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_get()", result_shmem_get, false);
    }

    /* Run shmem_g() test */
    shmem_barrier_all();
    bool result_shmem_g = test_c11_shmem_g();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_g()", result_shmem_g, false);
    }

    /* Run shmem_iget() test */
    shmem_barrier_all();
    bool result_shmem_iget = test_c11_shmem_iget();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_iget()", result_shmem_iget, false);
    }

    /* Run shmem_put_nbi() test */
    shmem_barrier_all();
    bool result_shmem_put_nbi = test_c11_shmem_put_nbi();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_put_nbi()", result_shmem_put_nbi, false);
    }

    /* Run shmem_get_nbi() test */
    shmem_barrier_all();
    bool result_shmem_get_nbi = test_c11_shmem_get_nbi();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_get_nbi()", result_shmem_get_nbi, false);
    }
  }
}

#ifdef __cplusplus
}
#endif
