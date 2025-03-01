/**
 * @file c11_shmem_atomic_compare_swap.c
 * @brief Unit test for shmem_atomic_compare_swap
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP(TYPE)                               \
  ({                                                                           \
    log_routine("shmem_atomic_compare_swap(" #TYPE ")");                       \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    TYPE old = 42, new_val = 43;                                               \
    *dest = old;                                                               \
    log_info("set %p to %d", (void *)dest, (char)old);                         \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    shmem_barrier_all();                                                       \
    log_info("executing atomic cmp swp: dest = %p, value = %d, new = %d",      \
             (void *)dest, (char)*dest, (char)new_val);                        \
    TYPE swapped =                                                             \
        shmem_atomic_compare_swap(dest, old, new_val, (mype + 1) % npes);      \
    shmem_barrier_all();                                                       \
    success = (swapped == old && *dest == new_val);                            \
    if (!success)                                                              \
      log_fail(                                                                \
          "atomic cmp swp on %s did not produce expected value = %d, ret = "   \
          "%d, got "                                                           \
          "instead value = %d, ret = %d",                                      \
          #TYPE, (char)(new_val), (char)old, (char)*dest, (char)swapped);      \
    else                                                                       \
      log_info("atomic cmp swp on a %s at %p produced expected result", #TYPE, \
               (void *)dest);                                                  \
    shmem_barrier_all();                                                       \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP(int);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP(long);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP(long long);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP(unsigned int);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP(unsigned long);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP(unsigned long long);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP(int32_t);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP(int64_t);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP(uint32_t);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP(uint64_t);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP(size_t);
  result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP(ptrdiff_t);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_atomic_compare_swap", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
