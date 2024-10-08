/**
 * @file c11_shmem_atomic_fetch_xor.c
 * @brief Test for shmem_atomic_fetch_xor()
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <shmem.h>

#include "shmemvv.h"

#define TEST_C11_SHMEM_ATOMIC_FETCH_XOR(TYPE)                            \
  ({                                                                             \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    TYPE value = 42, xor_val = 15;                                             \
    *dest = value;                                                             \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    fetch = shmem_atomic_fetch_xor(dest, xor_val, mype);          \
    shmem_barrier_all();                                                       \
    success = (fetch == value && *dest == (value ^ xor_val));                  \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

int main(void) {
  shmem_init();
  
  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C11_SHMEM_ATOMIC_FETCH_XOR(unsigned int);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_XOR(unsigned long);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_XOR(unsigned long long);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_XOR(int32_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_XOR(int64_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_XOR(uint32_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_XOR(uint64_t);
  
  shmem_barrier_all();

  if (result) {
    if (shmem_my_pe() == 0) {
      display_test_result("C11 shmem_atomic_fetch_xor()", result, false);
    }
  } else {
    if (shmem_my_pe() == 0) {
      display_test_result("C11 shmem_atomic_fetch_xor()", result, false);
      rc = EXIT_FAILURE;
    }
  }

  shmem_finalize();
  return rc;
}

