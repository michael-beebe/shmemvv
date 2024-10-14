/**
 * @file cxx_shmem_atomic_fetch.cpp
 * @brief Unit test for shmem_atomic_fetch
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"

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

int main(int argc, char *argv[]) {
  shmem_init();

  bool result = true;
  int rc = EXIT_SUCCESS;

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

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_atomic_fetch()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return rc;
}
