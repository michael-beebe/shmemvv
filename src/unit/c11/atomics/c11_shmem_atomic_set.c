/**
 * @file c11_shmem_atomic_set.c
 * @brief Unit test for shmem_atomic_set
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C11_SHMEM_ATOMIC_SET(TYPE)                                        \
  ({                                                                           \
    log_routine("shmem_atomic_set(" #TYPE ")");                                \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    TYPE value = 42;                                                           \
    *dest = 0;                                                                 \
    log_info("initialized dest at %p to %d", (void *)dest, (int)*dest);        \
    shmem_barrier_all();                                                       \
    log_info("executing atomic set: dest = %p, value = %d", (void *)dest,      \
             (int)value);                                                      \
    int mype = shmem_my_pe();                                                  \
    shmem_atomic_set(dest, value, mype);                                       \
    shmem_barrier_all();                                                       \
    success = (*dest == value);                                                \
    if (!success)                                                              \
      log_fail("atomic set on %s did not produce expected value %d, "          \
               "got instead %d",                                               \
               #TYPE, (int)value, (int)*dest);                                 \
    else                                                                       \
      log_info("atomic set on a %s at %p produced expected result (%d == %d)", \
               #TYPE, (void *)dest, (int)value, (int)*dest);                   \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

#define TEST_C11_CTX_SHMEM_ATOMIC_SET(TYPE)                                    \
  ({                                                                           \
    log_routine("shmem_atomic_set(ctx, " #TYPE ")");                           \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    TYPE value = 42;                                                           \
    *dest = 0;                                                                 \
    log_info("initialized dest at %p to %d", (void *)dest, (int)*dest);        \
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
    log_info("executing atomic set with context: dest = %p, value = %d",       \
             (void *)dest, (int)value);                                        \
    shmem_atomic_set(ctx, dest, value, (mype + 1) % npes);                     \
    shmem_ctx_quiet(ctx);                                                      \
    shmem_barrier_all();                                                       \
    success = (*dest == value);                                                \
    if (!success)                                                              \
      log_fail("atomic set with context on %s did not produce expected "       \
               "value %d, got instead %d",                                     \
               #TYPE, (int)value, (int)*dest);                                 \
    else                                                                       \
      log_info("atomic set with context on a %s at %p produced expected "      \
               "result %d",                                                    \
               #TYPE, (void *)dest, (int)*dest);                               \
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

  /* Test standard atomic set operations */
  bool result = true;
  result &= TEST_C11_SHMEM_ATOMIC_SET(int);
  result &= TEST_C11_SHMEM_ATOMIC_SET(long);
  result &= TEST_C11_SHMEM_ATOMIC_SET(long long);
  result &= TEST_C11_SHMEM_ATOMIC_SET(unsigned int);
  result &= TEST_C11_SHMEM_ATOMIC_SET(unsigned long);
  result &= TEST_C11_SHMEM_ATOMIC_SET(unsigned long long);
  result &= TEST_C11_SHMEM_ATOMIC_SET(int32_t);
  result &= TEST_C11_SHMEM_ATOMIC_SET(int64_t);
  result &= TEST_C11_SHMEM_ATOMIC_SET(uint32_t);
  result &= TEST_C11_SHMEM_ATOMIC_SET(uint64_t);
  result &= TEST_C11_SHMEM_ATOMIC_SET(size_t);
  result &= TEST_C11_SHMEM_ATOMIC_SET(ptrdiff_t);

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_atomic_set", result, false);
  }

  /* Test context-specific atomic set operations */
  bool result_ctx = true;
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_SET(int);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_SET(long);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_SET(long long);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_SET(unsigned int);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_SET(unsigned long);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_SET(unsigned long long);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_SET(int32_t);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_SET(int64_t);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_SET(uint32_t);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_SET(uint64_t);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_SET(size_t);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_SET(ptrdiff_t);

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_atomic_set with ctx", result_ctx, false);
  }

  if (!result || !result_ctx) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
