/**
 * @file c_shmem_atomic_fetch_xor_nbi.c
 * @brief Unit test for shmem_atomic_fetch_xor_nbi
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "shmemvv.h"

#define TEST_C_SHMEM_ATOMIC_FETCH_XOR_NBI(TYPE, TYPENAME)                      \
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

int main(int argc, char *argv[]) {
  shmem_init();

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C_SHMEM_ATOMIC_FETCH_XOR_NBI(unsigned int, uint);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_XOR_NBI(unsigned long, ulong);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_XOR_NBI(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_XOR_NBI(int32_t, int32);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_XOR_NBI(int64_t, int64);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_XOR_NBI(uint32_t, uint32);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_XOR_NBI(uint64_t, uint64);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_atomic_fetch_xor_nbi()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return rc;
}
