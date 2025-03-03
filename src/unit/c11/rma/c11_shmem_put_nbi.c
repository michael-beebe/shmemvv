/**
 * @file c11_shmem_put_nbi.c
 * @brief Unit test for the shmem_put_nbi() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C11_SHMEM_PUT_NBI(TYPE)                                           \
  ({                                                                           \
    log_routine("shmem_put_nbi(" #TYPE ")");                                   \
    bool success = true;                                                       \
    static TYPE src[10], dest[10];                                             \
    log_info("Allocated static arrays: src at %p, dest at %p", (void *)&src,   \
             (void *)&dest);                                                   \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    log_info("Running on PE %d of %d total PEs", mype, npes);                  \
                                                                               \
    for (int i = 0; i < 10; i++) {                                             \
      src[i] = i + mype;                                                       \
      log_info("PE %d: Initialized src[%d] = %d", mype, i, i + mype);          \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 0) {                                                           \
      log_info("PE 0: Starting non-blocking put operation to PE 1");           \
      log_info("PE 0: dest=%p, src=%p, nelems=10", (void *)dest, (void *)src); \
      shmem_put_nbi(dest, src, 10, 1);                                         \
      shmem_quiet();                                                           \
      log_info("PE 0: Completed put operation and quiet");                     \
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
        log_info("PE 1: dest[%d] = %d (valid)", i, (int)dest[i]);              \
      }                                                                        \
      if (success) {                                                           \
        log_info("PE 1: All elements validated successfully");                 \
      }                                                                        \
    } else {                                                                   \
      log_info("PE 0: Waiting for PE 1 to complete validation");               \
    }                                                                          \
                                                                               \
    success;                                                                   \
  })

#define TEST_C11_CTX_SHMEM_PUT_NBI(TYPE)                                       \
  ({                                                                           \
    log_routine("shmem_put_nbi(ctx, " #TYPE ")");                              \
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
    for (int i = 0; i < 10; i++) {                                             \
      src[i] = i + 20 + mype; /* Different values for context test */          \
      log_info("PE %d: Initialized src[%d] = %d", mype, i, i + 20 + mype);     \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 0) {                                                           \
      log_info("PE 0: Starting context-based non-blocking put to PE 1");       \
      log_info("PE 0: dest=%p, src=%p, nelems=10", (void *)dest, (void *)src); \
      shmem_put_nbi(ctx, dest, src, 10, 1);                                    \
      shmem_ctx_quiet(ctx);                                                    \
      log_info("PE 0: Completed context-based put operation and quiet");       \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Beginning validation of received data");                 \
      for (int i = 0; i < 10; i++) {                                           \
        int expected = i + 20; /* PE 0's value */                              \
        if (dest[i] != expected) {                                             \
          log_fail("PE 1: Validation failed - dest[%d] = %d, expected %d", i,  \
                   (int)dest[i], expected);                                    \
          success = false;                                                     \
          break;                                                               \
        }                                                                      \
        log_info("PE 1: dest[%d] = %d (valid)", i, (int)dest[i]);              \
      }                                                                        \
      if (success) {                                                           \
        log_info("PE 1: All elements validated successfully");                 \
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
    if (shmem_my_pe() == 0) {
      display_not_enough_pes("RMA");
    }
    shmem_finalize();
    return EXIT_SUCCESS;
  }

  int result = true;
  int rc = EXIT_SUCCESS;

  /* Test standard shmem_put_nbi variants */
  result &= TEST_C11_SHMEM_PUT_NBI(float);
  result &= TEST_C11_SHMEM_PUT_NBI(double);
  result &= TEST_C11_SHMEM_PUT_NBI(long double);
  result &= TEST_C11_SHMEM_PUT_NBI(char);
  result &= TEST_C11_SHMEM_PUT_NBI(signed char);
  result &= TEST_C11_SHMEM_PUT_NBI(short);
  result &= TEST_C11_SHMEM_PUT_NBI(int);
  result &= TEST_C11_SHMEM_PUT_NBI(long);
  result &= TEST_C11_SHMEM_PUT_NBI(long long);
  result &= TEST_C11_SHMEM_PUT_NBI(unsigned char);
  result &= TEST_C11_SHMEM_PUT_NBI(unsigned short);
  result &= TEST_C11_SHMEM_PUT_NBI(unsigned int);
  result &= TEST_C11_SHMEM_PUT_NBI(unsigned long);
  result &= TEST_C11_SHMEM_PUT_NBI(unsigned long long);
  result &= TEST_C11_SHMEM_PUT_NBI(int8_t);
  result &= TEST_C11_SHMEM_PUT_NBI(int16_t);
  result &= TEST_C11_SHMEM_PUT_NBI(int32_t);
  result &= TEST_C11_SHMEM_PUT_NBI(int64_t);
  result &= TEST_C11_SHMEM_PUT_NBI(uint8_t);
  result &= TEST_C11_SHMEM_PUT_NBI(uint16_t);
  result &= TEST_C11_SHMEM_PUT_NBI(uint32_t);
  result &= TEST_C11_SHMEM_PUT_NBI(uint64_t);
  result &= TEST_C11_SHMEM_PUT_NBI(size_t);
  result &= TEST_C11_SHMEM_PUT_NBI(ptrdiff_t);

  shmem_barrier_all();

  if (!result) {
    rc = EXIT_FAILURE;
  }

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_put_nbi", result, false);
  }

  /* Test context-specific shmem_put_nbi variants */
  int result_ctx = true;

  result_ctx &= TEST_C11_CTX_SHMEM_PUT_NBI(float);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_NBI(double);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_NBI(long double);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_NBI(char);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_NBI(signed char);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_NBI(short);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_NBI(int);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_NBI(long);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_NBI(long long);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_NBI(unsigned char);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_NBI(unsigned short);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_NBI(unsigned int);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_NBI(unsigned long);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_NBI(unsigned long long);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_NBI(int8_t);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_NBI(int16_t);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_NBI(int32_t);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_NBI(int64_t);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_NBI(uint8_t);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_NBI(uint16_t);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_NBI(uint32_t);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_NBI(uint64_t);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_NBI(size_t);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_NBI(ptrdiff_t);

  shmem_barrier_all();

  if (!result_ctx) {
    rc = EXIT_FAILURE;
  }

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_put_nbi with ctx", result_ctx, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
