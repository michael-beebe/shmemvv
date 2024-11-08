/**
 * @file c11_shmem_atomic_fetch_add.c
 * @brief Unit test for shmem_atomic_fetch_add
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C11_SHMEM_ATOMIC_FETCH_ADD(TYPE)                                  \
  ({                                                                           \
    log_routine("shmem_atomic_fetch_add(" #TYPE ")");                           \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    TYPE value = 42, add_val = 10;                                              \
    *dest = value;                                                             \
    log_info("set %p to %d", (void *)dest, (char)value);                       \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    log_info("executing atomic fetch add: dest = %p, add_val = %d",              \
             (void *)dest, (char)add_val);                                      \
    fetch = shmem_atomic_fetch_add(dest, add_val, mype);                         \
    shmem_barrier_all();                                                       \
    success = (fetch == value && *dest == (value + add_val));                   \
    if (!success)                                                              \
      log_fail("atomic add on %s did not produce expected value = %d, ret = "   \
               "%d, got "                                                      \
               "instead value = %d, ret = %d",                                 \
               #TYPE, (char)(value + add_val), (char)value, (char)*dest,        \
               (char)fetch);                                                   \
    else                                                                       \
      log_info("atomic fetch add on a %s at %p produced expected result",       \
               #TYPE, (void *)dest);                                           \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C11_SHMEM_ATOMIC_FETCH_ADD(int);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_ADD(long);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_ADD(long long);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_ADD(unsigned int);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_ADD(unsigned long);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_ADD(unsigned long long);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_ADD(int32_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_ADD(int64_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_ADD(uint32_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_ADD(uint64_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_ADD(size_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_ADD(ptrdiff_t);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_atomic_fetch_add()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
