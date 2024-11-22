/**
 * @file cxx_shmem_atomic_fetch_add_nbi.cpp
 * @brief Unit test for shmem_atomic_fetch_add_nbi
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_CXX_SHMEM_ATOMIC_FETCH_ADD_NBI(TYPE, TYPENAME)                    \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_atomic_fetch_add_nbi");                   \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    fetch = 0;                                                                 \
    TYPE value = 42, add_val = 10;                                             \
    *dest = value;                                                             \
    log_info("set %p to %d", (void *)dest, (char)value);                       \
    shmem_barrier_all();                                                       \
    log_info("executing atomic fetch_add_nbi: dest = %p, add_val = %d",        \
             (void *)dest, (char)add_val);                                     \
    int mype = shmem_my_pe();                                                  \
    shmem_##TYPENAME##_atomic_fetch_add_nbi(&fetch, dest, add_val, mype);      \
    shmem_quiet();                                                             \
    shmem_barrier_all();                                                       \
    success = (fetch == value && *dest == value + add_val);                    \
    if (!success)                                                              \
      log_fail("atomic fetch_add_nbi on %s did not produce expected values: "  \
               "fetch = %d (expected %d), dest = %d (expected %d)",            \
               #TYPE, (char)fetch, (char)value, (char)*dest,                   \
               (char)(value + add_val));                                       \
    else                                                                       \
      log_info(                                                                \
          "atomic fetch_add_nbi on a %s at %p produced expected results: "     \
          "fetch = %d, dest = %d",                                             \
          #TYPE, dest, (char)fetch, (char)*dest);                              \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_ADD_NBI(int, int);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_ADD_NBI(long, long);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_ADD_NBI(long long, longlong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_ADD_NBI(unsigned int, uint);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_ADD_NBI(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_ADD_NBI(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_ADD_NBI(int32_t, int32);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_ADD_NBI(int64_t, int64);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_ADD_NBI(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_ADD_NBI(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_ADD_NBI(size_t, size);
  result &= TEST_CXX_SHMEM_ATOMIC_FETCH_ADD_NBI(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_atomic_fetch_add_nbi()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
