/**
 * @file c11_shmem_atomic_fetch_inc.c
 * @brief Unit test for shmem_atomic_fetch_inc
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C11_SHMEM_ATOMIC_FETCH_INC(TYPE)                                  \
  ({                                                                           \
    log_routine("shmem_atomic_fetch_inc(" #TYPE ")");                          \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    TYPE value = 42;                                                           \
    *dest = value;                                                             \
    log_info("set %p to %d", (void *)dest, (char)value);                       \
    shmem_barrier_all();                                                       \
    log_info("executing atomic fetch inc: dest = %p, value = %d",              \
             (void *)dest, (char)*dest);                                       \
    int mype = shmem_my_pe();                                                  \
    fetch = shmem_atomic_fetch_inc(dest, mype);                                \
    shmem_barrier_all();                                                       \
    success = (fetch == value && *dest == value + 1);                          \
    if (!success)                                                              \
      log_fail("atomic fetch inc on %s did not produce expected ret = %d, "    \
               "*dest = %d, got "                                              \
               "instead ret = %d, *dest = %d",                                 \
               #TYPE, (char)value, (char)(value + 1), (char)fetch,             \
               (char)*dest);                                                   \
    else                                                                       \
      log_info("atomic fetch inc on a %s at %p produced expected result",      \
               #TYPE, dest);                                                   \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C11_SHMEM_ATOMIC_FETCH_INC(int);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_INC(long);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_INC(long long);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_INC(unsigned int);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_INC(unsigned long);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_INC(unsigned long long);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_INC(int32_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_INC(int64_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_INC(uint32_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_INC(uint64_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_INC(size_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_INC(ptrdiff_t);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_atomic_fetch_inc()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
