/**
 * @file c11_shmem_atomic_set.c
 * @brief Unit test for shmem_atomic_set
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C11_SHMEM_ATOMIC_SET(TYPE)                                        \
  ({                                                                           \
    log_routine("shmem_atomic_set(" #TYPE ")");                                \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE set;                                                           \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    TYPE value = 42;                                                           \
    *dest = 43;                                                                \
    log_info("set %p to %d", (void *)dest, 43);                                \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    shmem_atomic_set(dest, value, mype);                                       \
    log_info("executing atomic set: dest = %p, val = %d", (void *)dest,        \
             (char)value);                                                     \
    shmem_barrier_all();                                                       \
    success = (*dest == value);                                                \
    if (!success)                                                              \
      log_fail("atomic set on %s did not produce expected value %d, got "      \
               "instead %d",                                                   \
               #TYPE, (char)value, (char)*dest);                               \
    else                                                                       \
      log_info("atomic or on a %s at %p produced expected result (%d = %d)",   \
               #TYPE, (void *)dest, (char)*dest, (char)value);                 \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C11_SHMEM_ATOMIC_SET(int);
  result &= TEST_C11_SHMEM_ATOMIC_SET(long);
  result &= TEST_C11_SHMEM_ATOMIC_SET(long long);
  result &= TEST_C11_SHMEM_ATOMIC_SET(unsigned int);
  result &= TEST_C11_SHMEM_ATOMIC_SET(unsigned long);
  result &= TEST_C11_SHMEM_ATOMIC_SET(unsigned long long);
  result &= TEST_C11_SHMEM_ATOMIC_SET(int32_t);
  result &= TEST_C11_SHMEM_ATOMIC_SET(int64_t);
  result &= TEST_C11_SHMEM_ATOMIC_SET(uint32_t);
  result &= TEST_C11_SHMEM_ATOMIC_SET(uint64_t);
  result &= TEST_C11_SHMEM_ATOMIC_SET(size_t);
  result &= TEST_C11_SHMEM_ATOMIC_SET(ptrdiff_t);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_atomic_set", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
