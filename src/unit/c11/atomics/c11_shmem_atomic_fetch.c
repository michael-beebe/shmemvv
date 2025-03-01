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
#include "log.h"

#define TEST_C11_SHMEM_ATOMIC_FETCH(TYPE)                                      \
  ({                                                                           \
    log_routine("shmem_atomic_fetch(" #TYPE ")");                              \
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
    log_info("executing atomic fetch: dest = %p", (void *)dest);               \
    fetch = shmem_atomic_fetch(dest, mype);                                    \
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
    display_test_result("C11 shmem_atomic_fetch", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
