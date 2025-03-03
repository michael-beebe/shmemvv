/**
 * @file c11_shmem_atomic_xor.c
 * @brief Unit test for shmem_atomic_xor
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"
#include "log.h"

#define TEST_C11_SHMEM_ATOMIC_XOR(TYPE)                                        \
  ({                                                                           \
    log_routine("shmem_atomic_xor(" #TYPE ")");                                \
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
    log_info("executing atomic xor: dest = %p, value = %d", (void *)dest,      \
             (int)xor_value);                                                  \
    shmem_atomic_xor(dest, xor_value, (mype + 1) % npes);                      \
    shmem_barrier_all();                                                       \
    success = (*dest == (value ^ xor_value));                                  \
    if (!success)                                                              \
      log_fail("atomic xor on %s did not produce expected value %d, "          \
               "got instead %d",                                               \
               #TYPE, (int)(value ^ xor_value), (int)*dest);                   \
    else                                                                       \
      log_info("atomic xor on a %s at %p produced expected result "            \
               "(%d ^ %d = %d)",                                               \
               #TYPE, (void *)dest, (int)value, (int)xor_value, (int)*dest);   \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

#define TEST_C11_CTX_SHMEM_ATOMIC_XOR(TYPE)                                    \
  ({                                                                           \
    log_routine("shmem_atomic_xor(ctx, " #TYPE ")");                           \
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
    log_info("executing atomic xor with context: dest = %p, value = %d",       \
             (void *)dest, (int)xor_value);                                    \
    shmem_atomic_xor(ctx, dest, xor_value, (mype + 1) % npes);                 \
    shmem_ctx_quiet(ctx);                                                      \
    shmem_barrier_all();                                                       \
    success = (*dest == (value ^ xor_value));                                  \
    if (!success)                                                              \
      log_fail("atomic xor with context on %s did not produce expected "       \
               "value %d, got instead %d",                                     \
               #TYPE, (int)(value ^ xor_value), (int)*dest);                   \
    else                                                                       \
      log_info("atomic xor with context on a %s at %p produced expected "      \
               "result (%d ^ %d = %d)",                                        \
               #TYPE, (void *)dest, (int)value, (int)xor_value, (int)*dest);   \
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

  /* Test standard atomic xor operations */
  bool result = true;
  result &= TEST_C11_SHMEM_ATOMIC_XOR(unsigned int);
  result &= TEST_C11_SHMEM_ATOMIC_XOR(unsigned long);
  result &= TEST_C11_SHMEM_ATOMIC_XOR(unsigned long long);
  result &= TEST_C11_SHMEM_ATOMIC_XOR(int32_t);
  result &= TEST_C11_SHMEM_ATOMIC_XOR(int64_t);
  result &= TEST_C11_SHMEM_ATOMIC_XOR(uint32_t);
  result &= TEST_C11_SHMEM_ATOMIC_XOR(uint64_t);

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_atomic_xor", result, false);
  }

  /* Test context-specific atomic xor operations */
  bool result_ctx = true;
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_XOR(unsigned int);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_XOR(unsigned long);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_XOR(unsigned long long);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_XOR(int32_t);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_XOR(int64_t);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_XOR(uint32_t);
  result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_XOR(uint64_t);

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_atomic_xor with ctx", result_ctx, false);
  }

  if (!result || !result_ctx) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
