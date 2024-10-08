/**
 * @file c_shmem_atomic_add.c
 * @brief Unit test for shmem_atomic_add
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"

#define TEST_C_SHMEM_ATOMIC_ADD(TYPE, TYPENAME)                              \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    TYPE value = 42, add_val = 10;                                             \
    *dest = value;                                                             \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    shmem_##TYPENAME##_atomic_add(dest, add_val, mype);                       \
    shmem_barrier_all();                                                       \
    success = (*dest == value + add_val);                                        \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C_SHMEM_ATOMIC_ADD(int, int);
  result &= TEST_C_SHMEM_ATOMIC_ADD(long, long);
  result &= TEST_C_SHMEM_ATOMIC_ADD(long long, longlong);
  result &= TEST_C_SHMEM_ATOMIC_ADD(unsigned int, uint);
  result &= TEST_C_SHMEM_ATOMIC_ADD(unsigned long, ulong);
  result &= TEST_C_SHMEM_ATOMIC_ADD(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_ATOMIC_ADD(int32_t, int32);
  result &= TEST_C_SHMEM_ATOMIC_ADD(int64_t, int64);
  result &= TEST_C_SHMEM_ATOMIC_ADD(uint32_t, uint32);
  result &= TEST_C_SHMEM_ATOMIC_ADD(uint64_t, uint64);
  result &= TEST_C_SHMEM_ATOMIC_ADD(size_t, size);
  result &= TEST_C_SHMEM_ATOMIC_ADD(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (result) {
    if (shmem_my_pe() == 0) {
      display_test_result("C shmem_atomic_add()", result, false);
    }
  } else {
    if (shmem_my_pe() == 0) {
      display_test_result("C shmem_atomic_add()", result, false);
      rc = EXIT_FAILURE;
    }
  }

  shmem_finalize();

  return rc;
}
