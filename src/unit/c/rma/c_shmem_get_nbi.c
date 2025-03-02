/**
 * @file c_shmem_get_nbi.c
 * @brief Unit test for the shmem_get_nbi() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C_SHMEM_GET_NBI(TYPE, TYPENAME)                                   \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_get_nbi()");                              \
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
      shmem_##TYPENAME##_get_nbi(dest, src, 10, 0);                            \
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

#define TEST_C_CTX_SHMEM_GET_NBI(TYPE, TYPENAME)                               \
  ({                                                                           \
    log_routine("shmem_ctx_" #TYPENAME "_get_nbi()");                          \
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
        src[i] = i + 20;  /* Different values for context test */              \
      }                                                                        \
      log_info("PE 0: Initialized src array with values [20..29]");            \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Starting context-based non-blocking get from PE 0");     \
      shmem_ctx_##TYPENAME##_get_nbi(ctx, dest, src, 10, 0);                   \
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

  /* Test standard shmem_get_nbi variants */
  result &= TEST_C_SHMEM_GET_NBI(float, float);
  result &= TEST_C_SHMEM_GET_NBI(double, double);
  result &= TEST_C_SHMEM_GET_NBI(long double, longdouble);
  result &= TEST_C_SHMEM_GET_NBI(char, char);
  result &= TEST_C_SHMEM_GET_NBI(signed char, schar);
  result &= TEST_C_SHMEM_GET_NBI(short, short);
  result &= TEST_C_SHMEM_GET_NBI(int, int);
  result &= TEST_C_SHMEM_GET_NBI(long, long);
  result &= TEST_C_SHMEM_GET_NBI(long long, longlong);
  result &= TEST_C_SHMEM_GET_NBI(unsigned char, uchar);
  result &= TEST_C_SHMEM_GET_NBI(unsigned short, ushort);
  result &= TEST_C_SHMEM_GET_NBI(unsigned int, uint);
  result &= TEST_C_SHMEM_GET_NBI(unsigned long, ulong);
  result &= TEST_C_SHMEM_GET_NBI(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_GET_NBI(int8_t, int8);
  result &= TEST_C_SHMEM_GET_NBI(int16_t, int16);
  result &= TEST_C_SHMEM_GET_NBI(int32_t, int32);
  result &= TEST_C_SHMEM_GET_NBI(int64_t, int64);
  result &= TEST_C_SHMEM_GET_NBI(uint8_t, uint8);
  result &= TEST_C_SHMEM_GET_NBI(uint16_t, uint16);
  result &= TEST_C_SHMEM_GET_NBI(uint32_t, uint32);
  result &= TEST_C_SHMEM_GET_NBI(uint64_t, uint64);
  result &= TEST_C_SHMEM_GET_NBI(size_t, size);
  result &= TEST_C_SHMEM_GET_NBI(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (!result) {
    rc = EXIT_FAILURE;
  }

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_get_nbi", result, false);
  }

  /* Test context-specific shmem_get_nbi variants */
  int result_ctx = true;

  result_ctx &= TEST_C_CTX_SHMEM_GET_NBI(float, float);
  result_ctx &= TEST_C_CTX_SHMEM_GET_NBI(double, double);
  result_ctx &= TEST_C_CTX_SHMEM_GET_NBI(long double, longdouble);
  result_ctx &= TEST_C_CTX_SHMEM_GET_NBI(char, char);
  result_ctx &= TEST_C_CTX_SHMEM_GET_NBI(signed char, schar);
  result_ctx &= TEST_C_CTX_SHMEM_GET_NBI(short, short);
  result_ctx &= TEST_C_CTX_SHMEM_GET_NBI(int, int);
  result_ctx &= TEST_C_CTX_SHMEM_GET_NBI(long, long);
  result_ctx &= TEST_C_CTX_SHMEM_GET_NBI(long long, longlong);
  result_ctx &= TEST_C_CTX_SHMEM_GET_NBI(unsigned char, uchar);
  result_ctx &= TEST_C_CTX_SHMEM_GET_NBI(unsigned short, ushort);
  result_ctx &= TEST_C_CTX_SHMEM_GET_NBI(unsigned int, uint);
  result_ctx &= TEST_C_CTX_SHMEM_GET_NBI(unsigned long, ulong);
  result_ctx &= TEST_C_CTX_SHMEM_GET_NBI(unsigned long long, ulonglong);
  result_ctx &= TEST_C_CTX_SHMEM_GET_NBI(int8_t, int8);
  result_ctx &= TEST_C_CTX_SHMEM_GET_NBI(int16_t, int16);
  result_ctx &= TEST_C_CTX_SHMEM_GET_NBI(int32_t, int32);
  result_ctx &= TEST_C_CTX_SHMEM_GET_NBI(int64_t, int64);
  result_ctx &= TEST_C_CTX_SHMEM_GET_NBI(uint8_t, uint8);
  result_ctx &= TEST_C_CTX_SHMEM_GET_NBI(uint16_t, uint16);
  result_ctx &= TEST_C_CTX_SHMEM_GET_NBI(uint32_t, uint32);
  result_ctx &= TEST_C_CTX_SHMEM_GET_NBI(uint64_t, uint64);
  result_ctx &= TEST_C_CTX_SHMEM_GET_NBI(size_t, size);
  result_ctx &= TEST_C_CTX_SHMEM_GET_NBI(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (!result_ctx) {
    rc = EXIT_FAILURE;
  }

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_ctx_get_nbi", result_ctx, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
