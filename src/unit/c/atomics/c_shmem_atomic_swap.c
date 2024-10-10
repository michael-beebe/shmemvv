/**
 * @file c_shmem_atomic_swap.c
 * @brief Unit test for shmem_atomic_swap
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"

#define TEST_C_SHMEM_ATOMIC_SWAP(TYPE, TYPENAME)                             \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    TYPE value = 42, new_val = 43;                                             \
    *dest = value;                                                             \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    TYPE swapped = shmem_##TYPENAME##_atomic_swap(dest, new_val, mype);        \
    shmem_barrier_all();                                                       \
    success = (swapped == value && *dest == new_val);                          \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C_SHMEM_ATOMIC_SWAP(int, int);
  result &= TEST_C_SHMEM_ATOMIC_SWAP(long, long);
  result &= TEST_C_SHMEM_ATOMIC_SWAP(long long, longlong);
  result &= TEST_C_SHMEM_ATOMIC_SWAP(unsigned int, uint);
  result &= TEST_C_SHMEM_ATOMIC_SWAP(unsigned long, ulong);
  result &= TEST_C_SHMEM_ATOMIC_SWAP(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_ATOMIC_SWAP(int32_t, int32);
  result &= TEST_C_SHMEM_ATOMIC_SWAP(int64_t, int64);
  result &= TEST_C_SHMEM_ATOMIC_SWAP(uint32_t, uint32);
  result &= TEST_C_SHMEM_ATOMIC_SWAP(uint64_t, uint64);
  result &= TEST_C_SHMEM_ATOMIC_SWAP(size_t, size);
  result &= TEST_C_SHMEM_ATOMIC_SWAP(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_atomic_swap()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return rc;
}
