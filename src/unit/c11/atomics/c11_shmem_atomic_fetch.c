/**
 * @file c11_shmem_atomic_fetch.c
 * @brief Unit test for shmem_atomic_fetch
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"

#define TEST_C11_SHMEM_ATOMIC_FETCH(TYPE)                                      \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    TYPE value = 42;                                                           \
    *dest = value;                                                             \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    fetch = shmem_atomic_fetch(dest, mype);                                    \
    shmem_barrier_all();                                                       \
    success = (fetch == value);                                                \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C11_SHMEM_ATOMIC_FETCH(int);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH(long);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH(long long);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH(unsigned int);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH(unsigned long);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH(unsigned long long);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH(int32_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH(int64_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH(uint32_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH(uint64_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH(size_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH(ptrdiff_t);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_atomic_fetch()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return rc;
}
