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
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch = 0;                                                     \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    TYPE value = 42;                                                           \
    *dest = value;                                                             \
    log_info("set %p to %d", (void *)dest, (int)value);                        \
    shmem_barrier_all();                                                       \
    log_info("executing atomic fetch: dest = %p", (void *)dest);               \
    int mype = shmem_my_pe();                                                  \
    fetch = shmem_atomic_fetch(dest, mype);                                    \
    shmem_barrier_all();                                                       \
    success = (fetch == value);                                                \
    if (!success)                                                              \
      log_fail("atomic fetch on %s did not produce expected value %d, "        \
               "got instead %d",                                               \
               #TYPE, (int)value, (int)fetch);                                 \
    else                                                                       \
      log_info(                                                                \
          "atomic fetch on a %s at %p produced expected result (%d == %d)",    \
          #TYPE, (void *)dest, (int)value, (int)fetch);                        \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

#define TEST_C11_CTX_SHMEM_ATOMIC_FETCH(TYPE)                                  \
  ({                                                                           \
    log_routine("shmem_atomic_fetch(ctx, " #TYPE ")");                         \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch = 0;                                                     \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    TYPE value = 59; /*Different value for ctx test*/                          \
    *dest = value;                                                             \
    log_info("set %p to %d", (void *)dest, (int)value);                        \
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
    log_info("executing atomic fetch with context: dest = %p", (void *)dest);  \
    int mype = shmem_my_pe();                                                  \
    fetch = shmem_atomic_fetch(ctx, dest, mype);                               \
    shmem_ctx_quiet(ctx);                                                      \
    shmem_barrier_all();                                                       \
    success = (fetch == value);                                                \
    if (!success)                                                              \
      log_fail("atomic fetch with context on %s did not produce expected "     \
               "value %d, got instead %d",                                     \
               #TYPE, (int)value, (int)fetch);                                 \
    else                                                                       \
      log_info(                                                                \
          "atomic fetch with context on a %s at %p produced expected result "  \
          "(%d == %d)",                                                        \
          #TYPE, (void *)dest, (int)value, (int)fetch);                        \
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

  /* Test standard atomic fetch operations */
  bool result = true;
  #define X(type, shmem_types) result &= TEST_C11_SHMEM_ATOMIC_FETCH(type);
    SHMEM_EXTENDED_AMO_TYPE_TABLE(X)
  #undef X

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_atomic_fetch", result, false);
  }

  /* Test context-specific atomic fetch operations */
  bool result_ctx = true;
  #define X(type, shmem_types) result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_FETCH(type);
    SHMEM_EXTENDED_AMO_TYPE_TABLE(X)
  #undef X

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_atomic_fetch with ctx", result_ctx, false);
  }

  if (!result || !result_ctx) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
