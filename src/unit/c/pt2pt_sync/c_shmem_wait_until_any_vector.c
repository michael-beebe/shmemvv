/**
 * @file c_shmem_wait_until_any_vector.c
 * @brief Unit test shmem_wait_until_any_vector() routine.
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "shmemvv.h"

#define TIMEOUT 2

#define TEST_C_SHMEM_WAIT_UNTIL_ANY_VECTOR(TYPE, TYPENAME)                     \
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
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 0) {                                                         \
        shmem_##TYPENAME##_p(&flags[2], 1, 1);                                 \
        shmem_quiet();                                                         \
      }                                                                        \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype != 0) {                                                         \
        int status[4] = {SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ,             \
                         SHMEM_CMP_EQ};                                        \
        TYPE cmp_values[4] = {1, 1, 1, 1};                                     \
        size_t index = shmem_##TYPENAME##_wait_until_any_vector(               \
            flags, 4, status, SHMEM_CMP_EQ, cmp_values);                       \
        if (index == SIZE_MAX) {                                               \
          success = false;                                                     \
        } else if (flags[index] != 1) {                                        \
          success = false;                                                     \
        }                                                                      \
      }                                                                        \
      shmem_free(flags);                                                       \
    }                                                                          \
    success;                                                                   \
  })

int main(int argc, char **argv) {
  shmem_init();

  int result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C_SHMEM_WAIT_UNTIL_ANY_VECTOR(int, int);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ANY_VECTOR(long, long);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ANY_VECTOR(long long, longlong);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ANY_VECTOR(unsigned int, uint);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ANY_VECTOR(unsigned long, ulong);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ANY_VECTOR(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ANY_VECTOR(int32_t, int32);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ANY_VECTOR(int64_t, int64);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ANY_VECTOR(uint32_t, uint32);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ANY_VECTOR(uint64_t, uint64);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ANY_VECTOR(size_t, size);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ANY_VECTOR(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_wait_until_any_vector()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return rc;
}
