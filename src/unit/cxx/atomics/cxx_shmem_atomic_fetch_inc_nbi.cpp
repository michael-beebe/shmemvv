/**
 * @file cxx_shmem_atomic_fetch_inc_nbi.cpp
 * @brief Unit test for shmem_atomic_fetch_inc_nbi
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"

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

int main(int argc, char *argv[]) {
  shmem_init();

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_INC_NBI(int, int);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_INC_NBI(long, long);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_INC_NBI(long long, longlong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_INC_NBI(unsigned int, uint);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_INC_NBI(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_INC_NBI(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_INC_NBI(int32_t, int32);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_INC_NBI(int64_t, int64);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_INC_NBI(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_INC_NBI(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_INC_NBI(size_t, size);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_INC_NBI(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_atomic_fetch_inc_nbi()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return rc;
}
