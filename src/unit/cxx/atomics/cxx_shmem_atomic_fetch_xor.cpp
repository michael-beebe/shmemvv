/**
 * @file cxx_shmem_atomic_fetch_xor.cpp
 * @brief Test for shmem_atomic_fetch_xor()
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shmemvv.h"

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

int main(void) {
  shmem_init();

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_XOR(unsigned int, uint);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_XOR(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_XOR(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_XOR(int32_t, int32);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_XOR(int64_t, int64);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_XOR(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_XOR(uint64_t, uint64);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_atomic_fetch_xor()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return rc;
}
