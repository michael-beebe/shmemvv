/**
 * @file cxx_pt2pt_tests.cpp
 * @brief Contains functions definitions with test functions for the point-to-point synchronization routines.
 */

#include "cxx_pt2pt_tests.h"

#define TIMEOUT 1

/****************************************************************/
/**
 * @brief Tests the shmem_wait_until() routine.
 *
 * This test verifies that the shmem_wait_until() function correctly waits until a condition
 * on a memory location is met.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_WAIT_UNTIL(TYPE, TYPENAME)                          \
  ({                                                                       \
    bool success = true;                                                   \
    TYPE *flag = (TYPE *)shmem_malloc(sizeof(TYPE));                       \
    if (flag == NULL) {                                                    \
      success = false;                                                     \
    } else {                                                               \
      *flag = 0;                                                           \
      int mype = shmem_my_pe();                                            \
      int npes = shmem_n_pes();                                            \
                                                                           \
      shmem_barrier_all();                                                 \
                                                                           \
      if (mype == 0) {                                                     \
        for (int pe = 1; pe < npes; ++pe) {                                \
          shmem_##TYPENAME##_p(flag, 1, pe);                               \
        }                                                                  \
        shmem_quiet();                                                     \
      }                                                                    \
                                                                           \
      shmem_barrier_all();                                                 \
                                                                           \
      if (mype != 0) {                                                     \
        shmem_##TYPENAME##_wait_until(flag, SHMEM_CMP_EQ, 1);              \
        if (*flag != 1) {                                                  \
          success = false;                                                 \
        }                                                                  \
      }                                                                    \
      shmem_free(flag);                                                    \
    }                                                                      \
    success;                                                               \
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
 * @brief Tests the shmem_wait_until_all() routine.
 *
 * This test verifies that the shmem_wait_until_all() function correctly waits until all specified
 * conditions on an array of memory locations are met.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_WAIT_UNTIL_ALL(TYPE, TYPENAME)                     \
  ({                                                                      \
    bool success = true;                                                  \
    TYPE *flags = (TYPE *)shmem_malloc(2 * sizeof(TYPE));                 \
    if (flags == NULL) {                                                  \
      success = false;                                                    \
    } else {                                                              \
      flags[0] = 0;                                                       \
      flags[1] = 0;                                                       \
      int mype = shmem_my_pe();                                           \
      int npes = shmem_n_pes();                                           \
                                                                          \
      shmem_barrier_all();                                                \
                                                                          \
      if (mype == 0) {                                                    \
        for (int pe = 1; pe < npes; ++pe) {                               \
          shmem_##TYPENAME##_p(&flags[0], 1, pe);                         \
          shmem_##TYPENAME##_p(&flags[1], 1, pe);                         \
        }                                                                 \
        shmem_quiet();                                                    \
      }                                                                   \
                                                                          \
      shmem_barrier_all();                                                \
                                                                          \
      if (mype != 0) {                                                    \
        shmem_##TYPENAME##_wait_until_all(flags, 2, NULL, SHMEM_CMP_EQ, 1); \
        if (flags[0] != 1 || flags[1] != 1) {                             \
          success = false;                                                \
        }                                                                 \
      }                                                                   \
      shmem_free(flags);                                                  \
    }                                                                     \
    success;                                                              \
  })

bool test_cxx_shmem_wait_until_all(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL(short, short);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL(int, int);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL(long, long);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL(long long, longlong);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL(unsigned short, ushort);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL(unsigned int, uint);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL(int32_t, int32);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL(int64_t, int64);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL(size_t, size);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_wait_until_any() routine.
 *
 * This test verifies that the shmem_wait_until_any() function correctly waits until any one
 * of the specified conditions on an array of memory locations is met.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_WAIT_UNTIL_ANY(TYPE, TYPENAME)                    \
  ({                                                                     \
    bool success = true;                                                 \
    TYPE *flags = (TYPE *)shmem_malloc(3 * sizeof(TYPE));                \
    if (flags == NULL) {                                                 \
      success = false;                                                   \
    } else {                                                             \
      for (int i = 0; i < 3; i++) {                                      \
        flags[i] = 0;                                                    \
      }                                                                  \
      int mype = shmem_my_pe();                                          \
                                                                         \
      shmem_barrier_all();                                               \
                                                                         \
      if (mype == 0) {                                                   \
        shmem_##TYPENAME##_p(&flags[2], 1, 1);                           \
        shmem_quiet();                                                   \
      }                                                                  \
                                                                         \
      shmem_barrier_all();                                               \
                                                                         \
      if (mype != 0) {                                                   \
        int status[3] = {SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ};      \
        size_t index = shmem_##TYPENAME##_wait_until_any(flags, 3, status, SHMEM_CMP_EQ, 1); \
        if (index == SIZE_MAX) {                                         \
          success = false;                                               \
        } else if (flags[index] != 1) {                                  \
          success = false;                                               \
        }                                                                \
      }                                                                  \
      shmem_free(flags);                                                 \
    }                                                                    \
    success;                                                             \
  })

bool test_cxx_shmem_wait_until_any(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY(short, short);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY(int, int);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY(long, long);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY(long long, longlong);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY(unsigned short, ushort);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY(unsigned int, uint);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY(int32_t, int32);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY(int64_t, int64);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY(size_t, size);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_wait_until_some() routine.
 *
 * This test verifies that the shmem_wait_until_some() function correctly waits until some
 * of the specified conditions on an array of memory locations are met.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_WAIT_UNTIL_SOME(TYPE, TYPENAME)                   \
  ({                                                                     \
    bool success = true;                                                 \
    TYPE *flags = (TYPE *)shmem_malloc(4 * sizeof(TYPE));                \
    if (flags == NULL) {                                                 \
      success = false;                                                   \
    } else {                                                             \
      for (int i = 0; i < 4; ++i) {                                      \
        flags[i] = 0;                                                    \
      }                                                                  \
      int mype = shmem_my_pe();                                          \
                                                                         \
      shmem_barrier_all();                                               \
                                                                         \
      if (mype == 0) {                                                   \
        shmem_##TYPENAME##_p(&flags[1], 1, 1);                           \
        shmem_##TYPENAME##_p(&flags[3], 1, 1);                           \
        shmem_quiet();                                                   \
      }                                                                  \
                                                                         \
      shmem_barrier_all();                                               \
                                                                         \
      if (mype != 0) {                                                   \
        size_t indices[4];                                               \
        int status[4] = {SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ}; \
        size_t count = shmem_##TYPENAME##_wait_until_some(flags, 4, indices, status, SHMEM_CMP_EQ, 1); \
        if (count < 2) {                                                 \
          success = false;                                               \
        } else {                                                         \
          for (size_t i = 0; i < count; ++i) {                           \
            if (flags[indices[i]] != 1) {                                \
              success = false;                                           \
              break;                                                     \
            }                                                            \
          }                                                              \
        }                                                                \
      }                                                                  \
      shmem_free(flags);                                                 \
    }                                                                    \
    success;                                                             \
  })

bool test_cxx_shmem_wait_until_some(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME(short, short);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME(int, int);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME(long, long);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME(long long, longlong);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME(unsigned short, ushort);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME(unsigned int, uint);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME(int32_t, int32);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME(int64_t, int64);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME(size_t, size);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_wait_until_all_vector() routine.
 *
 * This test verifies that the shmem_wait_until_all_vector() function correctly waits until all specified
 * conditions on a vector of memory locations are met.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_WAIT_UNTIL_ALL_VECTOR(TYPE, TYPENAME)                      \
  ({                                                                             \
    bool success = true;                                                         \
    TYPE *flags = (TYPE *)shmem_malloc(4 * sizeof(TYPE));                        \
    if (flags == NULL) {                                                         \
      success = false;                                                           \
    } else {                                                                     \
      for (int i = 0; i < 4; ++i) {                                              \
        flags[i] = 0;                                                            \
      }                                                                          \
      int mype = shmem_my_pe();                                                  \
                                                                                 \
      shmem_barrier_all();                                                       \
                                                                                 \
      if (mype == 0) {                                                           \
        for (int i = 0; i < 4; ++i) {                                            \
          shmem_##TYPENAME##_p(&flags[i], 1, 1);                                 \
        }                                                                        \
        shmem_quiet();                                                           \
      }                                                                          \
                                                                                 \
      shmem_barrier_all();                                                       \
                                                                                 \
      if (mype != 0) {                                                           \
        int status[4] = {SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ};\
        TYPE cmp_values[4] = {1, 1, 1, 1};                                       \
        shmem_##TYPENAME##_wait_until_all_vector(flags, 4, status, SHMEM_CMP_EQ, cmp_values); \
        for (int i = 0; i < 4; ++i) {                                            \
          if (flags[i] != 1) {                                                   \
            success = false;                                                     \
            break;                                                               \
          }                                                                      \
        }                                                                        \
      }                                                                          \
      shmem_free(flags);                                                         \
    }                                                                            \
    success;                                                                     \
  })

bool test_cxx_shmem_wait_until_all_vector(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL_VECTOR(short, short);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL_VECTOR(int, int);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL_VECTOR(long, long);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL_VECTOR(long long, longlong);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL_VECTOR(unsigned short, ushort);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL_VECTOR(unsigned int, uint);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL_VECTOR(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL_VECTOR(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL_VECTOR(int32_t, int32);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL_VECTOR(int64_t, int64);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL_VECTOR(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL_VECTOR(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL_VECTOR(size_t, size);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL_VECTOR(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_wait_until_any_vector() routine.
 *
 * This test verifies that the shmem_wait_until_any_vector() function correctly waits until any one
 * of the specified conditions on a vector of memory locations is met.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_WAIT_UNTIL_ANY_VECTOR(TYPE, TYPENAME)                       \
  ({                                                                               \
    bool success = true;                                                           \
    TYPE *flags = (TYPE *)shmem_malloc(4 * sizeof(TYPE));                          \
    if (flags == NULL) {                                                           \
      success = false;                                                             \
    } else {                                                                       \
      for (int i = 0; i < 4; ++i) {                                                \
        flags[i] = 0;                                                              \
      }                                                                            \
      int mype = shmem_my_pe();                                                    \
                                                                                   \
      shmem_barrier_all();                                                         \
                                                                                   \
      if (mype == 0) {                                                             \
        shmem_##TYPENAME##_p(&flags[2], 1, 1);                                     \
        shmem_quiet();                                                             \
      }                                                                            \
                                                                                   \
      shmem_barrier_all();                                                         \
                                                                                   \
      if (mype != 0) {                                                             \
        int status[4] = {SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ};  \
        TYPE cmp_values[4] = {1, 1, 1, 1};                                         \
        size_t index = shmem_##TYPENAME##_wait_until_any_vector(flags, 4, status, SHMEM_CMP_EQ, cmp_values); \
        if (index == SIZE_MAX) {                                                   \
          success = false;                                                         \
        } else if (flags[index] != 1) {                                            \
          success = false;                                                         \
        }                                                                          \
      }                                                                            \
      shmem_free(flags);                                                           \
    }                                                                              \
    success;                                                                       \
  })

bool test_cxx_shmem_wait_until_any_vector(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY_VECTOR(short, short);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY_VECTOR(int, int);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY_VECTOR(long, long);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY_VECTOR(long long, longlong);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY_VECTOR(unsigned short, ushort);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY_VECTOR(unsigned int, uint);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY_VECTOR(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY_VECTOR(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY_VECTOR(int32_t, int32);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY_VECTOR(int64_t, int64);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY_VECTOR(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY_VECTOR(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY_VECTOR(size_t, size);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY_VECTOR(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_wait_until_some_vector() routine.
 *
 * This test verifies that the shmem_wait_until_some_vector() function correctly waits until some
 * of the specified conditions on a vector of memory locations are met.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_WAIT_UNTIL_SOME_VECTOR(TYPE, TYPENAME)                     \
  ({                                                                              \
    bool success = true;                                                          \
    TYPE *flags = (TYPE *)shmem_malloc(4 * sizeof(TYPE));                         \
    if (flags == NULL) {                                                          \
      success = false;                                                            \
    } else {                                                                      \
      for (int i = 0; i < 4; ++i) {                                               \
        flags[i] = 0;                                                             \
      }                                                                           \
      int mype = shmem_my_pe();                                                   \
                                                                                  \
      shmem_barrier_all();                                                        \
                                                                                  \
      if (mype == 0) {                                                            \
        shmem_##TYPENAME##_p(&flags[1], 1, 1);                                    \
        shmem_##TYPENAME##_p(&flags[3], 1, 1);                                    \
        shmem_quiet();                                                            \
      }                                                                           \
                                                                                  \
      shmem_barrier_all();                                                        \
                                                                                  \
      if (mype != 0) {                                                            \
        int status[4] = {SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ}; \
        TYPE cmp_values[4] = {1, 1, 1, 1};                                        \
        size_t indices[4];                                                        \
        size_t count = shmem_##TYPENAME##_wait_until_some_vector(flags, 4, indices, status, SHMEM_CMP_EQ, cmp_values); \
        if (count < 2) {                                                          \
          success = false;                                                        \
        } else {                                                                  \
          for (size_t i = 0; i < count; ++i) {                                    \
            if (flags[indices[i]] != 1) {                                         \
              success = false;                                                    \
              break;                                                              \
            }                                                                     \
          }                                                                       \
        }                                                                         \
      }                                                                           \
      shmem_free(flags);                                                          \
    }                                                                             \
    success;                                                                      \
  })

bool test_cxx_shmem_wait_until_some_vector(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME_VECTOR(short, short);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME_VECTOR(int, int);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME_VECTOR(long, long);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME_VECTOR(long long, longlong);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME_VECTOR(unsigned short, ushort);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME_VECTOR(unsigned int, uint);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME_VECTOR(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME_VECTOR(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME_VECTOR(int32_t, int32);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME_VECTOR(int64_t, int64);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME_VECTOR(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME_VECTOR(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME_VECTOR(size_t, size);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME_VECTOR(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_test() routine.
 *
 * This test verifies that the shmem_test() function correctly tests whether a condition
 * on a memory location is met without blocking.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_TEST(TYPE, TYPENAME)                          \
  ({                                                                \
    bool success = true;                                            \
    TYPE *flag = (TYPE *)shmem_malloc(sizeof(TYPE));                \
    if (flag == NULL) {                                             \
      success = false;                                              \
    } else {                                                        \
      *flag = 0;                                                    \
      int mype = shmem_my_pe();                                     \
                                                                    \
      shmem_barrier_all();                                          \
                                                                    \
      if (mype == 0) {                                              \
        *flag = 1;                                                  \
        shmem_quiet();                                              \
      }                                                             \
                                                                    \
      shmem_barrier_all();                                          \
                                                                    \
      if (mype != 0) {                                              \
        time_t start_time = time(NULL);                             \
        while (!shmem_##TYPENAME##_test(flag, SHMEM_CMP_EQ, 1)) {   \
          if (time(NULL) - start_time > TIMEOUT) {                  \
            break;                                                  \
          }                                                         \
          usleep(1000);                                             \
        }                                                           \
        if (*flag != 1) {                                           \
          success = false;                                          \
        }                                                           \
      }                                                             \
      shmem_free(flag);                                             \
    }                                                               \
    success;                                                        \
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
 * @brief Tests the shmem_test_all() routine.
 *
 * This test verifies that the shmem_test_all() function correctly tests whether
 * all specified conditions on a vector of memory locations are met without blocking.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_TEST_ALL(TYPE, TYPENAME)                         \
  ({                                                                    \
    bool success = true;                                                \
    TYPE *flags = (TYPE *)shmem_malloc(4 * sizeof(TYPE));               \
    if (flags == NULL) {                                                \
      success = false;                                                  \
    } else {                                                            \
      for (int i = 0; i < 4; ++i) {                                     \
        flags[i] = 0;                                                   \
      }                                                                 \
      int mype = shmem_my_pe();                                         \
      int npes = shmem_n_pes();                                         \
                                                                        \
      shmem_barrier_all();                                              \
                                                                        \
      if (mype == 0) {                                                  \
        for (int pe = 1; pe < npes; ++pe) {                             \
          for (int i = 0; i < 4; ++i) {                                 \
            shmem_##TYPENAME##_p(&flags[i], 1, pe);                     \
          }                                                             \
        }                                                               \
        shmem_quiet();                                                  \
      }                                                                 \
                                                                        \
      shmem_barrier_all();                                              \
                                                                        \
      if (mype != 0) {                                                  \
        TYPE cmp_value = 1;                                             \
        time_t start_time = time(NULL);                                 \
        while (!shmem_##TYPENAME##_test_all(flags, 4, NULL, SHMEM_CMP_EQ, cmp_value)) { \
          if (time(NULL) - start_time > TIMEOUT) {                      \
            break;                                                      \
          }                                                             \
          usleep(1000);                                                 \
        }                                                               \
        for (int i = 0; i < 4; ++i) {                                   \
          if (flags[i] != 1) {                                          \
            success = false;                                            \
            break;                                                      \
          }                                                             \
        }                                                               \
      }                                                                 \
      shmem_free(flags);                                                \
    }                                                                   \
    success;                                                            \
  })

bool test_cxx_shmem_test_all(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_TEST_ALL(short, short);
  result &= TEST_CXX_SHMEM_TEST_ALL(int, int);
  result &= TEST_CXX_SHMEM_TEST_ALL(long, long);
  result &= TEST_CXX_SHMEM_TEST_ALL(long long, longlong);
  result &= TEST_CXX_SHMEM_TEST_ALL(unsigned short, ushort);
  result &= TEST_CXX_SHMEM_TEST_ALL(unsigned int, uint);
  result &= TEST_CXX_SHMEM_TEST_ALL(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_TEST_ALL(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_TEST_ALL(int32_t, int32);
  result &= TEST_CXX_SHMEM_TEST_ALL(int64_t, int64);
  result &= TEST_CXX_SHMEM_TEST_ALL(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_TEST_ALL(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_TEST_ALL(size_t, size);
  result &= TEST_CXX_SHMEM_TEST_ALL(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_test_any() routine.
 *
 * This test verifies that the shmem_test_any() function correctly tests whether any one
 * of the specified conditions on a vector of memory locations is met without blocking.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_TEST_ANY(TYPE, TYPENAME)                         \
  ({                                                                    \
    bool success = true;                                                \
    TYPE *flags = (TYPE *)shmem_malloc(4 * sizeof(TYPE));               \
    if (flags == NULL) {                                                \
      success = false;                                                  \
    } else {                                                            \
      for (int i = 0; i < 4; ++i) {                                     \
        flags[i] = 0;                                                   \
      }                                                                 \
      int mype = shmem_my_pe();                                         \
      int npes = shmem_n_pes();                                         \
                                                                        \
      shmem_barrier_all();                                              \
                                                                        \
      if (mype == 0) {                                                  \
        for (int pe = 1; pe < npes; ++pe) {                             \
          shmem_##TYPENAME##_p(&flags[2], 1, pe);                       \
        }                                                               \
        shmem_quiet();                                                  \
      }                                                                 \
                                                                        \
      shmem_barrier_all();                                              \
                                                                        \
      if (mype != 0) {                                                  \
        TYPE cmp_value = 1;                                             \
        time_t start_time = time(NULL);                                 \
        while (!shmem_##TYPENAME##_test_any(flags, 4, NULL, SHMEM_CMP_EQ, cmp_value)) { \
          if (time(NULL) - start_time > TIMEOUT) {                      \
            break;                                                      \
          }                                                             \
          usleep(1000);                                                 \
        }                                                               \
        if (flags[2] != 1) {                                            \
          success = false;                                              \
        }                                                               \
      }                                                                 \
      shmem_free(flags);                                                \
    }                                                                   \
    success;                                                            \
  })

bool test_cxx_shmem_test_any(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_TEST_ANY(short, short);
  result &= TEST_CXX_SHMEM_TEST_ANY(int, int);
  result &= TEST_CXX_SHMEM_TEST_ANY(long, long);
  result &= TEST_CXX_SHMEM_TEST_ANY(long long, longlong);
  result &= TEST_CXX_SHMEM_TEST_ANY(unsigned short, ushort);
  result &= TEST_CXX_SHMEM_TEST_ANY(unsigned int, uint);
  result &= TEST_CXX_SHMEM_TEST_ANY(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_TEST_ANY(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_TEST_ANY(int32_t, int32);
  result &= TEST_CXX_SHMEM_TEST_ANY(int64_t, int64);
  result &= TEST_CXX_SHMEM_TEST_ANY(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_TEST_ANY(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_TEST_ANY(size_t, size);
  result &= TEST_CXX_SHMEM_TEST_ANY(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_test_some() routine.
 *
 * This test verifies that the shmem_test_some() function correctly tests whether some
 * of the specified conditions on a vector of memory locations are met without blocking.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_TEST_SOME(TYPE, TYPENAME)                        \
  ({                                                                    \
    bool success = true;                                                \
    TYPE *flags = (TYPE *)shmem_malloc(4 * sizeof(TYPE));               \
    if (flags == NULL) {                                                \
      success = false;                                                  \
    } else {                                                            \
      for (int i = 0; i < 4; ++i) {                                     \
        flags[i] = 0;                                                   \
      }                                                                 \
      int mype = shmem_my_pe();                                         \
      int npes = shmem_n_pes();                                         \
                                                                        \
      shmem_barrier_all();                                              \
                                                                        \
      if (mype == 0) {                                                  \
        for (int pe = 1; pe < npes; ++pe) {                             \
          shmem_##TYPENAME##_p(&flags[1], 1, pe);                       \
          shmem_##TYPENAME##_p(&flags[3], 1, pe);                       \
        }                                                               \
        shmem_quiet();                                                  \
      }                                                                 \
                                                                        \
      shmem_barrier_all();                                              \
                                                                        \
      if (mype != 0) {                                                  \
        TYPE cmp_value = 1;                                             \
        size_t indices[4];                                              \
        size_t num_indices;                                             \
        time_t start_time = time(NULL);                                 \
        while ((num_indices = shmem_##TYPENAME##_test_some(flags, 4, indices, NULL, SHMEM_CMP_EQ, cmp_value)) == 0) { \
          if (time(NULL) - start_time > TIMEOUT) {                      \
            break;                                                      \
          }                                                             \
          usleep(1000);                                                 \
        }                                                               \
        if (flags[1] != 1 || flags[3] != 1) {                           \
          success = false;                                              \
        }                                                               \
      }                                                                 \
      shmem_free(flags);                                                \
    }                                                                   \
    success;                                                            \
  })

bool test_cxx_shmem_test_some(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_TEST_SOME(short, short);
  result &= TEST_CXX_SHMEM_TEST_SOME(int, int);
  result &= TEST_CXX_SHMEM_TEST_SOME(long, long);
  result &= TEST_CXX_SHMEM_TEST_SOME(long long, longlong);
  result &= TEST_CXX_SHMEM_TEST_SOME(unsigned short, ushort);
  result &= TEST_CXX_SHMEM_TEST_SOME(unsigned int, uint);
  result &= TEST_CXX_SHMEM_TEST_SOME(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_TEST_SOME(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_TEST_SOME(int32_t, int32);
  result &= TEST_CXX_SHMEM_TEST_SOME(int64_t, int64);
  result &= TEST_CXX_SHMEM_TEST_SOME(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_TEST_SOME(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_TEST_SOME(size_t, size);
  result &= TEST_CXX_SHMEM_TEST_SOME(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_test_all_vector() routine.
 *
 * This test verifies that the shmem_test_all_vector() function correctly tests whether all specified
 * conditions on a vector of memory locations are met without blocking.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_TEST_ALL_VECTOR(TYPE, TYPENAME)                  \
  ({                                                                    \
    bool success = true;                                                \
    TYPE *flags = (TYPE *)shmem_malloc(4 * sizeof(TYPE));               \
    if (flags == NULL) {                                                \
      success = false;                                                  \
    } else {                                                            \
      for (int i = 0; i < 4; ++i) {                                     \
        flags[i] = 0;                                                   \
      }                                                                 \
      int mype = shmem_my_pe();                                         \
                                                                        \
      shmem_barrier_all();                                              \
                                                                        \
      if (mype == 0) {                                                  \
        for (int i = 0; i < 4; ++i) {                                   \
          flags[i] = 1;                                                 \
        }                                                               \
        shmem_quiet();                                                  \
      }                                                                 \
                                                                        \
      shmem_barrier_all();                                              \
                                                                        \
      if (mype != 0) {                                                  \
        TYPE cmp_values[4] = {1, 1, 1, 1};                              \
        time_t start_time = time(NULL);                                 \
        while (!shmem_##TYPENAME##_test_all_vector(flags, 4, NULL, SHMEM_CMP_EQ, cmp_values)) { \
          if (time(NULL) - start_time > TIMEOUT) {                      \
            break;                                                      \
          }                                                             \
          usleep(1000);                                                 \
        }                                                               \
        for (int i = 0; i < 4; ++i) {                                   \
          if (flags[i] != 1) {                                          \
            success = false;                                            \
            break;                                                      \
          }                                                             \
        }                                                               \
      }                                                                 \
      shmem_free(flags);                                                \
    }                                                                   \
    success;                                                            \
  })

bool test_cxx_shmem_test_all_vector(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_TEST_ALL_VECTOR(short, short);
  result &= TEST_CXX_SHMEM_TEST_ALL_VECTOR(int, int);
  result &= TEST_CXX_SHMEM_TEST_ALL_VECTOR(long, long);
  result &= TEST_CXX_SHMEM_TEST_ALL_VECTOR(long long, longlong);
  result &= TEST_CXX_SHMEM_TEST_ALL_VECTOR(unsigned short, ushort);
  result &= TEST_CXX_SHMEM_TEST_ALL_VECTOR(unsigned int, uint);
  result &= TEST_CXX_SHMEM_TEST_ALL_VECTOR(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_TEST_ALL_VECTOR(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_TEST_ALL_VECTOR(int32_t, int32);
  result &= TEST_CXX_SHMEM_TEST_ALL_VECTOR(int64_t, int64);
  result &= TEST_CXX_SHMEM_TEST_ALL_VECTOR(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_TEST_ALL_VECTOR(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_TEST_ALL_VECTOR(size_t, size);
  result &= TEST_CXX_SHMEM_TEST_ALL_VECTOR(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_test_any_vector() routine.
 *
 * This test verifies that the shmem_test_any_vector() function correctly tests whether any one
 * of the specified conditions on a vector of memory locations is met without blocking.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_TEST_ANY_VECTOR(TYPE, TYPENAME)                  \
  ({                                                                    \
    bool success = true;                                                \
    TYPE *flags = (TYPE *)shmem_malloc(4 * sizeof(TYPE));               \
    if (flags == NULL) {                                                \
      success = false;                                                  \
    } else {                                                            \
      for (int i = 0; i < 4; ++i) {                                     \
        flags[i] = 0;                                                   \
      }                                                                 \
      int mype = shmem_my_pe();                                         \
      int npes = shmem_n_pes();                                         \
                                                                        \
      shmem_barrier_all();                                              \
                                                                        \
      if (mype == 0) {                                                  \
        for (int pe = 1; pe < npes; ++pe) {                             \
          shmem_##TYPENAME##_p(&flags[2], 1, pe);                       \
        }                                                               \
        shmem_quiet();                                                  \
      }                                                                 \
                                                                        \
      shmem_barrier_all();                                              \
                                                                        \
      if (mype != 0) {                                                  \
        TYPE cmp_values[4] = {1, 1, 1, 1};                              \
        time_t start_time = time(NULL);                                 \
        while (!shmem_##TYPENAME##_test_any_vector(flags, 4, NULL, SHMEM_CMP_EQ, cmp_values)) { \
          if (time(NULL) - start_time > TIMEOUT) {                      \
            break;                                                      \
          }                                                             \
          usleep(1000);                                                 \
        }                                                               \
        if (flags[2] != 1) {                                            \
          success = false;                                              \
        }                                                               \
      }                                                                 \
      shmem_free(flags);                                                \
    }                                                                   \
    success;                                                            \
  })

bool test_cxx_shmem_test_any_vector(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_TEST_ANY_VECTOR(short, short);
  result &= TEST_CXX_SHMEM_TEST_ANY_VECTOR(int, int);
  result &= TEST_CXX_SHMEM_TEST_ANY_VECTOR(long, long);
  result &= TEST_CXX_SHMEM_TEST_ANY_VECTOR(long long, longlong);
  result &= TEST_CXX_SHMEM_TEST_ANY_VECTOR(unsigned short, ushort);
  result &= TEST_CXX_SHMEM_TEST_ANY_VECTOR(unsigned int, uint);
  result &= TEST_CXX_SHMEM_TEST_ANY_VECTOR(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_TEST_ANY_VECTOR(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_TEST_ANY_VECTOR(int32_t, int32);
  result &= TEST_CXX_SHMEM_TEST_ANY_VECTOR(int64_t, int64);
  result &= TEST_CXX_SHMEM_TEST_ANY_VECTOR(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_TEST_ANY_VECTOR(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_TEST_ANY_VECTOR(size_t, size);
  result &= TEST_CXX_SHMEM_TEST_ANY_VECTOR(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_test_some_vector() routine.
 *
 * This test verifies that the shmem_test_some_vector() function correctly tests whether some
 * of the specified conditions on a vector of memory locations are met without blocking.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_TEST_SOME_VECTOR(TYPE, TYPENAME)                        \
  ({                                                                           \
    bool success = true;                                                       \
    TYPE *flags = (TYPE *)shmem_malloc(4 * sizeof(TYPE));                      \
    if (flags == NULL) {                                                       \
      success = false;                                                         \
    } else {                                                                   \
      for (int i = 0; i < 4; ++i) {                                            \
        flags[i] = 0;                                                          \
      }                                                                        \
      int mype = shmem_my_pe();                                                \
      int npes = shmem_n_pes();                                                \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 0) {                                                         \
        for (int pe = 1; pe < npes; ++pe) {                                    \
          shmem_##TYPENAME##_p(&flags[1], 1, pe);                              \
          shmem_##TYPENAME##_p(&flags[3], 1, pe);                              \
        }                                                                      \
        shmem_quiet();                                                         \
      }                                                                        \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype != 0) {                                                         \
        TYPE cmp_values[4] = {0, 1, 0, 1};                                     \
        size_t indices[4];                                                     \
        size_t num_indices;                                                    \
        time_t start_time = time(NULL);                                        \
        while ((num_indices = shmem_##TYPENAME##_test_some_vector(flags, 4, indices, NULL, SHMEM_CMP_EQ, cmp_values)) == 0) { \
          if (time(NULL) - start_time > TIMEOUT) {                             \
            break;                                                             \
          }                                                                    \
          usleep(1000);                                                        \
        }                                                                      \
        if (flags[1] != 1 || flags[3] != 1) {                                  \
          success = false;                                                     \
        }                                                                      \
      }                                                                        \
      shmem_free(flags);                                                       \
    }                                                                          \
    success;                                                                   \
  })

bool test_cxx_shmem_test_some_vector(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_TEST_SOME_VECTOR(short, short);
  result &= TEST_CXX_SHMEM_TEST_SOME_VECTOR(int, int);
  result &= TEST_CXX_SHMEM_TEST_SOME_VECTOR(long, long);
  result &= TEST_CXX_SHMEM_TEST_SOME_VECTOR(long long, longlong);
  result &= TEST_CXX_SHMEM_TEST_SOME_VECTOR(unsigned short, ushort);
  result &= TEST_CXX_SHMEM_TEST_SOME_VECTOR(unsigned int, uint);
  result &= TEST_CXX_SHMEM_TEST_SOME_VECTOR(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_TEST_SOME_VECTOR(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_TEST_SOME_VECTOR(int32_t, int32);
  result &= TEST_CXX_SHMEM_TEST_SOME_VECTOR(int64_t, int64);
  result &= TEST_CXX_SHMEM_TEST_SOME_VECTOR(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_TEST_SOME_VECTOR(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_TEST_SOME_VECTOR(size_t, size);
  result &= TEST_CXX_SHMEM_TEST_SOME_VECTOR(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_signal_wait_until() routine.
 *
 * This test verifies that the shmem_signal_wait_until() function correctly waits until a signal
 * on a memory location meets a specified condition.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_SIGNAL_WAIT_UNTIL()                             \
  ({                                                                   \
    bool success = true;                                               \
    uint64_t *flag = (uint64_t *)shmem_malloc(sizeof(uint64_t));       \
    if (flag == NULL) {                                                \
      success = false;                                                 \
    } else {                                                           \
      *flag = 0;                                                       \
      int mype = shmem_my_pe();                                        \
      int npes = shmem_n_pes();                                        \
      uint64_t value = 1;                                              \
                                                                       \
      shmem_barrier_all();                                             \
                                                                       \
      if (mype == 0) {                                                 \
        for (int pe = 1; pe < npes; ++pe) {                            \
          shmem_uint64_p(flag, value, pe);                             \
        }                                                              \
        shmem_quiet();                                                 \
      }                                                                \
                                                                       \
      shmem_barrier_all();                                             \
                                                                       \
      if (mype != 0) {                                                 \
        time_t start_time = time(NULL);                                \
        while (!shmem_test(flag, SHMEM_CMP_EQ, value) && time(NULL) - start_time < TIMEOUT) { \
          shmem_signal_wait_until(flag, SHMEM_CMP_EQ, value);          \
        }                                                              \
        if (*flag != value) {                                          \
          success = false;                                             \
        }                                                              \
      }                                                                \
      shmem_free(flag);                                                \
    }                                                                  \
    success;                                                           \
  })

bool test_cxx_shmem_signal_wait_until(void) {
  return TEST_CXX_SHMEM_SIGNAL_WAIT_UNTIL();
}

/****************************************************************/
/**
 * @brief Run all C/CXX pt2pt synchronization tests
 */
