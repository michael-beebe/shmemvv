/**
 * @file c11_shmem_atomic_fetch_and_nbi.c
 * @brief Unit test for shmem_atomic_fetch_and_nbi
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"
#include "log.h"

#define TEST_C11_SHMEM_ATOMIC_FETCH_AND_NBI(TYPE)                              \
  ({                                                                           \
    log_routine("shmem_atomic_fetch_and_nbi(" #TYPE ")");                      \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    fetch = 0;                                                                 \
    TYPE value = 42, and_val = 15;                                             \
    *dest = value;                                                             \
    log_info("set %p to %d", (void *)dest, (char)value);                       \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    log_info("executing atomic fetch and (nbi): dest = %p, and_val = %d",      \
             (void *)dest, (char)and_val);                                     \
    shmem_atomic_fetch_and_nbi(&fetch, dest, and_val, mype);                   \
    shmem_quiet();                                                             \
    shmem_barrier_all();                                                       \
    success = (fetch == value && *dest == (value & and_val));                  \
    if (!success)                                                              \
      log_fail("atomic fetch and (nbi) on %s did not produce expected value "  \
               "= %d, ret = "                                                  \
               "%d, got "                                                      \
               "instead value = %d, ret = %d",                                 \
               #TYPE, (char)(value & and_val), (char)value, (char)*dest,       \
               (char)fetch);                                                   \
    else                                                                       \
      log_info("atomic fetch and on a %s at %p produced expected result",      \
               #TYPE, (void *)dest);                                           \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

#define TEST_C11_CTX_SHMEM_ATOMIC_FETCH_AND_NBI(TYPE)                          \
  ({                                                                           \
    log_routine("shmem_atomic_fetch_and_nbi(ctx, " #TYPE ")");                 \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    fetch = 0;                                                                 \
    TYPE value = 42, and_val = 15;                                             \
    *dest = value;                                                             \
    log_info("set %p to %d", (void *)dest, (char)value);                       \
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
    log_info("executing atomic fetch and nbi with context: dest = %p, "        \
             "and_val = %d",                                                   \
             (void *)dest, (char)and_val);                                     \
    int mype = shmem_my_pe();                                                  \
    shmem_atomic_fetch_and_nbi(ctx, &fetch, dest, and_val, mype);              \
    shmem_ctx_quiet(ctx);                                                      \
    shmem_barrier_all();                                                       \
    success = (fetch == value && *dest == (value & and_val));                  \
    if (!success)                                                              \
      log_fail("atomic fetch and nbi with context on %s did not produce "      \
               "expected value = %d, ret = %d, got instead value = %d, "       \
               "ret = %d",                                                     \
               #TYPE, (char)(value & and_val), (char)value, (char)*dest,       \
               (char)fetch);                                                   \
    else                                                                       \
      log_info("atomic fetch and nbi with context on a %s at %p produced "     \
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

  int rc = EXIT_SUCCESS;

  /* Test standard atomic fetch-and nbi operations */
  bool result = true;
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_AND_NBI(unsigned int);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_AND_NBI(unsigned long);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_AND_NBI(unsigned long long);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_AND_NBI(int32_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_AND_NBI(int64_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_AND_NBI(uint32_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_AND_NBI(uint64_t);

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_atomic_fetch_and_nbi", result, false);
  }

  /* Test context-specific atomic fetch-and nbi operations */
  bool result_ctx = true;
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_FETCH_AND_NBI(unsigned int);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_FETCH_AND_NBI(unsigned long);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_FETCH_AND_NBI(unsigned long long);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_FETCH_AND_NBI(int32_t);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_FETCH_AND_NBI(int64_t);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_FETCH_AND_NBI(uint32_t);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_FETCH_AND_NBI(uint64_t);

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_atomic_fetch_and_nbi with ctx", result_ctx,
                        false);
  }

  if (!result || !result_ctx) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
