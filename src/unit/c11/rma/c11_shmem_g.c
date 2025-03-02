/**
 * @file c11_shmem_g.c
 * @brief Unit test for the shmem_g() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C11_SHMEM_G(TYPE)                                                 \
  ({                                                                           \
    log_routine("shmem_g(" #TYPE ")");                                         \
    bool success = true;                                                       \
    static TYPE src, dest;                                                     \
    log_info("Allocated static variables: src at %p, dest at %p",              \
             (void *)&src, (void *)&dest);                                     \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    log_info("Running on PE %d of %d total PEs", mype, npes);                  \
                                                                               \
    if (mype == 0) {                                                           \
      src = 10;                                                                \
      log_info("PE 0: Initialized src = %d", (int)src);                        \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Fetching remote value from PE 0");                       \
      dest = shmem_g(&src, 0);                                                 \
      log_info("PE 1: Fetched value dest = %d", (int)dest);                    \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Beginning validation");                                  \
      if (dest != 10) {                                                        \
        log_fail("PE 1: Validation failed - expected dest=10, got dest=%d",    \
                 (int)dest);                                                   \
        success = false;                                                       \
      } else {                                                                 \
        log_info("PE 1: Validation successful - dest=10 as expected");         \
      }                                                                        \
    } else {                                                                   \
      log_info("PE 0: Waiting for PE 1 to complete validation");               \
    }                                                                          \
                                                                               \
    success;                                                                   \
  })

#define TEST_C11_CTX_SHMEM_G(TYPE)                                             \
  ({                                                                           \
    log_routine("shmem_g(ctx, " #TYPE ")");                                    \
    bool success = true;                                                       \
    static TYPE src, dest;                                                     \
    log_info("Allocated static variables: src at %p, dest at %p",              \
             (void *)&src, (void *)&dest);                                     \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    log_info("Running on PE %d of %d total PEs", mype, npes);                  \
                                                                               \
    /* Create a context for the operation */                                   \
    shmem_ctx_t ctx;                                                           \
    int ctx_create_status = shmem_ctx_create(0, &ctx);                         \
                                                                               \
    if (ctx_create_status != 0) {                                              \
      log_fail("Failed to create context");                                    \
      return false;                                                            \
    }                                                                          \
    log_info("Successfully created context");                                  \
                                                                               \
    if (mype == 0) {                                                           \
      src = 20;                                                                \
      log_info("PE 0: Initialized src = %d", (int)src);                        \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Fetching remote value from PE 0 using context");         \
      dest = shmem_g(ctx, &src, 0);                                            \
      log_info("PE 1: Fetched value dest = %d", (int)dest);                    \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Beginning validation");                                  \
      if (dest != 20) {                                                        \
        log_fail("PE 1: Validation failed - expected dest=20, got dest=%d",    \
                 (int)dest);                                                   \
        success = false;                                                       \
      } else {                                                                 \
        log_info("PE 1: Validation successful - dest=20 as expected");         \
      }                                                                        \
    } else {                                                                   \
      log_info("PE 0: Waiting for PE 1 to complete validation");               \
    }                                                                          \
                                                                               \
    /* Destroy the context */                                                  \
    shmem_ctx_destroy(ctx);                                                    \
    log_info("Context destroyed");                                             \
                                                                               \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  if (!(shmem_n_pes() <= 2)) {
    log_warn("Not enough PEs to run test (requires 2 PEs, have %d PEs)",
             shmem_n_pes());
    if (shmem_my_pe() == 0) {
      display_not_enough_pes("RMA");
    }
    shmem_finalize();
    return EXIT_SUCCESS;
  }

  int result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C11_SHMEM_G(long);
  result &= TEST_C11_SHMEM_G(double);
  result &= TEST_C11_SHMEM_G(long double);
  result &= TEST_C11_SHMEM_G(char);
  result &= TEST_C11_SHMEM_G(signed char);
  result &= TEST_C11_SHMEM_G(short);
  result &= TEST_C11_SHMEM_G(int);
  result &= TEST_C11_SHMEM_G(long);
  result &= TEST_C11_SHMEM_G(long long);
  result &= TEST_C11_SHMEM_G(unsigned char);
  result &= TEST_C11_SHMEM_G(unsigned short);
  result &= TEST_C11_SHMEM_G(unsigned int);
  result &= TEST_C11_SHMEM_G(unsigned long);
  result &= TEST_C11_SHMEM_G(unsigned long long);
  result &= TEST_C11_SHMEM_G(int8_t);
  result &= TEST_C11_SHMEM_G(int16_t);
  result &= TEST_C11_SHMEM_G(int32_t);
  result &= TEST_C11_SHMEM_G(int64_t);
  result &= TEST_C11_SHMEM_G(uint8_t);
  result &= TEST_C11_SHMEM_G(uint16_t);
  result &= TEST_C11_SHMEM_G(uint32_t);
  result &= TEST_C11_SHMEM_G(uint64_t);
  result &= TEST_C11_SHMEM_G(size_t);
  result &= TEST_C11_SHMEM_G(ptrdiff_t);

  shmem_barrier_all();

  if (!result) {
    rc = EXIT_FAILURE;
  }

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_g", result, false);
  }

  int result_ctx = true;

  /* Test context-specific shmem_g variants */
  result_ctx &= TEST_C11_CTX_SHMEM_G(long);
  result_ctx &= TEST_C11_CTX_SHMEM_G(double);
  result_ctx &= TEST_C11_CTX_SHMEM_G(long double);
  result_ctx &= TEST_C11_CTX_SHMEM_G(char);
  result_ctx &= TEST_C11_CTX_SHMEM_G(signed char);
  result_ctx &= TEST_C11_CTX_SHMEM_G(short);
  result_ctx &= TEST_C11_CTX_SHMEM_G(int);
  result_ctx &= TEST_C11_CTX_SHMEM_G(long);
  result_ctx &= TEST_C11_CTX_SHMEM_G(long long);
  result_ctx &= TEST_C11_CTX_SHMEM_G(unsigned char);
  result_ctx &= TEST_C11_CTX_SHMEM_G(unsigned short);
  result_ctx &= TEST_C11_CTX_SHMEM_G(unsigned int);
  result_ctx &= TEST_C11_CTX_SHMEM_G(unsigned long);
  result_ctx &= TEST_C11_CTX_SHMEM_G(unsigned long long);
  result_ctx &= TEST_C11_CTX_SHMEM_G(int8_t);
  result_ctx &= TEST_C11_CTX_SHMEM_G(int16_t);
  result_ctx &= TEST_C11_CTX_SHMEM_G(int32_t);
  result_ctx &= TEST_C11_CTX_SHMEM_G(int64_t);
  result_ctx &= TEST_C11_CTX_SHMEM_G(uint8_t);
  result_ctx &= TEST_C11_CTX_SHMEM_G(uint16_t);
  result_ctx &= TEST_C11_CTX_SHMEM_G(uint32_t);
  result_ctx &= TEST_C11_CTX_SHMEM_G(uint64_t);
  result_ctx &= TEST_C11_CTX_SHMEM_G(size_t);
  result_ctx &= TEST_C11_CTX_SHMEM_G(ptrdiff_t);

  shmem_barrier_all();

  if (!result_ctx) {
    rc = EXIT_FAILURE;
  }

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_g with ctx", result_ctx, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
