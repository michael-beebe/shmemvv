/**
 * @file cxx_atomics_tests.cpp
 * @brief Contains tests for OpenSHMEM atomic routines.
 */

#include "cxx_atomics_tests.h"

/****************************************************************/
/**
 * @brief Tests the shmem_atomic_fetch() routine.
 *
 * This test verifies that the shmem_atomic_fetch() routine
 * correctly retrieves the value from a remote memory location.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_ATOMIC_FETCH(TYPE, TYPENAME)                            \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    TYPE value = 42;                                                           \
    *dest = value;                                                             \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    fetch = shmem_##TYPENAME##_atomic_fetch(dest, mype);                       \
    shmem_barrier_all();                                                       \
    success = (fetch == value);                                                \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

bool test_cxx_shmem_atomic_fetch(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH(int, int);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH(long, long);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH(long long, longlong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH(unsigned int, uint);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH(int32_t, int32);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH(int64_t, int64);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH(size_t, size);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_atomic_set() routine.
 *
 * This test verifies that the shmem_atomic_set() routine
 * correctly sets the value at a remote memory location.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_ATOMIC_SET(TYPE, TYPENAME)                              \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    TYPE value = 42;                                                           \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    shmem_##TYPENAME##_atomic_set(dest, value, mype);                          \
    shmem_barrier_all();                                                       \
    success = (*dest == value);                                                \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

bool test_cxx_shmem_atomic_set(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_ATOMIC_SET(int, int);
  result &= TEST_CXX_SHMEM_ATOMIC_SET(long, long);
  result &= TEST_CXX_SHMEM_ATOMIC_SET(long long, longlong);
  result &= TEST_CXX_SHMEM_ATOMIC_SET(unsigned int, uint);
  result &= TEST_CXX_SHMEM_ATOMIC_SET(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_ATOMIC_SET(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_ATOMIC_SET(int32_t, int32);
  result &= TEST_CXX_SHMEM_ATOMIC_SET(int64_t, int64);
  result &= TEST_CXX_SHMEM_ATOMIC_SET(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_ATOMIC_SET(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_ATOMIC_SET(size_t, size);
  result &= TEST_CXX_SHMEM_ATOMIC_SET(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_atomic_compare_swap() routine.
 *
 * This test verifies that the shmem_atomic_compare_swap()
 * routine correctly swaps the value at a remote memory location
 * if it matches the expected value.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_ATOMIC_COMPARE_SWAP(TYPE, TYPENAME)                     \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    TYPE old = 42, new_val = 43;                                               \
    *dest = old;                                                               \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    shmem_barrier_all();                                                       \
    TYPE swapped = shmem_##TYPENAME##_atomic_compare_swap(dest, old, new_val,  \
                                                          (mype + 1) % npes);  \
    shmem_barrier_all();                                                       \
    success = (swapped == old && *dest == new_val);                            \
    shmem_barrier_all();                                                       \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

bool test_cxx_shmem_atomic_compare_swap(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_ATOMIC_COMPARE_SWAP(int, int);
  result &= TEST_CXX_SHMEM_ATOMIC_COMPARE_SWAP(long, long);
  result &= TEST_CXX_SHMEM_ATOMIC_COMPARE_SWAP(long long, longlong);
  result &= TEST_CXX_SHMEM_ATOMIC_COMPARE_SWAP(unsigned int, uint);
  result &= TEST_CXX_SHMEM_ATOMIC_COMPARE_SWAP(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_ATOMIC_COMPARE_SWAP(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_ATOMIC_COMPARE_SWAP(int32_t, int32);
  result &= TEST_CXX_SHMEM_ATOMIC_COMPARE_SWAP(int64_t, int64);
  result &= TEST_CXX_SHMEM_ATOMIC_COMPARE_SWAP(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_ATOMIC_COMPARE_SWAP(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_ATOMIC_COMPARE_SWAP(size_t, size);
  result &= TEST_CXX_SHMEM_ATOMIC_COMPARE_SWAP(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_atomic_swap() routine.
 *
 * This test verifies that the shmem_atomic_swap() routine
 * correctly swaps the value at a remote memory location and
 * returns the old value.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_ATOMIC_SWAP(TYPE, TYPENAME)                             \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    TYPE value = 42, new_val = 43;                                             \
    *dest = value;                                                             \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    TYPE swapped = shmem_##TYPENAME##_atomic_swap(dest, new_val, mype);        \
    shmem_barrier_all();                                                       \
    success = (swapped == value && *dest == new_val);                          \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

bool test_cxx_shmem_atomic_swap(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_ATOMIC_SWAP(int, int);
  result &= TEST_CXX_SHMEM_ATOMIC_SWAP(long, long);
  result &= TEST_CXX_SHMEM_ATOMIC_SWAP(long long, longlong);
  result &= TEST_CXX_SHMEM_ATOMIC_SWAP(unsigned int, uint);
  result &= TEST_CXX_SHMEM_ATOMIC_SWAP(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_ATOMIC_SWAP(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_ATOMIC_SWAP(int32_t, int32);
  result &= TEST_CXX_SHMEM_ATOMIC_SWAP(int64_t, int64);
  result &= TEST_CXX_SHMEM_ATOMIC_SWAP(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_ATOMIC_SWAP(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_ATOMIC_SWAP(size_t, size);
  result &= TEST_CXX_SHMEM_ATOMIC_SWAP(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_atomic_fetch_inc() routine.
 *
 * This test verifies that the shmem_atomic_fetch_inc() routine
 * correctly increments the value at a remote memory location
 * and returns the old value.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_ATOMIC_FETCH_INC(TYPE, TYPENAME)                        \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    TYPE value = 42;                                                           \
    *dest = value;                                                             \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    fetch = shmem_##TYPENAME##_atomic_fetch_inc(dest, mype);                   \
    shmem_barrier_all();                                                       \
    success = (fetch == value && *dest == value + 1);                          \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

bool test_cxx_shmem_atomic_fetch_inc(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_INC(int, int);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_INC(long, long);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_INC(long long, longlong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_INC(unsigned int, uint);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_INC(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_INC(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_INC(int32_t, int32);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_INC(int64_t, int64);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_INC(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_INC(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_INC(size_t, size);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_INC(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_atomic_inc() routine.
 *
 * This test verifies that the shmem_atomic_inc() routine
 * correctly increments the value at a remote memory location.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_ATOMIC_INC(TYPE, TYPENAME)                              \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    TYPE value = 42;                                                           \
    *dest = value;                                                             \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    shmem_##TYPENAME##_atomic_inc(dest, mype);                                 \
    shmem_barrier_all();                                                       \
    success = (*dest == value + 1);                                            \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

bool test_cxx_shmem_atomic_inc(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_ATOMIC_INC(int, int);
  result &= TEST_CXX_SHMEM_ATOMIC_INC(long, long);
  result &= TEST_CXX_SHMEM_ATOMIC_INC(long long, longlong);
  result &= TEST_CXX_SHMEM_ATOMIC_INC(unsigned int, uint);
  result &= TEST_CXX_SHMEM_ATOMIC_INC(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_ATOMIC_INC(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_ATOMIC_INC(int32_t, int32);
  result &= TEST_CXX_SHMEM_ATOMIC_INC(int64_t, int64);
  result &= TEST_CXX_SHMEM_ATOMIC_INC(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_ATOMIC_INC(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_ATOMIC_INC(size_t, size);
  result &= TEST_CXX_SHMEM_ATOMIC_INC(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_atomic_fetch_add() routine.
 *
 * This test verifies that the shmem_atomic_fetch_add() routine
 * correctly adds a value to the remote memory location and
 * returns the old value.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_ATOMIC_FETCH_ADD(TYPE, TYPENAME)                        \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    TYPE value = 42, add_val = 10;                                             \
    *dest = value;                                                             \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    fetch = shmem_##TYPENAME##_atomic_fetch_add(dest, add_val, mype);          \
    shmem_barrier_all();                                                       \
    success = (fetch == value && *dest == value + add_val);                    \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

bool test_cxx_shmem_atomic_fetch_add(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_ADD(int, int);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_ADD(long, long);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_ADD(long long, longlong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_ADD(unsigned int, uint);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_ADD(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_ADD(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_ADD(int32_t, int32);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_ADD(int64_t, int64);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_ADD(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_ADD(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_ADD(size_t, size);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_ADD(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_atomic_add() routine.
 *
 * This test verifies that the shmem_atomic_add() routine
 * correctly adds a value to the remote memory location.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_ATOMIC_ADD(TYPE, TYPENAME)                              \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    TYPE value = 42, add_val = 10;                                             \
    *dest = value;                                                             \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    shmem_##TYPENAME##_atomic_add(dest, add_val, mype);                        \
    shmem_barrier_all();                                                       \
    success = (*dest == value + add_val);                                      \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

bool test_cxx_shmem_atomic_add(void) {
  bool result = true;
  result &= TEST_CXX_SHMEM_ATOMIC_ADD(int, int);
  result &= TEST_CXX_SHMEM_ATOMIC_ADD(long, long);
  result &= TEST_CXX_SHMEM_ATOMIC_ADD(long long, longlong);
  result &= TEST_CXX_SHMEM_ATOMIC_ADD(unsigned int, uint);
  result &= TEST_CXX_SHMEM_ATOMIC_ADD(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_ATOMIC_ADD(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_ATOMIC_ADD(int32_t, int32);
  result &= TEST_CXX_SHMEM_ATOMIC_ADD(int64_t, int64);
  result &= TEST_CXX_SHMEM_ATOMIC_ADD(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_ATOMIC_ADD(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_ATOMIC_ADD(size_t, size);
  result &= TEST_CXX_SHMEM_ATOMIC_ADD(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_atomic_fetch_and() routine.
 *
 * This test verifies that the shmem_atomic_fetch_and() routine
 * correctly performs a bitwise AND operation with the remote
 * memory location and returns the old value.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_ATOMIC_FETCH_AND(TYPE, TYPENAME)                        \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    TYPE value = 42, and_val = 15;                                             \
    *dest = value;                                                             \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    fetch = shmem_##TYPENAME##_atomic_fetch_and(dest, and_val, mype);          \
    shmem_barrier_all();                                                       \
    success = (fetch == value && *dest == (value & and_val));                  \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

bool test_cxx_shmem_atomic_fetch_and(void) {
  bool result = true;
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_AND(int, int);
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_AND(long, long);
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_AND(long long, longlong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_AND(unsigned int, uint);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_AND(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_AND(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_AND(int32_t, int32);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_AND(int64_t, int64);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_AND(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_AND(uint64_t, uint64);
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_AND(size_t, size);
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_AND(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_atomic_and() routine.
 *
 * This test verifies that the shmem_atomic_and() routine
 * correctly performs a bitwise AND operation with the remote
 * memory location.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_ATOMIC_AND(TYPE, TYPENAME)                              \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    TYPE value = 42, and_val = 15;                                             \
    *dest = value;                                                             \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    shmem_##TYPENAME##_atomic_and(dest, and_val, mype);                        \
    shmem_barrier_all();                                                       \
    success = (*dest == (value & and_val));                                    \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

bool test_cxx_shmem_atomic_and(void) {
  bool result = true;
  // result &= TEST_CXX_SHMEM_ATOMIC_AND(int, int);
  // result &= TEST_CXX_SHMEM_ATOMIC_AND(long, long);
  // result &= TEST_CXX_SHMEM_ATOMIC_AND(long long, longlong);
  result &= TEST_CXX_SHMEM_ATOMIC_AND(unsigned int, uint);
  result &= TEST_CXX_SHMEM_ATOMIC_AND(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_ATOMIC_AND(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_ATOMIC_AND(int32_t, int32);
  result &= TEST_CXX_SHMEM_ATOMIC_AND(int64_t, int64);
  result &= TEST_CXX_SHMEM_ATOMIC_AND(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_ATOMIC_AND(uint64_t, uint64);
  // result &= TEST_CXX_SHMEM_ATOMIC_AND(size_t, size);
  // result &= TEST_CXX_SHMEM_ATOMIC_AND(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_atomic_fetch_or() routine.
 *
 * This test verifies that the shmem_atomic_fetch_or() routine
 * correctly performs a bitwise OR operation with the remote
 * memory location and returns the old value.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_ATOMIC_FETCH_OR(TYPE, TYPENAME)                         \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    TYPE value = 42, or_val = 15;                                              \
    *dest = value;                                                             \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    fetch = shmem_##TYPENAME##_atomic_fetch_or(dest, or_val, mype);            \
    shmem_barrier_all();                                                       \
    success = (fetch == value && *dest == (value | or_val));                   \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

bool test_cxx_shmem_atomic_fetch_or(void) {
  bool result = true;
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_OR(int, int);
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_OR(long, long);
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_OR(long long, longlong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_OR(unsigned int, uint);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_OR(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_OR(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_OR(int32_t, int32);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_OR(int64_t, int64);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_OR(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_OR(uint64_t, uint64);
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_OR(size_t, size);
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_OR(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_atomic_or() routine.
 *
 * This test verifies that the shmem_atomic_or() routine
 * correctly performs a bitwise OR operation with the remote
 * memory location.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_ATOMIC_OR(TYPE, TYPENAME)                               \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    TYPE value = 42, or_val = 15;                                              \
    *dest = value;                                                             \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    shmem_##TYPENAME##_atomic_or(dest, or_val, mype);                          \
    shmem_barrier_all();                                                       \
    success = (*dest == (value | or_val));                                     \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

bool test_cxx_shmem_atomic_or(void) {
  bool result = true;
  // result &= TEST_CXX_SHMEM_ATOMIC_OR(int, int);
  // result &= TEST_CXX_SHMEM_ATOMIC_OR(long, long);
  // result &= TEST_CXX_SHMEM_ATOMIC_OR(long long, longlong);
  result &= TEST_CXX_SHMEM_ATOMIC_OR(unsigned int, uint);
  result &= TEST_CXX_SHMEM_ATOMIC_OR(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_ATOMIC_OR(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_ATOMIC_OR(int32_t, int32);
  result &= TEST_CXX_SHMEM_ATOMIC_OR(int64_t, int64);
  result &= TEST_CXX_SHMEM_ATOMIC_OR(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_ATOMIC_OR(uint64_t, uint64);
  // result &= TEST_CXX_SHMEM_ATOMIC_OR(size_t, size);
  // result &= TEST_CXX_SHMEM_ATOMIC_OR(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_atomic_fetch_xor() routine.
 *
 * This test verifies that the shmem_atomic_fetch_xor() routine
 * correctly performs a bitwise XOR operation with the remote
 * memory location and returns the old value.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_ATOMIC_FETCH_XOR(TYPE, TYPENAME)                        \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    TYPE value = 42, xor_val = 15;                                             \
    *dest = value;                                                             \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    fetch = shmem_##TYPENAME##_atomic_fetch_xor(dest, xor_val, mype);          \
    shmem_barrier_all();                                                       \
    success = (fetch == value && *dest == (value ^ xor_val));                  \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

bool test_cxx_shmem_atomic_fetch_xor(void) {
  bool result = true;
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_XOR(int, int);
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_XOR(long, long);
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_XOR(long long, longlong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_XOR(unsigned int, uint);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_XOR(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_XOR(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_XOR(int32_t, int32);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_XOR(int64_t, int64);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_XOR(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_XOR(uint64_t, uint64);
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_XOR(size_t, size);
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_XOR(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_atomic_xor() routine.
 *
 * This test verifies that the shmem_atomic_xor() routine
 * correctly performs a bitwise XOR operation with the remote
 * memory location.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_ATOMIC_XOR(TYPE, TYPENAME)                              \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    TYPE value = 42, xor_val = 15;                                             \
    *dest = value;                                                             \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    shmem_##TYPENAME##_atomic_xor(dest, xor_val, mype);                        \
    shmem_barrier_all();                                                       \
    success = (*dest == (value ^ xor_val));                                    \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

bool test_cxx_shmem_atomic_xor(void) {
  bool result = true;
  // result &= TEST_CXX_SHMEM_ATOMIC_XOR(int, int);
  // result &= TEST_CXX_SHMEM_ATOMIC_XOR(long, long);
  // result &= TEST_CXX_SHMEM_ATOMIC_XOR(long long, longlong);
  result &= TEST_CXX_SHMEM_ATOMIC_XOR(unsigned int, uint);
  result &= TEST_CXX_SHMEM_ATOMIC_XOR(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_ATOMIC_XOR(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_ATOMIC_XOR(int32_t, int32);
  result &= TEST_CXX_SHMEM_ATOMIC_XOR(int64_t, int64);
  result &= TEST_CXX_SHMEM_ATOMIC_XOR(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_ATOMIC_XOR(uint64_t, uint64);
  // result &= TEST_CXX_SHMEM_ATOMIC_XOR(size_t, size);
  // result &= TEST_CXX_SHMEM_ATOMIC_XOR(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_atomic_fetch_nbi() routine.
 *
 * This test verifies that the shmem_atomic_fetch_nbi()
 * routine correctly retrieves the value from a remote memory
 * location in a non-blocking manner.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_ATOMIC_FETCH_NBI(TYPE, TYPENAME)                        \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    TYPE value = 42;                                                           \
    *dest = value;                                                             \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    shmem_##TYPENAME##_atomic_fetch_nbi(&fetch, dest, mype);                   \
    shmem_quiet();                                                             \
    shmem_barrier_all();                                                       \
    success = (fetch == value);                                                \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

bool test_cxx_shmem_atomic_fetch_nbi(void) {
  bool result = true;
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_NBI(int, int);
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_NBI(long, long);
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_NBI(long long, longlong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_NBI(unsigned int, uint);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_NBI(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_NBI(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_NBI(int32_t, int32);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_NBI(int64_t, int64);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_NBI(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_NBI(uint64_t, uint64);
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_NBI(size_t, size);
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_NBI(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_atomic_compare_swap_nbi() routine.
 *
 * This test verifies that the shmem_atomic_compare_swap_nbi()
 * routine correctly swaps the value at a remote memory location
 * in a non-blocking manner if it matches the expected value.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_ATOMIC_COMPARE_SWAP_NBI(TYPE, TYPENAME)                 \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    fetch = 0;                                                                 \
    TYPE old = 42, new_val = 43;                                               \
    *dest = old;                                                               \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    shmem_##TYPENAME##_atomic_compare_swap_nbi(&fetch, dest, old, new_val,     \
                                               mype);                          \
    shmem_quiet();                                                             \
    shmem_barrier_all();                                                       \
    success = (fetch == old && *dest == new_val);                              \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

bool test_cxx_shmem_atomic_compare_swap_nbi(void) {
  bool result = true;
  // result &= TEST_CXX_SHMEM_ATOMIC_COMPARE_SWAP_NBI(int, int);
  // result &= TEST_CXX_SHMEM_ATOMIC_COMPARE_SWAP_NBI(long, long);
  // result &= TEST_CXX_SHMEM_ATOMIC_COMPARE_SWAP_NBI(long long, longlong);
  result &= TEST_CXX_SHMEM_ATOMIC_COMPARE_SWAP_NBI(unsigned int, uint);
  result &= TEST_CXX_SHMEM_ATOMIC_COMPARE_SWAP_NBI(unsigned long, ulong);
  result &=
      TEST_CXX_SHMEM_ATOMIC_COMPARE_SWAP_NBI(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_ATOMIC_COMPARE_SWAP_NBI(int32_t, int32);
  result &= TEST_CXX_SHMEM_ATOMIC_COMPARE_SWAP_NBI(int64_t, int64);
  result &= TEST_CXX_SHMEM_ATOMIC_COMPARE_SWAP_NBI(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_ATOMIC_COMPARE_SWAP_NBI(uint64_t, uint64);
  // result &= TEST_CXX_SHMEM_ATOMIC_COMPARE_SWAP_NBI(size_t, size);
  // result &= TEST_CXX_SHMEM_ATOMIC_COMPARE_SWAP_NBI(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_atomic_swap_nbi() routine.
 *
 * This test verifies that the shmem_atomic_swap_nbi()
 * routine correctly swaps the value at a remote memory location
 * in a non-blocking manner and returns the old value.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_ATOMIC_SWAP_NBI(TYPE, TYPENAME)                         \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    fetch = 0;                                                                 \
    TYPE value = 42, new_val = 43;                                             \
    *dest = value;                                                             \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    shmem_##TYPENAME##_atomic_swap_nbi(&fetch, dest, new_val, mype);           \
    shmem_quiet();                                                             \
    shmem_barrier_all();                                                       \
    success = (fetch == value && *dest == new_val);                            \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

bool test_cxx_shmem_atomic_swap_nbi(void) {
  bool result = true;
  // result &= TEST_CXX_SHMEM_ATOMIC_SWAP_NBI(int, int);
  // result &= TEST_CXX_SHMEM_ATOMIC_SWAP_NBI(long, long);
  // result &= TEST_CXX_SHMEM_ATOMIC_SWAP_NBI(long long, longlong);
  result &= TEST_CXX_SHMEM_ATOMIC_SWAP_NBI(unsigned int, uint);
  result &= TEST_CXX_SHMEM_ATOMIC_SWAP_NBI(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_ATOMIC_SWAP_NBI(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_ATOMIC_SWAP_NBI(int32_t, int32);
  result &= TEST_CXX_SHMEM_ATOMIC_SWAP_NBI(int64_t, int64);
  result &= TEST_CXX_SHMEM_ATOMIC_SWAP_NBI(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_ATOMIC_SWAP_NBI(uint64_t, uint64);
  // result &= TEST_CXX_SHMEM_ATOMIC_SWAP_NBI(size_t, size);
  // result &= TEST_CXX_SHMEM_ATOMIC_SWAP_NBI(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_atomic_fetch_inc_nbi() routine.
 *
 * This test verifies that the shmem_atomic_fetch_inc_nbi()
 * routine correctly increments the value at a remote memory
 * location in a non-blocking manner and returns the old value.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_ATOMIC_FETCH_INC_NBI(TYPE, TYPENAME)                    \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    fetch = 0;                                                                 \
    TYPE value = 42;                                                           \
    *dest = value;                                                             \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    shmem_##TYPENAME##_atomic_fetch_inc_nbi(&fetch, dest, mype);               \
    shmem_quiet();                                                             \
    shmem_barrier_all();                                                       \
    success = (fetch == value && *dest == value + 1);                          \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

bool test_cxx_shmem_atomic_fetch_inc_nbi(void) {
  bool result = true;
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_INC_NBI(int, int);
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_INC_NBI(long, long);
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_INC_NBI(long long, longlong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_INC_NBI(unsigned int, uint);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_INC_NBI(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_INC_NBI(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_INC_NBI(int32_t, int32);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_INC_NBI(int64_t, int64);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_INC_NBI(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_INC_NBI(uint64_t, uint64);
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_INC_NBI(size_t, size);
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_INC_NBI(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_atomic_fetch_add_nbi() routine.
 *
 * This test verifies that the shmem_atomic_fetch_add_nbi()
 * routine correctly adds a value to the remote memory location
 * in a non-blocking manner and returns the old value.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_ATOMIC_FETCH_ADD_NBI(TYPE, TYPENAME)                    \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    fetch = 0;                                                                 \
    TYPE value = 42, add_val = 10;                                             \
    *dest = value;                                                             \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    shmem_##TYPENAME##_atomic_fetch_add_nbi(&fetch, dest, add_val, mype);      \
    shmem_quiet();                                                             \
    shmem_barrier_all();                                                       \
    success = (fetch == value && *dest == value + add_val);                    \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

bool test_cxx_shmem_atomic_fetch_add_nbi(void) {
  bool result = true;
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_ADD_NBI(int, int);
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_ADD_NBI(long, long);
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_ADD_NBI(long long, longlong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_ADD_NBI(unsigned int, uint);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_ADD_NBI(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_ADD_NBI(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_ADD_NBI(int32_t, int32);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_ADD_NBI(int64_t, int64);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_ADD_NBI(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_ADD_NBI(uint64_t, uint64);
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_ADD_NBI(size_t, size);
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_ADD_NBI(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_atomic_fetch_and_nbi() routine.
 *
 * This test verifies that the shmem_atomic_fetch_and_nbi()
 * routine correctly performs a bitwise AND operation with the
 * remote memory location in a non-blocking manner and returns
 * the old value.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_ATOMIC_FETCH_AND_NBI(TYPE, TYPENAME)                    \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    fetch = 0;                                                                 \
    TYPE value = 42, and_val = 15;                                             \
    *dest = value;                                                             \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    shmem_##TYPENAME##_atomic_fetch_and_nbi(&fetch, dest, and_val, mype);      \
    shmem_quiet();                                                             \
    shmem_barrier_all();                                                       \
    success = (fetch == value && *dest == (value & and_val));                  \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

bool test_cxx_shmem_atomic_fetch_and_nbi(void) {
  bool result = true;
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_AND_NBI(int, int);
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_AND_NBI(long, long);
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_AND_NBI(long long, longlong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_AND_NBI(unsigned int, uint);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_AND_NBI(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_AND_NBI(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_AND_NBI(int32_t, int32);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_AND_NBI(int64_t, int64);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_AND_NBI(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_AND_NBI(uint64_t, uint64);
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_AND_NBI(size_t, size);
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_AND_NBI(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_atomic_fetch_or_nbi() routine.
 *
 * This test verifies that the shmem_atomic_fetch_or_nbi()
 * routine correctly performs a bitwise OR operation with the
 * remote memory location in a non-blocking manner and returns
 * the old value.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_ATOMIC_FETCH_OR_NBI(TYPE, TYPENAME)                     \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    fetch = 0;                                                                 \
    TYPE value = 42, or_val = 15;                                              \
    *dest = value;                                                             \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    shmem_##TYPENAME##_atomic_fetch_or_nbi(&fetch, dest, or_val, mype);        \
    shmem_quiet();                                                             \
    shmem_barrier_all();                                                       \
    success = (fetch == value && *dest == (value | or_val));                   \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

bool test_cxx_shmem_atomic_fetch_or_nbi(void) {
  bool result = true;
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_OR_NBI(int, int);
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_OR_NBI(long, long);
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_OR_NBI(long long, longlong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_OR_NBI(unsigned int, uint);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_OR_NBI(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_OR_NBI(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_OR_NBI(int32_t, int32);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_OR_NBI(int64_t, int64);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_OR_NBI(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_OR_NBI(uint64_t, uint64);
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_OR_NBI(size_t, size);
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_OR_NBI(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Tests the shmem_atomic_fetch_xor_nbi() routine.
 *
 * This test verifies that the shmem_atomic_fetch_xor_nbi()
 * routine correctly performs a bitwise XOR operation with the
 * remote memory location in a non-blocking manner and returns
 * the old value.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
#define TEST_CXX_SHMEM_ATOMIC_FETCH_XOR_NBI(TYPE, TYPENAME)                    \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    fetch = 0;                                                                 \
    TYPE value = 42, xor_val = 15;                                             \
    *dest = value;                                                             \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    shmem_##TYPENAME##_atomic_fetch_xor_nbi(&fetch, dest, xor_val, mype);      \
    shmem_quiet();                                                             \
    shmem_barrier_all();                                                       \
    success = (fetch == value && *dest == (value ^ xor_val));                  \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

bool test_cxx_shmem_atomic_fetch_xor_nbi(void) {
  bool result = true;
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_XOR_NBI(int, int);
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_XOR_NBI(long, long);
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_XOR_NBI(long long, longlong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_XOR_NBI(unsigned int, uint);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_XOR_NBI(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_XOR_NBI(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_XOR_NBI(int32_t, int32);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_XOR_NBI(int64_t, int64);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_XOR_NBI(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_XOR_NBI(uint64_t, uint64);
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_XOR_NBI(size_t, size);
  // result &= TEST_CXX_SHMEM_ATOMIC_FETCH_XOR_NBI(ptrdiff_t, ptrdiff);
  return result;
}

/****************************************************************/
/**
 * @brief Run all C/CXX atomic tests
 */
