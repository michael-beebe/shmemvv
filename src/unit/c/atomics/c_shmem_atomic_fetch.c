/**
 * @file c_shmem_atomic_fetch.c
 * @brief Unit test for shmem_atomic_fetch
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C_SHMEM_ATOMIC_FETCH(TYPE, TYPENAME)                              \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_atomic_fetch");                           \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    TYPE value = 42;                                                           \
    *dest = value;                                                             \
    log_info("set %p to %d", (void *)dest, (char)value);                       \
    shmem_barrier_all();                                                       \
    log_info("executing atomic fetch: dest = %p", (void *)dest);               \
    int mype = shmem_my_pe();                                                  \
    fetch = shmem_##TYPENAME##_atomic_fetch(dest, mype);                       \
    shmem_barrier_all();                                                       \
    success = (fetch == value);                                                \
    if (!success)                                                              \
      log_fail("atomic fetch on %s did not produce expected value %d, "        \
               "got instead %d",                                               \
               #TYPE, (char)value, (char)*dest);                               \
    else                                                                       \
      log_info(                                                                \
          "atomic fetch on a %s at %p produced expected result (%d == %d)",    \
          #TYPE, dest, value, fetch);                                          \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C_SHMEM_ATOMIC_FETCH(int, int);
  result &= TEST_C_SHMEM_ATOMIC_FETCH(long, long);
  result &= TEST_C_SHMEM_ATOMIC_FETCH(long long, longlong);
  result &= TEST_C_SHMEM_ATOMIC_FETCH(unsigned int, uint);
  result &= TEST_C_SHMEM_ATOMIC_FETCH(unsigned long, ulong);
  result &= TEST_C_SHMEM_ATOMIC_FETCH(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_ATOMIC_FETCH(int32_t, int32);
  result &= TEST_C_SHMEM_ATOMIC_FETCH(int64_t, int64);
  result &= TEST_C_SHMEM_ATOMIC_FETCH(uint32_t, uint32);
  result &= TEST_C_SHMEM_ATOMIC_FETCH(uint64_t, uint64);
  result &= TEST_C_SHMEM_ATOMIC_FETCH(size_t, size);
  result &= TEST_C_SHMEM_ATOMIC_FETCH(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_atomic_fetch()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
