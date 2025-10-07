/**
 * @file c11_shmem_put.c
 * @brief Unit test for the shmem_put() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"
#include "type_tables.h"

#define TEST_C11_SHMEM_PUT(TYPE)                                               \
  ({                                                                           \
    log_routine("shmem_put(" #TYPE ")");                                       \
    bool success = true;                                                       \
    static TYPE src[10] = {0}, dest[10] = {0};                                 \
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
      log_info("PE 0: Starting put operation to PE 1");                        \
      log_info("PE 0: dest=%p, src=%p, nelems=10", (void *)dest, (void *)src); \
      shmem_put(dest, src, 10, 1);                                             \
      /*immediately set values to 0, ensure routine is blocking*/              \
      for (int i = 0; i < 10; i++){                                            \
        src[i] = 0;                                                            \
      }                                                                        \
      log_info("PE 0: Completed put operation");                               \
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

#define TEST_C11_CTX_SHMEM_PUT(TYPE)                                           \
  ({                                                                           \
    log_routine("shmem_put(ctx, " #TYPE ")");                                  \
    bool success = true;                                                       \
    static TYPE src[10] = {0}, dest[10] = {0};                                 \
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
    shmem_ctx_quiet(ctx);                                                      \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 0) {                                                           \
      log_info("PE 0: Starting context-based put to PE 1");                    \
      log_info("PE 0: dest=%p, src=%p, nelems=10", (void *)dest, (void *)src); \
      shmem_put(ctx, dest, src, 10, 1);                                        \
      /*immediately set values to 0, ensure routine is blocking*/              \
      for (int i = 0; i < 10; i++){                                            \
        src[i] = 0;                                                            \
      }                                                                        \
      log_info("PE 0: Completed context-based put operation");                 \
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

  static int result = true;
  static int result_ctx = true;

  /* Test standard shmem_put variants */
  #define X(type, shmem_types) result &= TEST_C11_SHMEM_PUT(type);
    SHMEM_STANDARD_RMA_TYPE_TABLE(X)
  #undef X

  shmem_barrier_all();

  reduce_test_result("C11 shmem_put", &result, false);

  /* Test context-specific shmem_put variants */
  #define X(type, shmem_types) result_ctx &= TEST_C11_CTX_SHMEM_PUT(type);
    SHMEM_STANDARD_RMA_TYPE_TABLE(X)
  #undef X

  shmem_barrier_all();

  reduce_test_result("C11 shmem_put with ctx", &result_ctx, false);

  bool passed = result & result_ctx;
  log_close(!passed);
  shmem_finalize();
  return passed ? EXIT_SUCCESS : EXIT_FAILURE;
}
