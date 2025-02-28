/**
 * @file cxx_shmem_atomic_swap.cpp
 * @brief Unit test for shmem_atomic_swap
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_CXX_SHMEM_ATOMIC_SWAP(TYPE, TYPENAME)                             \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_atomic_swap");                            \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    TYPE value = 42, new_val = 43;                                             \
    *dest = value;                                                             \
    log_info("set %p to %d", (void *)dest, (char)value);                       \
    shmem_barrier_all();                                                       \
    log_info("executing atomic swap: dest = %p, new_val = %d", (void *)dest,   \
             (char)new_val);                                                   \
    int mype = shmem_my_pe();                                                  \
    TYPE swapped = shmem_##TYPENAME##_atomic_swap(dest, new_val, mype);        \
    shmem_barrier_all();                                                       \
    success = (swapped == value && *dest == new_val);                          \
    if (!success)                                                              \
      log_fail("atomic swap on %s did not produce expected values: "           \
               "swapped = %d (expected %d), dest = %d (expected %d)",          \
               #TYPE, (char)swapped, (char)value, (char)*dest, (char)new_val); \
    else                                                                       \
      log_info("atomic swap on a %s at %p produced expected results: "         \
               "swapped = %d, dest = %d",                                      \
               #TYPE, dest, (char)swapped, (char)*dest);                       \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_CXX_SHMEM_ATOMIC_SWAP(int, int);
  result &= TEST_CXX_SHMEM_ATOMIC_SWAP(long, long);
  result &= TEST_CXX_SHMEM_ATOMIC_SWAP(long long, longlong);
  result &= TEST_CXX_SHMEM_ATOMIC_SWAP(unsigned int, uint);
  result &= TEST_CXX_SHMEM_ATOMIC_SWAP(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_ATOMIC_SWAP(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_ATOMIC_SWAP(int32_t, int32);
  result &= TEST_CXX_SHMEM_ATOMIC_SWAP(int64_t, int64);
  result &= TEST_CXX_SHMEM_ATOMIC_SWAP(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_ATOMIC_SWAP(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_ATOMIC_SWAP(size_t, size);
  result &= TEST_CXX_SHMEM_ATOMIC_SWAP(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_atomic_swap()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
