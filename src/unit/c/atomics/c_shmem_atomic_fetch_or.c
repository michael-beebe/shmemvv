/**
 * @file c_shmem_atomic_fetch_or.c
 * @brief Unit test for shmem_atomic_fetch_or
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"

#define TEST_C_SHMEM_ATOMIC_FETCH_OR(TYPE, TYPENAME)                         \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    TYPE value = 42, or_val = 15;                                              \
    *dest = value;                                                             \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    fetch = shmem_##TYPENAME##_atomic_fetch_or(dest, or_val, mype);            \
    shmem_barrier_all();                                                       \
    success = (fetch == value && *dest == (value | or_val));                   \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C_SHMEM_ATOMIC_FETCH_OR(unsigned int, uint);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_OR(unsigned long, ulong);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_OR(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_OR(int32_t, int32);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_OR(int64_t, int64);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_OR(uint32_t, uint32);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_OR(uint64_t, uint64);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_atomic_fetch_or()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return rc;
}
