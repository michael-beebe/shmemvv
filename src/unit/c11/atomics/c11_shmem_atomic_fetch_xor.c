/**
 * @file c11_shmem_atomic_fetch_xor.c
 * @brief Test for shmem_atomic_fetch_xor()
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C11_SHMEM_ATOMIC_FETCH_XOR(TYPE)                                  \
  ({                                                                           \
    log_routine("shmem_atomic_fetch_xor(" #TYPE ")");                           \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    TYPE value = 42, xor_val = 15;                                              \
    *dest = value;                                                             \
    log_info("set %p to %d", (void *)dest, (char)value);                       \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    log_info("executing atomic fetch xor: dest = %p, xor_val = %d",              \
             (void *)dest, (char)xor_val);                                      \
    fetch = shmem_atomic_fetch_xor(dest, xor_val, mype);                         \
    shmem_barrier_all();                                                       \
    success = (fetch == value && *dest == (value ^ xor_val));                   \
    if (!success)                                                              \
      log_fail("atomic xor on %s did not produce expected value = %d, ret = "   \
               "%d, got "                                                      \
               "instead value = %d, ret = %d",                                 \
               #TYPE, (char)(value ^ xor_val), (char)value, (char)*dest,        \
               (char)fetch);                                                   \
    else                                                                       \
      log_info("atomic fetch xor on a %s at %p produced expected result",       \
               #TYPE, (void *)dest);                                           \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

int main(void) {
  shmem_init();
  log_init(__FILE__);

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C11_SHMEM_ATOMIC_FETCH_XOR(unsigned int);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_XOR(unsigned long);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_XOR(unsigned long long);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_XOR(int32_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_XOR(int64_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_XOR(uint32_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_XOR(uint64_t);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_atomic_fetch_xor()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
