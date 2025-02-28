/**
 * @file c11_shmem_atomic_xor.c
 * @brief Unit test for shmem_atomic_xor
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"
#include "log.h"

#define TEST_C11_SHMEM_ATOMIC_XOR(TYPE)                                        \
  ({                                                                           \
    log_routine("shmem_atomic_xor(" #TYPE ")");                                \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    TYPE value = 42, xor_val = 15;                                             \
    *dest = value;                                                             \
    log_info("set %p to %d", (void *)dest, (char)value);                       \
    shmem_barrier_all();                                                       \
    log_info("executing atomic xor: dest = %p, xor_val = %d", (void *)dest,    \
             (char)xor_val);                                                   \
    int mype = shmem_my_pe();                                                  \
    shmem_atomic_xor(dest, xor_val, mype);                                     \
    shmem_barrier_all();                                                       \
    success = (*dest == (value ^ xor_val));                                    \
    if (!success)                                                              \
      log_fail("atomic xor on %s did not produce expected value %d, got "      \
               "instead %d",                                                   \
               #TYPE, (char)(value ^ xor_val), (char)*dest);                   \
    else                                                                       \
      log_info(                                                                \
          "atomic xor on a %s at %p produced expected result (%d ^ %d = %d)",  \
          #TYPE, dest, value, xor_val, *dest);                                 \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C11_SHMEM_ATOMIC_XOR(unsigned int);
  result &= TEST_C11_SHMEM_ATOMIC_XOR(unsigned long);
  result &= TEST_C11_SHMEM_ATOMIC_XOR(unsigned long long);
  result &= TEST_C11_SHMEM_ATOMIC_XOR(int32_t);
  result &= TEST_C11_SHMEM_ATOMIC_XOR(int64_t);
  result &= TEST_C11_SHMEM_ATOMIC_XOR(uint32_t);
  result &= TEST_C11_SHMEM_ATOMIC_XOR(uint64_t);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_atomic_xor()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
