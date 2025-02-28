/**
 * @file c11_shmem_atomic_swap.c
 * @brief Unit test for shmem_atomic_swap
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C11_SHMEM_ATOMIC_SWAP(TYPE)                                       \
  ({                                                                           \
    log_routine("shmem_atomic_set(" #TYPE ")");                                \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    TYPE value = 42, new_val = 43;                                             \
    *dest = value;                                                             \
    log_info("set %p to %d", (void *)dest, (char)value);                       \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    log_info("executing atomic swap: dest = %p, new_val = %d", (void *)dest,   \
             (char)new_val);                                                   \
    TYPE swapped = shmem_atomic_swap(dest, new_val, mype);                     \
    shmem_barrier_all();                                                       \
    success = (swapped == value && *dest == new_val);                          \
    if (!success)                                                              \
      log_fail("atomic swap on %s did not produce expected value = %d, "       \
               "return = %d, got "                                             \
               "instead value = %d, return = %d",                              \
               #TYPE, (char)new_val, (char)value, (char)*dest, (char)swapped); \
    else                                                                       \
      log_info("atomic swap on a %s at %p produced expected result", #TYPE,    \
               dest);                                                          \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C11_SHMEM_ATOMIC_SWAP(int);
  result &= TEST_C11_SHMEM_ATOMIC_SWAP(long);
  result &= TEST_C11_SHMEM_ATOMIC_SWAP(long long);
  result &= TEST_C11_SHMEM_ATOMIC_SWAP(unsigned int);
  result &= TEST_C11_SHMEM_ATOMIC_SWAP(unsigned long);
  result &= TEST_C11_SHMEM_ATOMIC_SWAP(unsigned long long);
  result &= TEST_C11_SHMEM_ATOMIC_SWAP(int32_t);
  result &= TEST_C11_SHMEM_ATOMIC_SWAP(int64_t);
  result &= TEST_C11_SHMEM_ATOMIC_SWAP(uint32_t);
  result &= TEST_C11_SHMEM_ATOMIC_SWAP(uint64_t);
  result &= TEST_C11_SHMEM_ATOMIC_SWAP(size_t);
  result &= TEST_C11_SHMEM_ATOMIC_SWAP(ptrdiff_t);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_atomic_swap()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