/****************************************************************/
void run_cxx_atomics_tests(int mype, int npes) {
  /* Make sure there are at least 2 PEs */
  if (!(npes > 1)) {
    if (mype == 0) {
      display_not_enough_pes("ATOMIC MEMORY OPS");
    }
  } else {
    /* Run shmem_atomic_fetch() test */
    shmem_barrier_all();
    bool result_shmem_atomic_fetch = test_cxx_shmem_atomic_fetch();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_fetch()",
                          result_shmem_atomic_fetch, false);
    }

    /* Run shmem_atomic_set() test */
    shmem_barrier_all();
    bool result_shmem_atomic_set = test_cxx_shmem_atomic_set();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_set()", result_shmem_atomic_set,
                          false);
    }

    /* Run shmem_atomic_compare_swap() test */
    shmem_barrier_all();
    bool result_shmem_atomic_compare_swap =
        test_cxx_shmem_atomic_compare_swap();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_compare_swap()",
                          result_shmem_atomic_compare_swap, false);
    }

    /* Run shmem_atomic_swap() test */
    shmem_barrier_all();
    bool result_shmem_atomic_swap = test_cxx_shmem_atomic_swap();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_swap()", result_shmem_atomic_swap,
                          false);
    }

    /* Run shmem_atomic_fetch_inc() test */
    shmem_barrier_all();
    bool result_shmem_atomic_fetch_inc = test_cxx_shmem_atomic_fetch_inc();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_fetch_inc()",
                          result_shmem_atomic_fetch_inc, false);
    }

    /* Run shmem_atomic_inc() test */
    shmem_barrier_all();
    bool result_shmem_atomic_inc = test_cxx_shmem_atomic_inc();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_inc()", result_shmem_atomic_inc,
                          false);
    }

    /* Run shmem_atomic_fetch_add() test */
    shmem_barrier_all();
    bool result_shmem_atomic_fetch_add = test_cxx_shmem_atomic_fetch_add();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_fetch_add()",
                          result_shmem_atomic_fetch_add, false);
    }

    /* Run shmem_atomic_add() test */
    shmem_barrier_all();
    bool result_shmem_atomic_add = test_cxx_shmem_atomic_add();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_add()", result_shmem_atomic_add,
                          false);
    }

    /* Run shmem_atomic_fetch_and() test */
    shmem_barrier_all();
    bool result_shmem_atomic_fetch_and = test_cxx_shmem_atomic_fetch_and();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_fetch_and()",
                          result_shmem_atomic_fetch_and, false);
    }

    /* Run shmem_atomic_and() test */
    shmem_barrier_all();
    bool result_shmem_atomic_and = test_cxx_shmem_atomic_and();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_and()", result_shmem_atomic_and,
                          false);
    }

    /* Run shmem_atomic_fetch_or() test */
    shmem_barrier_all();
    bool result_shmem_atomic_fetch_or = test_cxx_shmem_atomic_fetch_or();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_fetch_or()",
                          result_shmem_atomic_fetch_or, false);
    }

    /* Run shmem_atomic_or() test */
    shmem_barrier_all();
    bool result_shmem_atomic_or = test_cxx_shmem_atomic_or();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_or()", result_shmem_atomic_or,
                          false);
    }

    /* Run shmem_atomic_fetch_xor() test */
    shmem_barrier_all();
    bool result_shmem_atomic_fetch_xor = test_cxx_shmem_atomic_fetch_xor();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_fetch_xor()",
                          result_shmem_atomic_fetch_xor, false);
    }

    /* Run shmem_atomic_xor() test */
    shmem_barrier_all();
    bool result_shmem_atomic_xor = test_cxx_shmem_atomic_xor();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_xor()", result_shmem_atomic_xor,
                          false);
    }

    /* Run shmem_atomic_fetch_nbi() test */
    shmem_barrier_all();
    bool result_shmem_atomic_fetch_nbi = test_cxx_shmem_atomic_fetch_nbi();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_fetch_nbi()",
                          result_shmem_atomic_fetch_nbi, false);
    }

    /* Run shmem_atomic_compare_swap_nbi() test */
    shmem_barrier_all();
    bool result_shmem_atomic_compare_swap_nbi =
        test_cxx_shmem_atomic_compare_swap_nbi();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_compare_swap_nbi()",
                          result_shmem_atomic_compare_swap_nbi, false);
    }

    /* Run shmem_atomic_swap_nbi() test */
    shmem_barrier_all();
    bool result_shmem_atomic_swap_nbi = test_cxx_shmem_atomic_swap_nbi();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_swap_nbi()",
                          result_shmem_atomic_swap_nbi, false);
    }

    /* Run shmem_atomic_fetch_inc_nbi() test */
    shmem_barrier_all();
    bool result_shmem_atomic_fetch_inc_nbi =
        test_cxx_shmem_atomic_fetch_inc_nbi();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_fetch_inc_nbi()",
                          result_shmem_atomic_fetch_inc_nbi, false);
    }

    /* Run shmem_atomic_fetch_add_nbi() test */
    shmem_barrier_all();
    bool result_shmem_atomic_fetch_add_nbi =
        test_cxx_shmem_atomic_fetch_add_nbi();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_fetch_add_nbi()",
                          result_shmem_atomic_fetch_add_nbi, false);
    }

    /* Run shmem_atomic_fetch_and_nbi() test */
    shmem_barrier_all();
    bool result_shmem_atomic_fetch_and_nbi =
        test_cxx_shmem_atomic_fetch_and_nbi();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_fetch_and_nbi()",
                          result_shmem_atomic_fetch_and_nbi, false);
    }

    /* Run shmem_atomic_fetch_or_nbi() test */
    shmem_barrier_all();
    bool result_shmem_atomic_fetch_or_nbi =
        test_cxx_shmem_atomic_fetch_or_nbi();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_fetch_or_nbi()",
                          result_shmem_atomic_fetch_or_nbi, false);
    }

    /* Run shmem_atomic_fetch_xor_nbi() test */
    shmem_barrier_all();
    bool result_shmem_atomic_fetch_xor_nbi =
        test_cxx_shmem_atomic_fetch_xor_nbi();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C/CXX shmem_atomic_fetch_xor_nbi()",
                          result_shmem_atomic_fetch_xor_nbi, false);
    }
  }
}
