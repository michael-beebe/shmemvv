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
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    TYPE old = 42, new_val = 43;                                               \
    *dest = old;                                                               \
    log_info("initialized dest at %p to %d", (void *)dest, (int)old);          \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    log_info(                                                                  \
        "executing atomic compare swap: dest = %p, cond = %d, value = %d",     \
        (void *)dest, (int)old, (int)new_val);                                 \
    TYPE swapped = shmem_##TYPENAME##_atomic_compare_swap(dest, old, new_val,  \
                                                          (mype + 1) % npes);  \
    shmem_barrier_all();                                                       \
    success = (swapped == old && *dest == new_val);                            \
    if (!success)                                                              \
      log_fail("atomic compare swap on %s did not produce expected values: "   \
               "swapped = %d (expected %d), dest = %d (expected %d)",          \
               #TYPE, (int)swapped, (int)old, (int)*dest, (int)new_val);       \
    else                                                                       \
      log_info("atomic compare swap on a %s at %p produced expected result "   \
               "(swapped = %d, dest = %d)",                                    \
               #TYPE, (void *)dest, (int)swapped, (int)*dest);                 \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

#define TEST_C_CTX_SHMEM_ATOMIC_COMPARE_SWAP(TYPE, TYPENAME)                   \
  ({                                                                           \
    log_routine("shmem_ctx_" #TYPENAME "_atomic_compare_swap");                \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    TYPE old = 42, new_val = 43;                                               \
    *dest = old;                                                               \
    log_info("initialized dest at %p to %d", (void *)dest, (int)old);          \
                                                                               \
    shmem_ctx_t ctx;                                                           \
    int ctx_create_status = shmem_ctx_create(0, &ctx);                         \
    if (ctx_create_status != 0) {                                              \
      log_fail("Failed to create context");                                    \
      shmem_free(dest);                                                        \
      return false;                                                            \
    }                                                                          \
    log_info("Successfully created context");                                  \
                                                                               \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    log_info(                                                                  \
        "executing atomic compare swap with context: dest = %p, cond = %d, "   \
        "value = %d",                                                          \
        (void *)dest, (int)old, (int)new_val);                                 \
    TYPE swapped = shmem_ctx_##TYPENAME##_atomic_compare_swap(                 \
        ctx, dest, old, new_val, (mype + 1) % npes);                           \
    shmem_ctx_quiet(ctx);                                                      \
    shmem_barrier_all();                                                       \
    success = (swapped == old && *dest == new_val);                            \
    if (!success)                                                              \
      log_fail(                                                                \
          "atomic compare swap with context on %s did not produce expected "   \
          "values: swapped = %d (expected %d), dest = %d (expected %d)",       \
          #TYPE, (int)swapped, (int)old, (int)*dest, (int)new_val);            \
    else                                                                       \
      log_info(                                                                \
          "atomic compare swap with context on a %s at %p produced expected "  \
          "result (swapped = %d, dest = %d)",                                  \
          #TYPE, (void *)dest, (int)swapped, (int)*dest);                      \
                                                                               \
    shmem_ctx_destroy(ctx);                                                    \
    log_info("Context destroyed");                                             \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  int rc = EXIT_SUCCESS;

  /* Test standard atomic compare swap operations */
  bool result = true;
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

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_atomic_compare_swap", result, false);
  }

  /* Test context-specific atomic compare swap operations */
  bool result_ctx = true;
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_COMPARE_SWAP(int, int);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_COMPARE_SWAP(long, long);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_COMPARE_SWAP(long long, longlong);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_COMPARE_SWAP(unsigned int, uint);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_COMPARE_SWAP(unsigned long, ulong);
  result_ctx &=
      TEST_C_CTX_SHMEM_ATOMIC_COMPARE_SWAP(unsigned long long, ulonglong);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_COMPARE_SWAP(int32_t, int32);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_COMPARE_SWAP(int64_t, int64);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_COMPARE_SWAP(uint32_t, uint32);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_COMPARE_SWAP(uint64_t, uint64);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_COMPARE_SWAP(size_t, size);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_COMPARE_SWAP(ptrdiff_t, ptrdiff);

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_ctx_atomic_compare_swap", result_ctx, false);
  }

  if (!result || !result_ctx) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
