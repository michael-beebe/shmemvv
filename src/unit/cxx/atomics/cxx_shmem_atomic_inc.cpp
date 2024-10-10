/**
 * @file cxx_shmem_atomic_inc.cpp
 * @brief Unit test for shmem_atomic_inc
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"

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

int main(int argc, char *argv[]) {
  shmem_init();

  bool result = true;
  int rc = EXIT_SUCCESS;

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

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_atomic_inc()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return rc;
}

