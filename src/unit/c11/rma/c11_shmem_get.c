/**
 * @file c11_shmem_get.c
 * @brief Unit test for the shmem_get() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C11_SHMEM_GET(TYPE)                                               \
  ({                                                                           \
    log_routine("shmem_get(" #TYPE ")");                                       \
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
      log_info("PE 1: Starting get of 10 elements from PE 0");                 \
      shmem_get(dest, src, 10, 0);                                             \
      log_info("PE 1: Completed get operation");                               \
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

#define TEST_C11_CTX_SHMEM_GET(TYPE)                                           \
  ({                                                                           \
    log_routine("shmem_get(ctx, " #TYPE ")");                                  \
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
      log_info("PE 1: Starting context-based get of 10 elements from PE 0");   \
      shmem_get(ctx, dest, src, 10, 0);                                        \
      log_info("PE 1: Completed context-based get operation");                 \
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

  /* Test standard shmem_get variants */
  result &= TEST_C11_SHMEM_GET(float);
  result &= TEST_C11_SHMEM_GET(double);
  result &= TEST_C11_SHMEM_GET(long double);
  result &= TEST_C11_SHMEM_GET(char);
  result &= TEST_C11_SHMEM_GET(signed char);
  result &= TEST_C11_SHMEM_GET(short);
  result &= TEST_C11_SHMEM_GET(int);
  result &= TEST_C11_SHMEM_GET(long);
  result &= TEST_C11_SHMEM_GET(long long);
  result &= TEST_C11_SHMEM_GET(unsigned char);
  result &= TEST_C11_SHMEM_GET(unsigned short);
  result &= TEST_C11_SHMEM_GET(unsigned int);
  result &= TEST_C11_SHMEM_GET(unsigned long);
  result &= TEST_C11_SHMEM_GET(unsigned long long);
  result &= TEST_C11_SHMEM_GET(int8_t);
  result &= TEST_C11_SHMEM_GET(int16_t);
  result &= TEST_C11_SHMEM_GET(int32_t);
  result &= TEST_C11_SHMEM_GET(int64_t);
  result &= TEST_C11_SHMEM_GET(uint8_t);
  result &= TEST_C11_SHMEM_GET(uint16_t);
  result &= TEST_C11_SHMEM_GET(uint32_t);
  result &= TEST_C11_SHMEM_GET(uint64_t);
  result &= TEST_C11_SHMEM_GET(size_t);
  result &= TEST_C11_SHMEM_GET(ptrdiff_t);

  shmem_barrier_all();

  if (!result) {
    rc = EXIT_FAILURE;
  }

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_get", result, false);
  }

  /* Test context-specific shmem_get variants */
  int result_ctx = true;

  result_ctx &= TEST_C11_CTX_SHMEM_GET(float);
  result_ctx &= TEST_C11_CTX_SHMEM_GET(double);
  result_ctx &= TEST_C11_CTX_SHMEM_GET(long double);
  result_ctx &= TEST_C11_CTX_SHMEM_GET(char);
  result_ctx &= TEST_C11_CTX_SHMEM_GET(signed char);
  result_ctx &= TEST_C11_CTX_SHMEM_GET(short);
  result_ctx &= TEST_C11_CTX_SHMEM_GET(int);
  result_ctx &= TEST_C11_CTX_SHMEM_GET(long);
  result_ctx &= TEST_C11_CTX_SHMEM_GET(long long);
  result_ctx &= TEST_C11_CTX_SHMEM_GET(unsigned char);
  result_ctx &= TEST_C11_CTX_SHMEM_GET(unsigned short);
  result_ctx &= TEST_C11_CTX_SHMEM_GET(unsigned int);
  result_ctx &= TEST_C11_CTX_SHMEM_GET(unsigned long);
  result_ctx &= TEST_C11_CTX_SHMEM_GET(unsigned long long);
  result_ctx &= TEST_C11_CTX_SHMEM_GET(int8_t);
  result_ctx &= TEST_C11_CTX_SHMEM_GET(int16_t);
  result_ctx &= TEST_C11_CTX_SHMEM_GET(int32_t);
  result_ctx &= TEST_C11_CTX_SHMEM_GET(int64_t);
  result_ctx &= TEST_C11_CTX_SHMEM_GET(uint8_t);
  result_ctx &= TEST_C11_CTX_SHMEM_GET(uint16_t);
  result_ctx &= TEST_C11_CTX_SHMEM_GET(uint32_t);
  result_ctx &= TEST_C11_CTX_SHMEM_GET(uint64_t);
  result_ctx &= TEST_C11_CTX_SHMEM_GET(size_t);
  result_ctx &= TEST_C11_CTX_SHMEM_GET(ptrdiff_t);

  shmem_barrier_all();

  if (!result_ctx) {
    rc = EXIT_FAILURE;
  }

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_get with ctx", result_ctx, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
