/**
 * @file c11_shmem_atomic_fetch_nbi.c
 * @brief Unit test for shmem_atomic_fetch_nbi
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"
#include "log.h"
#include "type_tables.h"

#define TEST_C11_SHMEM_ATOMIC_FETCH_NBI(TYPE)                                  \
  ({                                                                           \
    log_routine("shmem_atomic_fetch_nbi(" #TYPE ")");                          \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    int fetch_pe = (mype + 1) % npes;                                          \
    TYPE value = 42;                                                           \
    *dest = value + mype;                                                      \
    log_info("set %p to %d", (void *)dest, (char)*dest);                       \
    shmem_barrier_all();                                                       \
    log_info("executing atomic fetch (nbi): dest = %p", (void *)dest);         \
    shmem_atomic_fetch_nbi(&fetch, dest, fetch_pe);                            \
    shmem_quiet();                                                             \
    shmem_barrier_all();                                                       \
    success = (fetch == value + fetch_pe);                                     \
    if (!success)                                                              \
      log_fail("atomic fetch on %s did not produce expected value %d, got "    \
               "instead %d",                                                   \
               #TYPE, (char)value + fetch_pe, (char)fetch);                    \
    else                                                                       \
      log_info("atomic fetch on a %s at %p produced expected result", #TYPE,   \
               dest);                                                          \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

#define TEST_C11_CTX_SHMEM_ATOMIC_FETCH_NBI(TYPE)                              \
  ({                                                                           \
    log_routine("shmem_atomic_fetch_nbi(ctx, " #TYPE ")");                     \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    TYPE value = 42;                                                           \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    int fetch_pe = (mype + 1) % npes;                                          \
    *dest = value + mype;                                                      \
    log_info("set %p to %d", (void *)dest, (char)*dest);                       \
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
    log_info("executing atomic fetch nbi with context: dest = %p",             \
             (void *)dest);                                                    \
    shmem_atomic_fetch_nbi(ctx, &fetch, dest, fetch_pe);                       \
    shmem_ctx_quiet(ctx);                                                      \
    shmem_barrier_all();                                                       \
    success = (fetch == value + fetch_pe);                                     \
    if (!success)                                                              \
      log_fail("atomic fetch nbi with context on %s did not produce "          \
               "expected value %d, got instead %d",                            \
               #TYPE, (char)value + fetch_pe, (char)fetch);                    \
    else                                                                       \
      log_info("atomic fetch nbi with context on a %s at %p produced "         \
               "expected result",                                              \
               #TYPE, (void *)dest);                                           \
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

  /* Test standard atomic fetch nbi operations */
  static bool result = true;
  #define X(type, shmem_types) result &= TEST_C11_SHMEM_ATOMIC_FETCH_NBI(type);
    SHMEM_EXTENDED_AMO_TYPE_TABLE(X)
  #undef X

  reduce_test_result("C11 shmem_atomic_fetch_nbi", &result, false);

  /* Test context-specific atomic fetch nbi operations */
  static bool result_ctx = true;
  #define X(type, shmem_types) result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_FETCH_NBI(type);
    SHMEM_EXTENDED_AMO_TYPE_TABLE(X)
  #undef X

  reduce_test_result("C11 shmem_atomic_fetch_nbi with ctx", &result_ctx, false);

  bool rc = result & result_ctx ? EXIT_SUCCESS : EXIT_FAILURE;
  log_close(rc);
  shmem_finalize();
  return rc;
}
