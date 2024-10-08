/**
 * @file c11_shmem_atomic_compare_swap_nbi.c
 * @brief Unit test for shmem_atomic_compare_swap_nbi
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"

#define TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP_NBI(TYPE)                 \
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
    shmem_atomic_compare_swap_nbi(&fetch, dest, old, new_val,     \
                                               mype);                          \
    shmem_quiet();                                                             \
    shmem_barrier_all();                                                       \
    success = (fetch == old && *dest == new_val);                              \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP_NBI(int);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP_NBI(long);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP_NBI(long long);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP_NBI(unsigned int);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP_NBI(unsigned long);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP_NBI(unsigned long long);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP_NBI(int32_t);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP_NBI(int64_t);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP_NBI(uint32_t);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP_NBI(uint64_t);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP_NBI(size_t);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP_NBI(ptrdiff_t);

  shmem_barrier_all();

  if (result) {
    if (shmem_my_pe() == 0) {
      display_test_result("C11 shmem_atomic_compare_swap_nbi()", result, false);
    }
  } else {
    if (shmem_my_pe() == 0) {
      display_test_result("C11 shmem_atomic_compare_swap_nbi()", result, false);
      rc = EXIT_FAILURE;
    }
  }
  return rc;
}
