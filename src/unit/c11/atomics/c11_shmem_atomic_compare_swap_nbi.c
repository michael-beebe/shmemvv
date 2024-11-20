/**
 * @file c11_shmem_atomic_compare_swap_nbi.c
 * @brief Unit test for shmem_atomic_compare_swap_nbi
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"
#include "log.h"

#define TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP_NBI(TYPE)                           \
  ({                                                                           \
    log_routine("shmem_atomic_compare_swap_nbi(" #TYPE ")");                   \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    fetch = 0;                                                                 \
    TYPE old = 42, new_val = 43;                                               \
    *dest = old;                                                               \
    log_info("set %p to %d", (void *)dest, (char)old);                         \
    shmem_barrier_all();                                                       \
    log_info(                                                                  \
        "executing atomic cmp swp (nbi): dest = %p, value = %d, new = %d",     \
        (void *)dest, (char)*dest, (char)new_val);                             \
    int mype = shmem_my_pe();                                                  \
    shmem_atomic_compare_swap_nbi(&fetch, dest, old, new_val, mype);           \
    shmem_quiet();                                                             \
    shmem_barrier_all();                                                       \
    success = (fetch == old && *dest == new_val);                              \
    if (!success)                                                              \
      log_fail(                                                                \
          "atomic cmp swp on %s did not produce expected value = %d, ret = "   \
          "%d, got "                                                           \
          "instead value = %d, ret = %d",                                      \
          #TYPE, (char)(new_val), (char)old, (char)*dest, (char)fetch);        \
    else                                                                       \
      log_info("atomic cmp swp on a %s at %p produced expected result", #TYPE, \
               (void *)dest);                                                  \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP_NBI(int);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP_NBI(long);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP_NBI(long long);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP_NBI(unsigned int);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP_NBI(unsigned long);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP_NBI(unsigned long long);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP_NBI(int32_t);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP_NBI(int64_t);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP_NBI(uint32_t);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP_NBI(uint64_t);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP_NBI(size_t);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP_NBI(ptrdiff_t);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_atomic_compare_swap_nbi()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
