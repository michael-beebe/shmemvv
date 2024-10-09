/**
 * @file cxx_shmem_atomic_xor.cpp
 * @brief Unit test for shmem_atomic_xor
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"

#define TEST_CXX_SHMEM_ATOMIC_XOR(TYPE, TYPENAME)                               \
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

int main(int argc, char *argv[]) {
  shmem_init();

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_CXX_SHMEM_ATOMIC_XOR(unsigned int, uint);
  result &= TEST_CXX_SHMEM_ATOMIC_XOR(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_ATOMIC_XOR(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_ATOMIC_XOR(int32_t, int32);
  result &= TEST_CXX_SHMEM_ATOMIC_XOR(int64_t, int64);
  result &= TEST_CXX_SHMEM_ATOMIC_XOR(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_ATOMIC_XOR(uint64_t, uint64);

  shmem_barrier_all();

  if (result) {
    if (shmem_my_pe() == 0) {
      display_test_result("CXX shmem_atomic_xor()", result, false);
    }
  } else {
    if (shmem_my_pe() == 0) {
      display_test_result("CXX shmem_atomic_xor()", result, false);
      rc = EXIT_FAILURE;
    }
  }

  shmem_finalize();
  return rc;
}
