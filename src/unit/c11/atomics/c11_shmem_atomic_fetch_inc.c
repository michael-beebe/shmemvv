/**
 * @file c11_shmem_atomic_fetch_inc.c
 * @brief Unit test for shmem_atomic_fetch_inc
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"

#define TEST_C11_SHMEM_ATOMIC_FETCH_INC(TYPE)                        \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    TYPE value = 42;                                                           \
    *dest = value;                                                             \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    fetch = shmem_atomic_fetch_inc(dest, mype);                   \
    shmem_barrier_all();                                                       \
    success = (fetch == value && *dest == value + 1);                          \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C11_SHMEM_ATOMIC_FETCH_INC(int);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_INC(long);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_INC(long long);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_INC(unsigned int);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_INC(unsigned long);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_INC(unsigned long long);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_INC(int32_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_INC(int64_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_INC(uint32_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_INC(uint64_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_INC(size_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_INC(ptrdiff_t);

  shmem_barrier_all();

  if (result) {
    if (shmem_my_pe() == 0) {
      display_test_result("C11 shmem_atomic_fetch_inc()", result, false);
    }
  } else {
    if (shmem_my_pe() == 0) {
      display_test_result("C11 shmem_atomic_fetch_inc()", result, false);
      rc = EXIT_FAILURE;
    }
  }

  shmem_finalize();
  return rc;
}
