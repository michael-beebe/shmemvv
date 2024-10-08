/**
 * @file c_shmem_atomic_set.c
 * @brief Unit test for shmem_atomic_set
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"

#define TEST_C_SHMEM_ATOMIC_SET(TYPE, TYPENAME)                                \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE set;                                                          \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    TYPE value = 42;                                                           \
    *dest = value;                                                             \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    shmem_atomic_set(dest, value, mype);                                        \
    shmem_barrier_all();                                                       \
    success = (*dest == value);                                                  \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C_SHMEM_ATOMIC_SET(int, int);
  result &= TEST_C_SHMEM_ATOMIC_SET(long, long);
  result &= TEST_C_SHMEM_ATOMIC_SET(long long, longlong);
  result &= TEST_C_SHMEM_ATOMIC_SET(unsigned int, uint);
  result &= TEST_C_SHMEM_ATOMIC_SET(unsigned long, ulong);
  result &= TEST_C_SHMEM_ATOMIC_SET(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_ATOMIC_SET(int32_t, int32);
  result &= TEST_C_SHMEM_ATOMIC_SET(int64_t, int64);
  result &= TEST_C_SHMEM_ATOMIC_SET(uint32_t, uint32);
  result &= TEST_C_SHMEM_ATOMIC_SET(uint64_t, uint64);
  result &= TEST_C_SHMEM_ATOMIC_SET(size_t, size);
  result &= TEST_C_SHMEM_ATOMIC_SET(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (result) {
    if (shmem_my_pe() == 0) {
      display_test_result("C shmem_atomic_set()", result, false);
    }
  } else {
    if (shmem_my_pe() == 0) {
      display_test_result("C shmem_atomic_set()", result, false);
      rc = EXIT_FAILURE;
    }
  }

  shmem_finalize();
  return rc;
}
