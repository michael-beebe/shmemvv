/**
 * @file c11_shmem_atomic_fetch.c
 * @brief Unit test for shmem_atomic_fetch
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"
#include "log.h"
#include "type_tables.h"

#define TEST_C11_SHMEM_ATOMIC_FETCH(TYPE)                                      \
  ({                                                                           \
    log_routine("shmem_atomic_fetch(" #TYPE ")");                              \
    int mype = shmem_my_pe();                                                  \
    int fetch_pe = (mype + 1) % shmem_n_pes();                                 \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch = 0;                                                     \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    TYPE value = 42 + mype; /*some arbitrary number*/                          \
    *dest = value;                                                             \
    TYPE expected = 42 + fetch_pe;                                             \
    log_info("set %p to %d", (void *)dest, (int)value);                        \
    shmem_barrier_all();                                                       \
    log_info("PE %d: executing atomic fetch from PE %d: dest = %p",mype,       \
      fetch_pe, (void *)dest);                                                 \
    fetch = shmem_atomic_fetch(dest, fetch_pe);                                \
    /*no barrier, routine should be blocking*/                                 \
    success = (fetch == expected);                                             \
    if (!success)                                                              \
      log_fail("atomic fetch on %s did not produce expected value %d, "        \
               "got instead %d",                                               \
               #TYPE, (int)expected, (int)fetch);                              \
    else                                                                       \
      log_info(                                                                \
          "atomic fetch on a %s at %p produced expected result (%d == %d)",    \
          #TYPE, (void *)dest, (int)expected, (int)fetch);                     \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

#define TEST_C11_CTX_SHMEM_ATOMIC_FETCH(TYPE)                                  \
  ({                                                                           \
    log_routine("shmem_atomic_fetch(ctx, " #TYPE ")");                         \
    shmem_ctx_t ctx;                                                           \
    int ctx_create_status = shmem_ctx_create(0, &ctx);                         \
    if (ctx_create_status != 0) {                                              \
      log_fail("Failed to create context");                                    \
      return false;                                                            \
    }                                                                          \
    log_info("Successfully created context");                                  \
                                                                               \
    bool success = true;                                                       \
    int mype = shmem_my_pe();                                                  \
    int fetch_pe = (mype + 1) % shmem_n_pes();                                 \
    static TYPE *dest;                                                         \
    static TYPE fetch = 0;                                                     \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    TYPE value = 52 + mype; /*some arbitrary number, different for ctx*/       \
    *dest = value;                                                             \
    TYPE expected = 52 + fetch_pe;                                             \
    log_info("set %p to %d", (void *)dest, (int)value);                        \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("PE %d: executing atomic fetch with context from PE %d:"          \
      " dest = %p", mype, fetch_pe, (void *)dest);                             \
    fetch = shmem_atomic_fetch(ctx, dest, fetch_pe);                           \
    /*no barrier, routine should be blocking*/                                 \
    success = (fetch == expected);                                             \
    if (!success)                                                              \
      log_fail("atomic fetch with context on %s did not produce expected "     \
               "value %d, got instead %d",                                     \
               #TYPE, (int)expected, (int)fetch);                              \
    else                                                                       \
      log_info(                                                                \
          "atomic fetch with context on a %s at %p produced expected result "  \
          "(%d == %d)",                                                        \
          #TYPE, (void *)dest, (int)expected, (int)fetch);                     \
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
      display_not_enough_pes("RMA");
    }
    shmem_finalize();
    return EXIT_SUCCESS;
  }

  static int result = true;
  static int result_ctx = true;

  /* Test standard atomic fetch operations */
  #define X(type, shmem_types) result &= TEST_C11_SHMEM_ATOMIC_FETCH(type);
    SHMEM_EXTENDED_AMO_TYPE_TABLE(X)
  #undef X

  shmem_barrier_all();

  reduce_test_result("C11 shmem_atomic_fetch", &result, false);

  /* Test context-specific atomic fetch operations */
  #define X(type, shmem_types) result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_FETCH(type);
    SHMEM_EXTENDED_AMO_TYPE_TABLE(X)
  #undef X

  reduce_test_result("C11 shmem_atomic_fetch with ctx", &result_ctx, false);

  shmem_barrier_all();

  bool rc = result & result_ctx ? EXIT_SUCCESS : EXIT_FAILURE;
  log_close(rc);
  shmem_finalize();
  return rc;
}
