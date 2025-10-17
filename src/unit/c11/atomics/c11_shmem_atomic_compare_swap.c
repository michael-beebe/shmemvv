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
#include "type_tables.h"

#define TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP(TYPE)                               \
  ({                                                                           \
    log_routine("shmem_atomic_compare_swap(" #TYPE ")");                       \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    int prev_pe = (mype + npes - 1) % npes, next_pe = (mype + 1) % npes;       \
    TYPE value = mype, new_val = next_pe;                                      \
    *dest = value;                                                             \
    log_info("initialized dest at %p to %d", (void *)dest, (int)value);        \
    shmem_barrier_all();                                                       \
    log_info(                                                                  \
        "executing atomic compare swap: dest = %p, cond = %d, value = %d",     \
        (void *)dest, (int)next_pe, (int)new_val);                             \
    TYPE swapped =                                                             \
        shmem_atomic_compare_swap(dest, new_val, value, next_pe);              \
    shmem_barrier_all();                                                       \
    success = (swapped == next_pe && *dest == prev_pe);                        \
    if (!success)                                                              \
      log_fail("atomic compare swap on %s did not produce expected values: "   \
               "swapped = %d (expected %d), dest = %d (expected %d)",          \
               #TYPE, (int)swapped, (int)next_pe, (int)*dest, (int)prev_pe);   \
    else                                                                       \
      log_info("atomic compare swap on a %s at %p produced expected result "   \
               "(swapped = %d, dest = %d)",                                    \
               #TYPE, (void *)dest, (int)swapped, (int)*dest);                 \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

#define TEST_C11_CTX_SHMEM_ATOMIC_COMPARE_SWAP(TYPE)                           \
  ({                                                                           \
    log_routine("shmem_atomic_compare_swap(ctx, " #TYPE ")");                  \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    int prev_pe = (mype + npes - 1) % npes, next_pe = (mype + 1) % npes;       \
    TYPE value = mype, new_val = next_pe;                                      \
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
    log_info(                                                                  \
        "executing atomic compare swap with context: dest = %p, cond = %d, "   \
        "value = %d",                                                          \
        (void *)dest, (int)next_pe, (int)new_val);                             \
    TYPE swapped =                                                             \
        shmem_atomic_compare_swap(ctx, dest, new_val, value, next_pe);         \
    shmem_ctx_quiet(ctx);                                                      \
    shmem_barrier_all();                                                       \
    success = (swapped == next_pe && *dest == prev_pe);                        \
    if (!success)                                                              \
      log_fail(                                                                \
          "atomic compare swap with context on %s did not produce expected "   \
          "values: swapped = %d (expected %d), dest = %d (expected %d)",       \
          #TYPE, (int)swapped, (int)next_pe, (int)*dest, (int)prev_pe);        \
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

  if (!(shmem_n_pes() >= 2)) {
    log_warn("Not enough PEs to run test (requires 2 PEs, have %d PEs)",
             shmem_n_pes());
    if (shmem_my_pe() == 0) {
      display_not_enough_pes("atomic");
    }
    shmem_finalize();
    return EXIT_SUCCESS;
  }

  static int result = true;
  static int result_ctx = true;

  /* Test standard atomic add operations */
  #define X(type, shmem_types) result &= TEST_C11_SHMEM_ATOMIC_COMPARE_SWAP(type);
    SHMEM_STANDARD_AMO_TYPE_TABLE(X)
  #undef X

  shmem_barrier_all();

  reduce_test_result("C11 shmem_atomic_compare_swap", &result, false);


  /* Test context-specific atomic add operations */
  #define X(type, shmem_types) result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_COMPARE_SWAP(type);
    SHMEM_STANDARD_AMO_TYPE_TABLE(X)
  #undef X

  shmem_barrier_all();

  reduce_test_result("C11 shmem_atomic_compare_swap with ctx", &result_ctx, false);

  bool rc = result & result_ctx ? EXIT_SUCCESS : EXIT_FAILURE;
  log_close(rc);
  shmem_finalize();
  return rc;
}
