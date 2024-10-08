/**
 * @file c11_shmem_atomic_compare_swap.c
 * @brief Unit test for shmem_atomic_compare_swap
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"

#define TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP(TYPE)                     \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    TYPE old = 42, new_val = 43;                                               \
    *dest = old;                                                               \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    shmem_barrier_all();                                                       \
    TYPE swapped = shmem_atomic_compare_swap(dest, old, new_val,  \
                                                          (mype + 1) % npes);  \
    shmem_barrier_all();                                                       \
    success = (swapped == old && *dest == new_val);                            \
    shmem_barrier_all();                                                       \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP(int);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP(long);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP(long long);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP(unsigned int);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP(unsigned long);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP(unsigned long long);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP(int32_t);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP(int64_t);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP(uint32_t);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP(uint64_t);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP(size_t);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP(ptrdiff_t);

  shmem_barrier_all();

  if (result) {
    if (shmem_my_pe() == 0) {
      display_test_result("C11 shmem_atomic_compare_swap()", result, false);
    }
  } else {
    if (shmem_my_pe() == 0) {
      display_test_result("C11 shmem_atomic_compare_swap()", result, false);
      rc = EXIT_FAILURE;
    }
  }

  shmem_finalize();
  return rc;
}

