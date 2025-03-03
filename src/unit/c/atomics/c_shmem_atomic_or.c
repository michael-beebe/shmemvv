/**
 * @file c_shmem_atomic_or.c
 * @brief Unit test for shmem_atomic_or
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C_SHMEM_ATOMIC_OR(TYPE, TYPENAME)                                 \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_atomic_or");                              \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    TYPE value = 42;                                                           \
    TYPE or_value = 15;                                                        \
    *dest = value;                                                             \
    log_info("initialized dest at %p to %d", (void *)dest, (int)value);        \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    log_info("executing atomic or: dest = %p, value = %d", (void *)dest,       \
             (int)or_value);                                                   \
    shmem_##TYPENAME##_atomic_or(dest, or_value, (mype + 1) % npes);           \
    shmem_barrier_all();                                                       \
    success = (*dest == (value | or_value));                                   \
    if (!success)                                                              \
      log_fail("atomic or on %s did not produce expected value %d, "           \
               "got instead %d",                                               \
               #TYPE, (int)(value | or_value), (int)*dest);                    \
    else                                                                       \
      log_info("atomic or on a %s at %p produced expected result "             \
               "(%d | %d = %d)",                                               \
               #TYPE, (void *)dest, (int)value, (int)or_value, (int)*dest);    \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

#define TEST_C_CTX_SHMEM_ATOMIC_OR(TYPE, TYPENAME)                             \
  ({                                                                           \
    log_routine("shmem_ctx_" #TYPENAME "_atomic_or");                          \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    TYPE value = 42;                                                           \
    TYPE or_value = 15;                                                        \
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
    log_info("executing atomic or with context: dest = %p, value = %d",        \
             (void *)dest, (int)or_value);                                     \
    shmem_ctx_##TYPENAME##_atomic_or(ctx, dest, or_value, (mype + 1) % npes);  \
    shmem_ctx_quiet(ctx);                                                      \
    shmem_barrier_all();                                                       \
    success = (*dest == (value | or_value));                                   \
    if (!success)                                                              \
      log_fail("atomic or with context on %s did not produce expected "        \
               "value %d, got instead %d",                                     \
               #TYPE, (int)(value | or_value), (int)*dest);                    \
    else                                                                       \
      log_info("atomic or with context on a %s at %p produced expected "       \
               "result (%d | %d = %d)",                                        \
               #TYPE, (void *)dest, (int)value, (int)or_value, (int)*dest);    \
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

  /* Test standard atomic or operations */
  bool result = true;
  result &= TEST_C_SHMEM_ATOMIC_OR(unsigned int, uint);
  result &= TEST_C_SHMEM_ATOMIC_OR(unsigned long, ulong);
  result &= TEST_C_SHMEM_ATOMIC_OR(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_ATOMIC_OR(int32_t, int32);
  result &= TEST_C_SHMEM_ATOMIC_OR(int64_t, int64);
  result &= TEST_C_SHMEM_ATOMIC_OR(uint32_t, uint32);
  result &= TEST_C_SHMEM_ATOMIC_OR(uint64_t, uint64);

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_atomic_or", result, false);
  }

  /* Test context-specific atomic or operations */
  bool result_ctx = true;
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_OR(unsigned int, uint);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_OR(unsigned long, ulong);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_OR(unsigned long long, ulonglong);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_OR(int32_t, int32);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_OR(int64_t, int64);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_OR(uint32_t, uint32);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_OR(uint64_t, uint64);

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_ctx_atomic_or", result_ctx, false);
  }

  if (!result || !result_ctx) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
