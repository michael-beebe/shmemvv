/**
 * @file c_shmem_atomic_fetch_xor.c
 * @brief Test for shmem_atomic_fetch_xor()
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C_SHMEM_ATOMIC_FETCH_XOR(TYPE, TYPENAME)                          \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_atomic_fetch_xor");                       \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    TYPE value = 42;                                                           \
    TYPE xor_value = 15;                                                       \
    *dest = value;                                                             \
    log_info("initialized dest at %p to %d", (void *)dest, (int)value);        \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    log_info("executing atomic fetch-xor: dest = %p, value = %d",              \
             (void *)dest, (int)xor_value);                                    \
    TYPE fetched = shmem_##TYPENAME##_atomic_fetch_xor(dest, xor_value,        \
                                                       (mype + 1) % npes);     \
    shmem_barrier_all();                                                       \
    success = (fetched == value && *dest == (value ^ xor_value));              \
    if (!success)                                                              \
      log_fail("atomic fetch-xor on %s did not produce expected values: "      \
               "fetched = %d (expected %d), dest = %d (expected %d)",          \
               #TYPE, (int)fetched, (int)value, (int)*dest,                    \
               (int)(value ^ xor_value));                                      \
    else                                                                       \
      log_info("atomic fetch-xor on a %s at %p produced expected result "      \
               "(fetched = %d, dest = %d ^ %d = %d)",                          \
               #TYPE, (void *)dest, (int)fetched, (int)value, (int)xor_value,  \
               (int)*dest);                                                    \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

#define TEST_C_CTX_SHMEM_ATOMIC_FETCH_XOR(TYPE, TYPENAME)                      \
  ({                                                                           \
    log_routine("shmem_ctx_" #TYPENAME "_atomic_fetch_xor");                   \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    TYPE value = 42;                                                           \
    TYPE xor_value = 15;                                                       \
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
    log_info("executing atomic fetch-xor with context: dest = %p, value = %d", \
             (void *)dest, (int)xor_value);                                    \
    TYPE fetched = shmem_ctx_##TYPENAME##_atomic_fetch_xor(                    \
        ctx, dest, xor_value, (mype + 1) % npes);                              \
    shmem_ctx_quiet(ctx);                                                      \
    shmem_barrier_all();                                                       \
    success = (fetched == value && *dest == (value ^ xor_value));              \
    if (!success)                                                              \
      log_fail("atomic fetch-xor with context on %s did not produce "          \
               "expected values: fetched = %d (expected %d), dest = %d "       \
               "(expected %d)",                                                \
               #TYPE, (int)fetched, (int)value, (int)*dest,                    \
               (int)(value ^ xor_value));                                      \
    else                                                                       \
      log_info("atomic fetch-xor with context on a %s at %p produced "         \
               "expected result (fetched = %d, dest = %d ^ %d = %d)",          \
               #TYPE, (void *)dest, (int)fetched, (int)value, (int)xor_value,  \
               (int)*dest);                                                    \
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

  /* Test standard atomic fetch-xor operations */
  bool result = true;
  result &= TEST_C_SHMEM_ATOMIC_FETCH_XOR(unsigned int, uint);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_XOR(unsigned long, ulong);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_XOR(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_XOR(int32_t, int32);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_XOR(int64_t, int64);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_XOR(uint32_t, uint32);
  result &= TEST_C_SHMEM_ATOMIC_FETCH_XOR(uint64_t, uint64);

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_atomic_fetch_xor", result, false);
  }

  /* Test context-specific atomic fetch-xor operations */
  bool result_ctx = true;
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_FETCH_XOR(unsigned int, uint);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_FETCH_XOR(unsigned long, ulong);
  result_ctx &=
      TEST_C_CTX_SHMEM_ATOMIC_FETCH_XOR(unsigned long long, ulonglong);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_FETCH_XOR(int32_t, int32);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_FETCH_XOR(int64_t, int64);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_FETCH_XOR(uint32_t, uint32);
  result_ctx &= TEST_C_CTX_SHMEM_ATOMIC_FETCH_XOR(uint64_t, uint64);

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_ctx_atomic_fetch_xor", result_ctx, false);
  }

  if (!result || !result_ctx) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
