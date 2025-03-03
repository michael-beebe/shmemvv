/**
 * @file c11_shmem_atomic_swap.c
 * @brief Unit test for shmem_atomic_swap
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C11_SHMEM_ATOMIC_SWAP(TYPE)                                       \
  ({                                                                           \
    log_routine("shmem_atomic_swap(" #TYPE ")");                               \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    TYPE value = 42, new_val = 43;                                             \
    *dest = value;                                                             \
    log_info("initialized dest at %p to %d", (void *)dest, (int)value);        \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    log_info("executing atomic swap: dest = %p, value = %d", (void *)dest,     \
             (int)new_val);                                                    \
    TYPE swapped = shmem_atomic_swap(dest, new_val, (mype + 1) % npes);        \
    shmem_barrier_all();                                                       \
    success = (swapped == value && *dest == new_val);                          \
    if (!success)                                                              \
      log_fail("atomic swap on %s did not produce expected values: "           \
               "swapped = %d (expected %d), dest = %d (expected %d)",          \
               #TYPE, (int)swapped, (int)value, (int)*dest, (int)new_val);     \
    else                                                                       \
      log_info("atomic swap on a %s at %p produced expected result "           \
               "(swapped = %d, dest = %d)",                                    \
               #TYPE, (void *)dest, (int)swapped, (int)*dest);                 \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

#define TEST_C11_CTX_SHMEM_ATOMIC_SWAP(TYPE)                                   \
  ({                                                                           \
    log_routine("shmem_atomic_swap(ctx, " #TYPE ")");                          \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    TYPE value = 42, new_val = 43;                                             \
    *dest = value;                                                             \
    log_info("initialized dest at %p to %d", (void *)dest, (int)value);        \
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
    log_info("executing atomic swap with context: dest = %p, value = %d",      \
             (void *)dest, (int)new_val);                                      \
    TYPE swapped = shmem_atomic_swap(ctx, dest, new_val, (mype + 1) % npes);   \
    shmem_ctx_quiet(ctx);                                                      \
    shmem_barrier_all();                                                       \
    success = (swapped == value && *dest == new_val);                          \
    if (!success)                                                              \
      log_fail(                                                                \
          "atomic swap with context on %s did not produce expected values: "   \
          "swapped = %d (expected %d), dest = %d (expected %d)",               \
          #TYPE, (int)swapped, (int)value, (int)*dest, (int)new_val);          \
    else                                                                       \
      log_info(                                                                \
          "atomic swap with context on a %s at %p produced expected result "   \
          "(swapped = %d, dest = %d)",                                         \
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

  /* Test standard atomic swap operations */
  bool result = true;
  result &= TEST_C11_SHMEM_ATOMIC_SWAP(int);
  result &= TEST_C11_SHMEM_ATOMIC_SWAP(long);
  result &= TEST_C11_SHMEM_ATOMIC_SWAP(long long);
  result &= TEST_C11_SHMEM_ATOMIC_SWAP(unsigned int);
  result &= TEST_C11_SHMEM_ATOMIC_SWAP(unsigned long);
  result &= TEST_C11_SHMEM_ATOMIC_SWAP(unsigned long long);
  result &= TEST_C11_SHMEM_ATOMIC_SWAP(int32_t);
  result &= TEST_C11_SHMEM_ATOMIC_SWAP(int64_t);
  result &= TEST_C11_SHMEM_ATOMIC_SWAP(uint32_t);
  result &= TEST_C11_SHMEM_ATOMIC_SWAP(uint64_t);
  result &= TEST_C11_SHMEM_ATOMIC_SWAP(size_t);
  result &= TEST_C11_SHMEM_ATOMIC_SWAP(ptrdiff_t);

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_atomic_swap", result, false);
  }

  /* Test context-specific atomic swap operations */
  bool result_ctx = true;
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_SWAP(int);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_SWAP(long);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_SWAP(long long);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_SWAP(unsigned int);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_SWAP(unsigned long);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_SWAP(unsigned long long);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_SWAP(int32_t);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_SWAP(int64_t);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_SWAP(uint32_t);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_SWAP(uint64_t);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_SWAP(size_t);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_SWAP(ptrdiff_t);

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_atomic_swap with ctx", result_ctx, false);
  }

  if (!result || !result_ctx) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
