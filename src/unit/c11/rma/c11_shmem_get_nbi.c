/**
 * @file c11_shmem_get_nbi.c
 * @brief Unit test for the shmem_get_nbi() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C11_SHMEM_GET_NBI(TYPE)                                           \
  ({                                                                           \
    log_routine("shmem_get_nbi(" #TYPE ")");                                   \
    bool success = true;                                                       \
    static TYPE src[10], dest[10];                                             \
    log_info("Allocated static arrays: src at %p, dest at %p", (void *)&src,   \
             (void *)&dest);                                                   \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    log_info("Running on PE %d of %d total PEs", mype, npes);                  \
                                                                               \
    if (mype == 0) {                                                           \
      for (int i = 0; i < 10; i++) {                                           \
        src[i] = i;                                                            \
      }                                                                        \
      log_info("PE 0: Initialized src array with values [0..9]");              \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Starting non-blocking get of 10 elements from PE 0");    \
      shmem_get_nbi(dest, src, 10, 0);                                         \
      log_info("PE 1: Calling quiet to ensure completion");                    \
      shmem_quiet();                                                           \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Beginning validation of received data");                 \
      for (int i = 0; i < 10; i++) {                                           \
        if (dest[i] != i) {                                                    \
          log_fail("PE 1: Validation failed - dest[%d] = %d, expected %d", i,  \
                   (int)dest[i], i);                                           \
          success = false;                                                     \
          break;                                                               \
        }                                                                      \
      }                                                                        \
      if (success) {                                                           \
        log_info("PE 1: Validation successful - all elements match expected "  \
                 "values");                                                    \
      }                                                                        \
    } else {                                                                   \
      log_info("PE 0: Waiting for PE 1 to complete validation");               \
    }                                                                          \
                                                                               \
    success;                                                                   \
  })

#define TEST_C11_CTX_SHMEM_GET_NBI(TYPE)                                       \
  ({                                                                           \
    log_routine("shmem_get_nbi(ctx, " #TYPE ")");                              \
    bool success = true;                                                       \
    static TYPE src[10], dest[10];                                             \
    log_info("Allocated static arrays: src at %p, dest at %p", (void *)&src,   \
             (void *)&dest);                                                   \
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
      for (int i = 0; i < 10; i++) {                                           \
        src[i] = i + 20; /* Different values for context test */               \
      }                                                                        \
      log_info("PE 0: Initialized src array with values [20..29]");            \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Starting context-based non-blocking get from PE 0");     \
      shmem_get_nbi(ctx, dest, src, 10, 0);                                    \
      log_info("PE 1: Calling quiet on context to ensure completion");         \
      shmem_ctx_quiet(ctx);                                                    \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Beginning validation of received data");                 \
      for (int i = 0; i < 10; i++) {                                           \
        if (dest[i] != i + 20) {                                               \
          log_fail("PE 1: Validation failed - dest[%d] = %d, expected %d", i,  \
                   (int)dest[i], i + 20);                                      \
          success = false;                                                     \
          break;                                                               \
        }                                                                      \
      }                                                                        \
      if (success) {                                                           \
        log_info("PE 1: Validation successful - all elements match expected "  \
                 "values");                                                    \
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

  if (!(shmem_n_pes() >= 2)) {
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

  result &= TEST_C11_SHMEM_GET_NBI(float);
  result &= TEST_C11_SHMEM_GET_NBI(double);
  result &= TEST_C11_SHMEM_GET_NBI(long double);
  result &= TEST_C11_SHMEM_GET_NBI(char);
  result &= TEST_C11_SHMEM_GET_NBI(signed char);
  result &= TEST_C11_SHMEM_GET_NBI(short);
  result &= TEST_C11_SHMEM_GET_NBI(int);
  result &= TEST_C11_SHMEM_GET_NBI(long);
  result &= TEST_C11_SHMEM_GET_NBI(long long);
  result &= TEST_C11_SHMEM_GET_NBI(unsigned char);
  result &= TEST_C11_SHMEM_GET_NBI(unsigned short);
  result &= TEST_C11_SHMEM_GET_NBI(unsigned int);
  result &= TEST_C11_SHMEM_GET_NBI(unsigned long);
  result &= TEST_C11_SHMEM_GET_NBI(unsigned long long);
  result &= TEST_C11_SHMEM_GET_NBI(int8_t);
  result &= TEST_C11_SHMEM_GET_NBI(int16_t);
  result &= TEST_C11_SHMEM_GET_NBI(int32_t);
  result &= TEST_C11_SHMEM_GET_NBI(int64_t);
  result &= TEST_C11_SHMEM_GET_NBI(uint8_t);
  result &= TEST_C11_SHMEM_GET_NBI(uint16_t);
  result &= TEST_C11_SHMEM_GET_NBI(uint32_t);
  result &= TEST_C11_SHMEM_GET_NBI(uint64_t);
  result &= TEST_C11_SHMEM_GET_NBI(size_t);
  result &= TEST_C11_SHMEM_GET_NBI(ptrdiff_t);

  shmem_barrier_all();

  if (!result) {
    rc = EXIT_FAILURE;
  }

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_get_nbi", result, false);
  }

  /* Test context-specific shmem_get_nbi variants */
  int result_ctx = true;

  result_ctx &= TEST_C11_CTX_SHMEM_GET_NBI(float);
  result_ctx &= TEST_C11_CTX_SHMEM_GET_NBI(double);
  result_ctx &= TEST_C11_CTX_SHMEM_GET_NBI(long double);
  result_ctx &= TEST_C11_CTX_SHMEM_GET_NBI(char);
  result_ctx &= TEST_C11_CTX_SHMEM_GET_NBI(signed char);
  result_ctx &= TEST_C11_CTX_SHMEM_GET_NBI(short);
  result_ctx &= TEST_C11_CTX_SHMEM_GET_NBI(int);
  result_ctx &= TEST_C11_CTX_SHMEM_GET_NBI(long);
  result_ctx &= TEST_C11_CTX_SHMEM_GET_NBI(long long);
  result_ctx &= TEST_C11_CTX_SHMEM_GET_NBI(unsigned char);
  result_ctx &= TEST_C11_CTX_SHMEM_GET_NBI(unsigned short);
  result_ctx &= TEST_C11_CTX_SHMEM_GET_NBI(unsigned int);
  result_ctx &= TEST_C11_CTX_SHMEM_GET_NBI(unsigned long);
  result_ctx &= TEST_C11_CTX_SHMEM_GET_NBI(unsigned long long);
  result_ctx &= TEST_C11_CTX_SHMEM_GET_NBI(int8_t);
  result_ctx &= TEST_C11_CTX_SHMEM_GET_NBI(int16_t);
  result_ctx &= TEST_C11_CTX_SHMEM_GET_NBI(int32_t);
  result_ctx &= TEST_C11_CTX_SHMEM_GET_NBI(int64_t);
  result_ctx &= TEST_C11_CTX_SHMEM_GET_NBI(uint8_t);
  result_ctx &= TEST_C11_CTX_SHMEM_GET_NBI(uint16_t);
  result_ctx &= TEST_C11_CTX_SHMEM_GET_NBI(uint32_t);
  result_ctx &= TEST_C11_CTX_SHMEM_GET_NBI(uint64_t);
  result_ctx &= TEST_C11_CTX_SHMEM_GET_NBI(size_t);
  result_ctx &= TEST_C11_CTX_SHMEM_GET_NBI(ptrdiff_t);

  shmem_barrier_all();

  if (!result_ctx) {
    rc = EXIT_FAILURE;
  }

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_get_nbi with ctx", result_ctx, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
