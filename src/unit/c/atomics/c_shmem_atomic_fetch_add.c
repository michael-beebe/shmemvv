/**
 * @file c_shmem_atomic_fetch_add.c
 * @brief Unit test for shmem_atomic_fetch_add
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C_SHMEM_ATOMIC_FETCH_ADD(TYPE, TYPENAME)                          \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_atomic_fetch_add");                       \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    TYPE value = 42;                                                           \
    TYPE add_value = 5;                                                        \
    *dest = value;                                                             \
    log_info("initialized dest at %p to %d", (void *)dest, (int)value);        \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    log_info("executing atomic fetch-add: dest = %p, value = %d",              \
             (void *)dest, (int)add_value);                                    \
    TYPE fetched = shmem_##TYPENAME##_atomic_fetch_add(dest, add_value,        \
                                                       (mype + 1) % npes);     \
    shmem_barrier_all();                                                       \
    success = (fetched == value && *dest == value + add_value);                \
    if (!success)                                                              \
      log_fail("atomic fetch-add on %s did not produce expected values: "      \
               "fetched = %d (expected %d), dest = %d (expected %d)",          \
               #TYPE, (int)fetched, (int)value, (int)*dest,                    \
               (int)(value + add_value));                                      \
    else                                                                       \
      log_info("atomic fetch-add on a %s at %p produced expected result "      \
               "(fetched = %d, dest = %d)",                                    \
               #TYPE, (void *)dest, (int)fetched, (int)*dest);                 \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

#define TEST_C_CTX_SHMEM_ATOMIC_FETCH_ADD(TYPE, TYPENAME)                      \
  ({                                                                           \
    log_routine("shmem_ctx_" #TYPENAME "_atomic_fetch_add");                   \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    TYPE value = 42;                                                           \
    TYPE add_value = 5;                                                        \
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
    log_info("executing atomic fetch-add with context: dest = %p, value = %d", \
             (void *)dest, (int)add_value);                                    \
    TYPE fetched = shmem_ctx_##TYPENAME##_atomic_fetch_add(                    \
        ctx, dest, add_value, (mype + 1) % npes);                              \
    shmem_ctx_quiet(ctx);                                                      \
    shmem_barrier_all();                                                       \
    success = (fetched == value && *dest == value + add_value);                \
    if (!success)                                                              \
      log_fail("atomic fetch-add with context on %s did not produce "          \
               "expected values: fetched = %d (expected %d), dest = %d "       \
               "(expected %d)",                                                \
               #TYPE, (int)fetched, (int)value, (int)*dest,                    \
               (int)(value + add_value));                                      \
    else                                                                       \
      log_info("atomic fetch-add with context on a %s at %p produced "         \
               "expected result (fetched = %d, dest = %d)",                    \
               #TYPE, (void *)dest, (int)fetched, (int)*dest);                 \
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

  /* Test standard atomic fetch-add operations */
  bool result = true;
  result &= TEST_C_SHMEM_ATOMIC_FETCH_ADD(int, int);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_ADD(long, long);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_ADD(long long, longlong);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_ADD(unsigned int, uint);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_ADD(unsigned long, ulong);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_ADD(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_ADD(int32_t, int32);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_ADD(int64_t, int64);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_ADD(uint32_t, uint32);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_ADD(uint64_t, uint64);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_ADD(size_t, size);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_ADD(ptrdiff_t, ptrdiff);

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_atomic_fetch_add", result, false);
  }

  /* Test context-specific atomic fetch-add operations */
  bool result_ctx = true;
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_FETCH_ADD(int, int);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_FETCH_ADD(long, long);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_FETCH_ADD(long long, longlong);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_FETCH_ADD(unsigned int, uint);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_FETCH_ADD(unsigned long, ulong);
  result_ctx &=
      TEST_C_CTX_SHMEM_ATOMIC_FETCH_ADD(unsigned long long, ulonglong);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_FETCH_ADD(int32_t, int32);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_FETCH_ADD(int64_t, int64);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_FETCH_ADD(uint32_t, uint32);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_FETCH_ADD(uint64_t, uint64);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_FETCH_ADD(size_t, size);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_FETCH_ADD(ptrdiff_t, ptrdiff);

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_ctx_atomic_fetch_add", result_ctx, false);
  }

  if (!result || !result_ctx) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