/****************************************************************/
void run_cxx_pt2pt_synch_tests(int mype, int npes) {
  if (!(npes > 1)) {
    display_not_enough_pes("POINT-TO-POINT SYNCH OPS");
  }
  else {
    /* Run shmem_wait_until() test */
    shmem_barrier_all();
    bool result_shmem_wait_until = test_cxx_shmem_wait_until();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_wait_until()", result_shmem_wait_until, false);
    }

    /* Run shmem_wait_until_all() test */
    shmem_barrier_all();
    bool result_shmem_wait_until_all = test_cxx_shmem_wait_until_all();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_wait_until_all()", result_shmem_wait_until_all, false);
    }

    /* Run shmem_wait_until_any() test */
    shmem_barrier_all();
    bool result_shmem_wait_until_any = test_cxx_shmem_wait_until_any();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_wait_until_any()", result_shmem_wait_until_any, false);
    }

    /* Run shmem_wait_until_some() test */
    shmem_barrier_all();
    bool result_shmem_wait_until_some = test_cxx_shmem_wait_until_some();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_wait_until_some()", result_shmem_wait_until_some, false);
    }

    /* Run shmem_wait_until_all_vector() test */
    shmem_barrier_all();
    bool result_shmem_wait_until_all_vector = test_cxx_shmem_wait_until_all_vector();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_wait_until_all_vector()", result_shmem_wait_until_all_vector, false);
    }

    /* Run shmem_wait_until_any_vector() test */
    shmem_barrier_all();
    bool result_shmem_wait_until_any_vector = test_cxx_shmem_wait_until_any_vector();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_wait_until_any_vector()", result_shmem_wait_until_any_vector, false);
    }

    /* Run shmem_wait_until_some_vector() test */
    shmem_barrier_all();
    bool result_shmem_wait_until_some_vector = test_cxx_shmem_wait_until_some_vector();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_wait_until_some_vector()", result_shmem_wait_until_some_vector, false);
    }

    /* Run shmem_test() test */
    shmem_barrier_all();
    bool result_shmem_test = test_cxx_shmem_test();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_test()", result_shmem_test, false);
    }

    /* Run shmem_test_all() test */
    shmem_barrier_all();
    bool result_shmem_test_all = test_cxx_shmem_test_all();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_test_all()", result_shmem_test_all, false);
    }

    /* Run shmem_test_any() test */
    shmem_barrier_all();
    bool result_shmem_test_any = test_cxx_shmem_test_any();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_test_any()", result_shmem_test_any, false);
    }

    /* Run shmem_test_some() test */
    shmem_barrier_all();
    bool result_shmem_test_some = test_cxx_shmem_test_some();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_test_some()", result_shmem_test_some, false);
    }

    /* Run shmem_test_all_vector() test */
    shmem_barrier_all();
    bool result_shmem_test_all_vector = test_cxx_shmem_test_all_vector();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_test_all_vector()", result_shmem_test_all_vector, false);
    }

    /* Run shmem_test_any_vector() test */
    shmem_barrier_all();
    bool result_shmem_test_any_vector = test_cxx_shmem_test_any_vector();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_test_any_vector()", result_shmem_test_any_vector, false);
    }

    /* Run shmem_test_some_vector() test */
    shmem_barrier_all();
    bool result_shmem_test_some_vector = test_cxx_shmem_test_some_vector();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_test_some_vector()", result_shmem_test_some_vector, false);
    }

    /* Run shmem_signal_wait_until() test */
    shmem_barrier_all();
    bool result_shmem_signal_wait_until = test_cxx_shmem_signal_wait_until();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_signal_wait_until()", result_shmem_signal_wait_until, false); 
    }
  }
}
