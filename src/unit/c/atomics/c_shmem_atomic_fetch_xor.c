/**
 * @file c_shmem_atomic_fetch_xor.c
 * @brief Test for shmem_atomic_fetch_xor()
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C_SHMEM_ATOMIC_FETCH_XOR(TYPE, TYPENAME)                          \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_atomic_fetch_xor");                       \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    TYPE value = 42, xor_val = 15;                                             \
    *dest = value;                                                             \
    log_info("set %p to %d", (void *)dest, (char)value);                       \
    shmem_barrier_all();                                                       \
    log_info("executing atomic fetch xor: dest = %p, xor_val = %d",            \
             (void *)dest, (char)xor_val);                                     \
    int mype = shmem_my_pe();                                                  \
    fetch = shmem_##TYPENAME##_atomic_fetch_xor(dest, xor_val, mype);          \
    shmem_barrier_all();                                                       \
    success = (fetch == value && *dest == (value ^ xor_val));                  \
    if (!success)                                                              \
      log_fail("atomic fetch xor on %s did not produce expected value %d, "    \
               "got instead %d",                                               \
               #TYPE, (char)(value ^ xor_val), (char)*dest);                   \
    else                                                                       \
      log_info("atomic fetch xor on a %s at %p produced expected result "      \
               "(%d == %d && %d == %d)",                                       \
               #TYPE, dest, value, fetch, value ^ xor_val);                    \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

int main(void) {
  shmem_init();
  log_init(__FILE__);

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C_SHMEM_ATOMIC_FETCH_XOR(unsigned int, uint);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_XOR(unsigned long, ulong);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_XOR(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_XOR(int32_t, int32);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_XOR(int64_t, int64);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_XOR(uint32_t, uint32);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_XOR(uint64_t, uint64);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_atomic_fetch_xor()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
