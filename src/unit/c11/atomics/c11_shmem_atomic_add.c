/**
 * @file c11_shmem_atomic_add.c
 * @brief Unit test for shmem_atomic_add
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"

#define TEST_C11_SHMEM_ATOMIC_ADD(TYPE)                              \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    TYPE value = 42, add_val = 10;                                             \
    *dest = value;                                                             \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    shmem_atomic_add(dest, add_val, mype);                       \
    shmem_barrier_all();                                                       \
    success = (*dest == value + add_val);                                        \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C11_SHMEM_ATOMIC_ADD(int);
  result &= TEST_C11_SHMEM_ATOMIC_ADD(long);
  result &= TEST_C11_SHMEM_ATOMIC_ADD(long long);
  result &= TEST_C11_SHMEM_ATOMIC_ADD(unsigned int);
  result &= TEST_C11_SHMEM_ATOMIC_ADD(unsigned long);
  result &= TEST_C11_SHMEM_ATOMIC_ADD(unsigned long long);
  result &= TEST_C11_SHMEM_ATOMIC_ADD(int32_t);
  result &= TEST_C11_SHMEM_ATOMIC_ADD(int64_t);
  result &= TEST_C11_SHMEM_ATOMIC_ADD(uint32_t);
  result &= TEST_C11_SHMEM_ATOMIC_ADD(uint64_t);
  result &= TEST_C11_SHMEM_ATOMIC_ADD(size_t);
  result &= TEST_C11_SHMEM_ATOMIC_ADD(ptrdiff_t);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_atomic_add()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();

  return rc;
}
