/**
 * @file c11_shmem_iput.c
 * @brief Unit test for the shmem_iput() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"
#include "type_tables.h"

#define TEST_C11_SHMEM_IPUT(TYPE)                                              \
  ({                                                                           \
    log_routine("shmem_iput(" #TYPE ")");                                      \
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
    }                                                                          \
    log_info("PE %d: Initialized src array with values [%d..%d]", mype, mype,  \
             mype + 9);                                                        \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 0) {                                                           \
      log_info("PE 0: Starting strided put to PE 1");                          \
      log_info("PE 0: dest=%p, src=%p, dest_stride=2, src_stride=2, nelems=5", \
               (void *)dest, (void *)src);                                     \
      shmem_iput(dest, src, 2, 2, 5, 1);                                       \
      /*set src array to 0 to test shmem_iput blocking behavior*/              \
      for (int i = 0; i < 10; i++){                                            \
        src[i] = 0;                                                            \
      }                                                                        \
      log_info("PE 0: Completed strided put operation");                       \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Beginning validation of received data");                 \
      /*ensure even indexes contain transfered data*/                          \
      for (int i = 0; i < 10; i += 2) {                                        \
        if (dest[i] != i) {                                                    \
          log_fail("PE 1: Validation failed - dest[%d] = %d, expected %d", i,  \
                   (int)dest[i], i);                                           \
          success = false;                                                     \
          break;                                                               \
        }                                                                      \
      }                                                                        \
      log_info("PE 1: Beginning validation of unchanged elements between "     \
         "strides");                                                           \
      /*ensure odd indexes do not contain transfered data*/                    \
      for (int i = 1; i < 10; i += 2){                                         \
        if (dest[i] != 0) {                                                    \
          log_fail("PE 1: Validation failed - dest[%d] = %d, expected 0", i,   \
                   (int)dest[i]);                                              \
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

#define TEST_C11_CTX_SHMEM_IPUT(TYPE)                                          \
  ({                                                                           \
    log_routine("shmem_iput(ctx, " #TYPE ")");                                 \
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
    }                                                                          \
    log_info("PE %d: Initialized src array with values [%d..%d]", mype,        \
             20 + mype, 29 + mype);                                            \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 0) {                                                           \
      log_info("PE 0: Starting context-based strided put to PE 1");            \
      log_info("PE 0: dest=%p, src=%p, dest_stride=2, src_stride=2, nelems=5", \
               (void *)dest, (void *)src);                                     \
      shmem_iput(ctx, dest, src, 2, 2, 5, 1);                                  \
      /*set src array to 0 to test shmem_iput blocking behavior*/              \
      for (int i = 0; i < 10; i++){                                            \
        src[i] = 0;                                                            \
      }                                                                        \
      log_info("PE 0: Completed context-based strided put operation");         \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Beginning validation of received data");                 \
      /*ensure even indexes contain transfered data*/                          \
      for (int i = 0; i < 10; i += 2) {                                        \
        int expected = i + 20;                                                 \
        if (dest[i] != expected) {                                             \
          log_fail("PE 1: Validation failed - dest[%d] = %d, expected %d", i,  \
                   (int)dest[i], expected);                                    \
          success = false;                                                     \
          break;                                                               \
        }                                                                      \
      }                                                                        \
      log_info("PE 1: Beginning validation of unchanged elements between "     \
         "strides");                                                           \
      /*ensure odd indexes do not contain transfered data*/                    \
      for (int i = 1; i < 10; i += 2){                                         \
        if (dest[i] != 0) {                                                    \
          log_fail("PE 1: Validation failed - dest[%d] = %d, expected 0", i,   \
                   (int)dest[i]);                                              \
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

  static int result = true;
  static int result_ctx = true;

  /* Test standard shmem_iput variants */
  #define X(type, shmem_types) result &= TEST_C11_SHMEM_IPUT(type);
    SHMEM_STANDARD_RMA_TYPE_TABLE(X)
  #undef X

  shmem_barrier_all();

  reduce_test_result("C11 shmem_iput", &result, false);

  /* Test context-specific shmem_iput variants */
  #define X(type, shmem_types) result_ctx &= TEST_C11_CTX_SHMEM_IPUT(type);
    SHMEM_STANDARD_RMA_TYPE_TABLE(X)
  #undef X

  shmem_barrier_all();

  reduce_test_result("C11 shmem_iput with ctx", &result_ctx, false);


  log_close(result & result_ctx);
  shmem_finalize();
  return result & result_ctx ? EXIT_SUCCESS : EXIT_FAILURE;
}
