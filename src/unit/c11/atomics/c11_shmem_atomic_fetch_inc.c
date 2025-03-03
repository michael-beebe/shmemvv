/**
 * @file c11_shmem_atomic_fetch_inc.c
 * @brief Unit test for shmem_atomic_fetch_inc
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C11_SHMEM_ATOMIC_FETCH_INC(TYPE)                                  \
  ({                                                                           \
    log_routine("shmem_atomic_fetch_inc(" #TYPE ")");                          \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    TYPE value = 42;                                                           \
    *dest = value;                                                             \
    log_info("initialized dest at %p to %d", (void *)dest, (int)value);        \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    log_info("executing atomic fetch inc: dest = %p", (void *)dest);           \
    TYPE fetch = shmem_atomic_fetch_inc(dest, (mype + 1) % npes);              \
    shmem_barrier_all();                                                       \
    success = (fetch == value && *dest == value + 1);                          \
    if (!success)                                                              \
      log_fail("atomic fetch inc on %s did not produce expected values: "      \
               "fetch = %d (expected %d), dest = %d (expected %d)",            \
               #TYPE, (int)fetch, (int)value, (int)*dest, (int)(value + 1));   \
    else                                                                       \
      log_info("atomic fetch inc on a %s at %p produced expected result "      \
               "(fetch = %d, dest = %d)",                                      \
               #TYPE, (void *)dest, (int)fetch, (int)*dest);                   \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

#define TEST_C11_CTX_SHMEM_ATOMIC_FETCH_INC(TYPE)                              \
  ({                                                                           \
    log_routine("shmem_atomic_fetch_inc(ctx, " #TYPE ")");                     \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    TYPE value = 42;                                                           \
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
    log_info("executing atomic fetch inc with context: dest = %p",             \
             (void *)dest);                                                    \
    TYPE fetch = shmem_atomic_fetch_inc(ctx, dest, (mype + 1) % npes);         \
    shmem_ctx_quiet(ctx);                                                      \
    shmem_barrier_all();                                                       \
    success = (fetch == value && *dest == value + 1);                          \
    if (!success)                                                              \
      log_fail("atomic fetch inc with context on %s did not produce expected " \
               "values: fetch = %d (expected %d), dest = %d (expected %d)",    \
               #TYPE, (int)fetch, (int)value, (int)*dest, (int)(value + 1));   \
    else                                                                       \
      log_info(                                                                \
          "atomic fetch inc with context on a %s at %p produced expected "     \
          "result (fetch = %d, dest = %d)",                                    \
          #TYPE, (void *)dest, (int)fetch, (int)*dest);                        \
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

  /* Test standard atomic fetch inc operations */
  bool result = true;
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_INC(int);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_INC(long);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_INC(long long);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_INC(unsigned int);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_INC(unsigned long);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_INC(unsigned long long);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_INC(int32_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_INC(int64_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_INC(uint32_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_INC(uint64_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_INC(size_t);
  result &= TEST_C11_SHMEM_ATOMIC_FETCH_INC(ptrdiff_t);

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_atomic_fetch_inc", result, false);
  }

  /* Test context-specific atomic fetch inc operations */
  bool result_ctx = true;
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_FETCH_INC(int);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_FETCH_INC(long);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_FETCH_INC(long long);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_FETCH_INC(unsigned int);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_FETCH_INC(unsigned long);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_FETCH_INC(unsigned long long);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_FETCH_INC(int32_t);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_FETCH_INC(int64_t);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_FETCH_INC(uint32_t);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_FETCH_INC(uint64_t);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_FETCH_INC(size_t);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_FETCH_INC(ptrdiff_t);

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_atomic_fetch_inc with ctx", result_ctx,
                        false);
  }

  if (!result || !result_ctx) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
