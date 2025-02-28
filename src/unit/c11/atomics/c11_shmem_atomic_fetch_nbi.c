/**
 * @file c11_shmem_atomic_fetch_nbi.c
 * @brief Unit test for shmem_atomic_fetch_nbi
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"
#include "log.h"

#define TEST_C11_SHMEM_ATOMIC_FETCH_NBI(TYPE)                                  \
  ({                                                                           \
    log_routine("shmem_atomic_fetch_nbi(" #TYPE ")");                          \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    TYPE value = 42;                                                           \
    *dest = value;                                                             \
    log_info("set %p to %d", (void *)dest, (char)value);                       \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    log_info("executing atomic fetch (nbi): dest = %p", (void *)dest);         \
    shmem_atomic_fetch_nbi(&fetch, dest, mype);                                \
    shmem_quiet();                                                             \
    shmem_barrier_all();                                                       \
    success = (fetch == value);                                                \
    if (!success)                                                              \
      log_fail("atomic fetch on %s did not produce expected value %d, got "    \
               "instead %d",                                                   \
               #TYPE, (char)value, (char)fetch);                               \
    else                                                                       \
      log_info("atomic fetch on a %s at %p produced expected result", #TYPE,   \
               dest);                                                          \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C11_SHMEM_ATOMIC_FETCH_NBI(int);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_NBI(long);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_NBI(long long);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_NBI(unsigned int);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_NBI(unsigned long);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_NBI(unsigned long long);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_NBI(int32_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_NBI(int64_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_NBI(uint32_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_NBI(uint64_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_NBI(size_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_NBI(ptrdiff_t);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_atomic_fetch_nbi()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
