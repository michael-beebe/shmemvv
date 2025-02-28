/**
 * @file c_shmem_atomic_fetch_and_nbi.c
 * @brief Unit test for shmem_atomic_fetch_and_nbi
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C_SHMEM_ATOMIC_FETCH_AND_NBI(TYPE, TYPENAME)                      \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_atomic_fetch_and_nbi");                   \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    fetch = 0;                                                                 \
    TYPE value = 42, and_val = 15;                                             \
    *dest = value;                                                             \
    log_info("set %p to %d", (void *)dest, (char)value);                       \
    shmem_barrier_all();                                                       \
    log_info("executing atomic fetch and nbi: dest = %p, and_val = %d",        \
             (void *)dest, (char)and_val);                                     \
    int mype = shmem_my_pe();                                                  \
    shmem_##TYPENAME##_atomic_fetch_and_nbi(&fetch, dest, and_val, mype);      \
    shmem_quiet();                                                             \
    shmem_barrier_all();                                                       \
    success = (fetch == value && *dest == (value & and_val));                  \
    if (!success)                                                              \
      log_fail(                                                                \
          "atomic fetch and nbi on %s did not produce expected value %d, got " \
          "instead %d",                                                        \
          #TYPE, (char)(value & and_val), (char)*dest);                        \
    else                                                                       \
      log_info("atomic fetch and nbi on a %s at %p produced expected result "  \
               "(%d == %d && %d == %d)",                                       \
               #TYPE, dest, value, fetch, and_val, *dest);                     \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);
  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C_SHMEM_ATOMIC_FETCH_AND_NBI(unsigned int, uint);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_AND_NBI(unsigned long, ulong);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_AND_NBI(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_AND_NBI(int32_t, int32);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_AND_NBI(int64_t, int64);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_AND_NBI(uint32_t, uint32);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_AND_NBI(uint64_t, uint64);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_atomic_fetch_and_nbi()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
