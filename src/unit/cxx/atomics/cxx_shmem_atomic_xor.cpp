/**
 * @file cxx_shmem_atomic_xor.cpp
 * @brief Unit test for shmem_atomic_xor
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_CXX_SHMEM_ATOMIC_XOR(TYPE, TYPENAME)                              \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_atomic_xor");                             \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    TYPE value = 42, xor_val = 15;                                             \
    *dest = value;                                                             \
    log_info("set %p to %d", (void *)dest, (char)value);                       \
    shmem_barrier_all();                                                       \
    log_info("executing atomic xor: dest = %p, xor_val = %d",                  \
             (void *)dest, (char)xor_val);                                     \
    int mype = shmem_my_pe();                                                  \
    shmem_##TYPENAME##_atomic_xor(dest, xor_val, mype);                        \
    shmem_barrier_all();                                                       \
    success = (*dest == (value ^ xor_val));                                    \
    if (!success)                                                              \
      log_fail("atomic xor on %s did not produce expected value: "             \
               "dest = %d (expected %d)", #TYPE, (char)*dest,                  \
               (char)(value ^ xor_val));                                       \
    else                                                                       \
      log_info("atomic xor on a %s at %p produced expected result: dest = %d", \
               #TYPE, dest, (char)*dest);                                      \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_CXX_SHMEM_ATOMIC_XOR(unsigned int, uint);
  result &= TEST_CXX_SHMEM_ATOMIC_XOR(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_ATOMIC_XOR(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_ATOMIC_XOR(int32_t, int32);
  result &= TEST_CXX_SHMEM_ATOMIC_XOR(int64_t, int64);
  result &= TEST_CXX_SHMEM_ATOMIC_XOR(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_ATOMIC_XOR(uint64_t, uint64);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_atomic_xor()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
