/**
 * @file cxx_shmem_atomic_inc.cpp
 * @brief Unit test for shmem_atomic_inc
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_CXX_SHMEM_ATOMIC_INC(TYPE, TYPENAME)                              \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_atomic_inc");                             \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    TYPE value = 42;                                                           \
    *dest = value;                                                             \
    log_info("set %p to %d", (void *)dest, (char)value);                       \
    shmem_barrier_all();                                                       \
    log_info("executing atomic inc: dest = %p", (void *)dest);                 \
    int mype = shmem_my_pe();                                                  \
    shmem_##TYPENAME##_atomic_inc(dest, mype);                                 \
    shmem_barrier_all();                                                       \
    success = (*dest == value + 1);                                            \
    if (!success)                                                              \
      log_fail("atomic inc on %s did not produce expected value: "             \
               "dest = %d (expected %d)",                                      \
               #TYPE, (char)*dest, (char)(value + 1));                         \
    else                                                                       \
      log_info("atomic inc on a %s at %p produced expected result: "           \
               "dest = %d",                                                    \
               #TYPE, dest, (char)*dest);                                      \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_CXX_SHMEM_ATOMIC_INC(int, int);
  result &= TEST_CXX_SHMEM_ATOMIC_INC(long, long);
  result &= TEST_CXX_SHMEM_ATOMIC_INC(long long, longlong);
  result &= TEST_CXX_SHMEM_ATOMIC_INC(unsigned int, uint);
  result &= TEST_CXX_SHMEM_ATOMIC_INC(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_ATOMIC_INC(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_ATOMIC_INC(int32_t, int32);
  result &= TEST_CXX_SHMEM_ATOMIC_INC(int64_t, int64);
  result &= TEST_CXX_SHMEM_ATOMIC_INC(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_ATOMIC_INC(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_ATOMIC_INC(size_t, size);
  result &= TEST_CXX_SHMEM_ATOMIC_INC(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_atomic_inc()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
