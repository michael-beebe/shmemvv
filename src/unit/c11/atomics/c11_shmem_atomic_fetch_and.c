/**
 * @file c11_shmem_atomic_fetch_and.c
 * @brief Unit test for shmem_atomic_fetch_and
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"

#define TEST_C11_SHMEM_ATOMIC_FETCH_AND(TYPE)                                  \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    TYPE value = 42, and_val = 15;                                             \
    *dest = value;                                                             \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    fetch = shmem_atomic_fetch_and(dest, and_val, mype);                       \
    shmem_barrier_all();                                                       \
    success = (fetch == value && *dest == (value & and_val));                  \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C11_SHMEM_ATOMIC_FETCH_AND(unsigned int);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_AND(unsigned long);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_AND(unsigned long long);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_AND(int32_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_AND(int64_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_AND(uint32_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_AND(uint64_t);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_atomic_fetch_and()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();

  return rc;
}
