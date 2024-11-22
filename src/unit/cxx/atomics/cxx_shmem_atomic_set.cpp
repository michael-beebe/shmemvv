/**
 * @file cxx_shmem_atomic_set.cpp
 * @brief Unit test for shmem_atomic_set
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_CXX_SHMEM_ATOMIC_SET(TYPE, TYPENAME)                              \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_atomic_set");                             \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE set;                                                           \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    TYPE value = 42;                                                           \
    *dest = value;                                                             \
    log_info("set %p to %d", (void *)dest, (char)value);                       \
    shmem_barrier_all();                                                       \
    log_info("executing atomic set: dest = %p", (void *)dest);                 \
    int mype = shmem_my_pe();                                                  \
    shmem_##TYPENAME##_atomic_set(dest, value, mype);                          \
    shmem_barrier_all();                                                       \
    success = (*dest == value);                                                \
    if (!success)                                                              \
      log_fail("atomic set on %s did not produce expected value: "             \
               "dest = %d (expected %d)",                                      \
               #TYPE, (char)*dest, (char)value);                               \
    else                                                                       \
      log_info("atomic set on a %s at %p produced expected result: "           \
               "dest = %d",                                                    \
               #TYPE, dest, (char)*dest);                                      \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

// TODO: add macro for the ctx version

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_CXX_SHMEM_ATOMIC_SET(int, int);
  result &= TEST_CXX_SHMEM_ATOMIC_SET(long, long);
  result &= TEST_CXX_SHMEM_ATOMIC_SET(long long, longlong);
  result &= TEST_CXX_SHMEM_ATOMIC_SET(unsigned int, uint);
  result &= TEST_CXX_SHMEM_ATOMIC_SET(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_ATOMIC_SET(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_ATOMIC_SET(int32_t, int32);
  result &= TEST_CXX_SHMEM_ATOMIC_SET(int64_t, int64);
  result &= TEST_CXX_SHMEM_ATOMIC_SET(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_ATOMIC_SET(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_ATOMIC_SET(size_t, size);
  result &= TEST_CXX_SHMEM_ATOMIC_SET(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_atomic_set()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
