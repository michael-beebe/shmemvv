/**
 * @file c_shmem_atomic_compare_swap.c
 * @brief Unit test for shmem_atomic_compare_swap
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C_SHMEM_ATOMIC_COMPARE_SWAP(TYPE, TYPENAME)                       \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_atomic_compare_swap");                    \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    TYPE old = 42, new_val = 43;                                               \
    *dest = old;                                                               \
    log_info("set %p to %d", (void *)dest, (char)old);                         \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    shmem_barrier_all();                                                       \
    log_info(                                                                  \
        "executing atomic compare swap: dest = %p, old = %d, new_val = %d",    \
        (void *)dest, (char)old, (char)new_val);                               \
    TYPE swapped = shmem_##TYPENAME##_atomic_compare_swap(dest, old, new_val,  \
                                                          (mype + 1) % npes);  \
    shmem_barrier_all();                                                       \
    success = (swapped == old && *dest == new_val);                            \
    if (!success)                                                              \
      log_fail(                                                                \
          "atomic compare swap on %s did not produce expected value %d, got "  \
          "instead %d",                                                        \
          #TYPE, (char)new_val, (char)*dest);                                  \
    else                                                                       \
      log_info("atomic compare swap on a %s at %p produced expected result "   \
               "(%d == %d && %d == %d)",                                       \
               #TYPE, dest, old, swapped, new_val, *dest);                     \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();

  log_init(__FILE__);

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C_SHMEM_ATOMIC_COMPARE_SWAP(int, int);
  result &= TEST_C_SHMEM_ATOMIC_COMPARE_SWAP(long, long);
  result &= TEST_C_SHMEM_ATOMIC_COMPARE_SWAP(long long, longlong);
  result &= TEST_C_SHMEM_ATOMIC_COMPARE_SWAP(unsigned int, uint);
  result &= TEST_C_SHMEM_ATOMIC_COMPARE_SWAP(unsigned long, ulong);
  result &= TEST_C_SHMEM_ATOMIC_COMPARE_SWAP(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_ATOMIC_COMPARE_SWAP(int32_t, int32);
  result &= TEST_C_SHMEM_ATOMIC_COMPARE_SWAP(int64_t, int64);
  result &= TEST_C_SHMEM_ATOMIC_COMPARE_SWAP(uint32_t, uint32);
  result &= TEST_C_SHMEM_ATOMIC_COMPARE_SWAP(uint64_t, uint64);
  result &= TEST_C_SHMEM_ATOMIC_COMPARE_SWAP(size_t, size);
  result &= TEST_C_SHMEM_ATOMIC_COMPARE_SWAP(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_atomic_compare_swap()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);

  shmem_finalize();
  return rc;
}
