/**
 * @file c_shmem_atomic_fetch_inc_nbi.c
 * @brief Unit test for shmem_atomic_fetch_inc_nbi
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C_SHMEM_ATOMIC_FETCH_INC_NBI(TYPE, TYPENAME)                      \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_atomic_fetch_inc_nbi");                   \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    fetch = 0;                                                                 \
    TYPE value = 42;                                                           \
    *dest = value;                                                             \
    log_info("set %p to %d", (void *)dest, (char)value);                       \
    shmem_barrier_all();                                                       \
    log_info("executing atomic fetch inc nbi: dest = %p", (void *)dest);       \
    int mype = shmem_my_pe();                                                  \
    shmem_##TYPENAME##_atomic_fetch_inc_nbi(&fetch, dest, mype);               \
    shmem_quiet();                                                             \
    shmem_barrier_all();                                                       \
    success = (fetch == value && *dest == value + 1);                          \
    if (!success)                                                              \
      log_fail(                                                                \
          "atomic fetch inc nbi on %s did not produce expected value %d, "     \
          "got instead %d",                                                    \
          #TYPE, (char)(value + 1), (char)*dest);                              \
    else                                                                       \
      log_info("atomic fetch inc nbi on a %s at %p produced expected result "  \
               "(%d == %d && %d == %d)",                                       \
               #TYPE, dest, value, fetch, value + 1);                          \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

#define TEST_C_CTX_SHMEM_ATOMIC_FETCH_INC_NBI(TYPE, TYPENAME)                  \
  ({                                                                           \
    log_routine("shmem_ctx_" #TYPENAME "_atomic_fetch_inc_nbi");               \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    static TYPE fetch;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    fetch = 0;                                                                 \
    TYPE value = 42;                                                           \
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
    log_info("executing atomic fetch inc nbi with context: dest = %p",         \
             (void *)dest);                                                    \
    int mype = shmem_my_pe();                                                  \
    shmem_ctx_##TYPENAME##_atomic_fetch_inc_nbi(ctx, &fetch, dest, mype);      \
    shmem_ctx_quiet(ctx);                                                      \
    shmem_barrier_all();                                                       \
    success = (fetch == value && *dest == value + 1);                          \
    if (!success)                                                              \
      log_fail("atomic fetch inc nbi with context on %s did not produce "      \
               "expected value %d, got instead %d",                            \
               #TYPE, (char)(value + 1), (char)*dest);                         \
    else                                                                       \
      log_info("atomic fetch inc nbi with context on a %s at %p produced "     \
               "expected result (%d == %d && %d == %d)",                       \
               #TYPE, dest, value, fetch, value + 1, *dest);                   \
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

  /* Test standard atomic fetch-inc nbi operations */
  bool result = true;
  result &= TEST_C_SHMEM_ATOMIC_FETCH_INC_NBI(int, int);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_INC_NBI(long, long);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_INC_NBI(long long, longlong);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_INC_NBI(unsigned int, uint);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_INC_NBI(unsigned long, ulong);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_INC_NBI(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_INC_NBI(int32_t, int32);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_INC_NBI(int64_t, int64);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_INC_NBI(uint32_t, uint32);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_INC_NBI(uint64_t, uint64);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_INC_NBI(size_t, size);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_INC_NBI(ptrdiff_t, ptrdiff);

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_atomic_fetch_inc_nbi", result, false);
  }

  /* Test context-specific atomic fetch-inc nbi operations */
  bool result_ctx = true;
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_FETCH_INC_NBI(int, int);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_FETCH_INC_NBI(long, long);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_FETCH_INC_NBI(long long, longlong);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_FETCH_INC_NBI(unsigned int, uint);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_FETCH_INC_NBI(unsigned long, ulong);
  result_ctx &=
      TEST_C_CTX_SHMEM_ATOMIC_FETCH_INC_NBI(unsigned long long, ulonglong);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_FETCH_INC_NBI(int32_t, int32);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_FETCH_INC_NBI(int64_t, int64);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_FETCH_INC_NBI(uint32_t, uint32);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_FETCH_INC_NBI(uint64_t, uint64);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_FETCH_INC_NBI(size_t, size);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_FETCH_INC_NBI(ptrdiff_t, ptrdiff);

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_ctx_atomic_fetch_inc_nbi", result_ctx, false);
  }

  if (!result || !result_ctx) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
