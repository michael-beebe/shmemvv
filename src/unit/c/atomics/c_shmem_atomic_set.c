/**
 * @file c_shmem_atomic_set.c
 * @brief Unit test for shmem_atomic_set
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C_SHMEM_ATOMIC_SET(TYPE, TYPENAME)                                \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_atomic_set");                             \
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
    shmem_##TYPENAME##_atomic_set(dest, value, mype);                          \
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

#define TEST_C_CTX_SHMEM_ATOMIC_SET(TYPE, TYPENAME)                            \
  ({                                                                           \
    log_routine("shmem_ctx_" #TYPENAME "_atomic_set");                         \
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
    log_info("executing atomic set with context: dest = %p, value = %d",       \
             (void *)dest, (int)value);                                        \
    int mype = shmem_my_pe();                                                  \
    shmem_ctx_##TYPENAME##_atomic_set(ctx, dest, value, mype);                 \
    shmem_ctx_quiet(ctx);                                                      \
    shmem_barrier_all();                                                       \
    success = (*dest == value);                                                \
    if (!success)                                                              \
      log_fail("atomic set with context on %s did not produce expected "       \
               "value %d, got instead %d",                                     \
               #TYPE, (int)value, (int)*dest);                                 \
    else                                                                       \
      log_info(                                                                \
          "atomic set with context on a %s at %p produced expected result "    \
          "(%d == %d)",                                                        \
          #TYPE, (void *)dest, (int)value, (int)*dest);                        \
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
  result &= TEST_C_SHMEM_ATOMIC_SET(int, int);
  result &= TEST_C_SHMEM_ATOMIC_SET(long, long);
  result &= TEST_C_SHMEM_ATOMIC_SET(long long, longlong);
  result &= TEST_C_SHMEM_ATOMIC_SET(unsigned int, uint);
  result &= TEST_C_SHMEM_ATOMIC_SET(unsigned long, ulong);
  result &= TEST_C_SHMEM_ATOMIC_SET(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_ATOMIC_SET(int32_t, int32);
  result &= TEST_C_SHMEM_ATOMIC_SET(int64_t, int64);
  result &= TEST_C_SHMEM_ATOMIC_SET(uint32_t, uint32);
  result &= TEST_C_SHMEM_ATOMIC_SET(uint64_t, uint64);
  result &= TEST_C_SHMEM_ATOMIC_SET(size_t, size);
  result &= TEST_C_SHMEM_ATOMIC_SET(ptrdiff_t, ptrdiff);

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_atomic_set", result, false);
  }

  /* Test context-specific atomic set operations */
  bool result_ctx = true;
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_SET(int, int);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_SET(long, long);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_SET(long long, longlong);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_SET(unsigned int, uint);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_SET(unsigned long, ulong);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_SET(unsigned long long, ulonglong);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_SET(int32_t, int32);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_SET(int64_t, int64);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_SET(uint32_t, uint32);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_SET(uint64_t, uint64);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_SET(size_t, size);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_SET(ptrdiff_t, ptrdiff);

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_ctx_atomic_set", result_ctx, false);
  }

  if (!result || !result_ctx) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
