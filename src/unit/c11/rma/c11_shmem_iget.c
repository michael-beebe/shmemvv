/**
 * @file c11_shmem_iget.c
 * @brief Unit test for the shmem_iget() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"
#include "type_tables.h"

#define TEST_C11_SHMEM_IGET(TYPE)                                              \
  ({                                                                           \
    log_routine("shmem_iget(" #TYPE ")");                                      \
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
      log_info("PE 1: Starting strided get from PE 0");                        \
      log_info("PE 1: dest=%p, src=%p, dest_stride=2, src_stride=2, nelems=5", \
               (void *)dest, (void *)src);                                     \
      shmem_iget(dest, src, 2, 2, 5, 0);                                       \
      log_info("PE 1: Completed strided get operation");                       \
                                                                               \
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
      for (int i = 1; i < 10; i += 2) {                                        \
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

#define TEST_C11_CTX_SHMEM_IGET(TYPE)                                          \
  ({                                                                           \
    log_routine("shmem_iget(ctx, " #TYPE ")");                                 \
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
      log_info("PE 1: Starting context-based strided get from PE 0");          \
      log_info("PE 1: dest=%p, src=%p, dest_stride=2, src_stride=2, nelems=5", \
               (void *)dest, (void *)src);                                     \
      shmem_iget(ctx, dest, src, 2, 2, 5, 0);                                  \
      log_info("PE 1: Completed context-based strided get operation");         \
                                                                               \
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
      for (int i = 1; i < 10; i += 2) {                                        \
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

  /* Test standard shmem_iget variants */
  #define X(type, shmem_types) result &= TEST_C11_SHMEM_IGET(type);
    SHMEM_STANDARD_RMA_TYPE_TABLE(X)
  #undef X

  shmem_barrier_all();

  reduce_test_result("C11 shmem_iput", &result, false);


  /* Test context-specific shmem_iget variants */  
  #define X(type, shmem_types) result_ctx &= TEST_C11_CTX_SHMEM_IGET(type);
    SHMEM_STANDARD_RMA_TYPE_TABLE(X)
  #undef X

  shmem_barrier_all();

  reduce_test_result("C11 shmem_iput with ctx", &result_ctx, false);

  log_close(result & result_ctx);
  shmem_finalize();
  return result & result_ctx ? EXIT_SUCCESS : EXIT_FAILURE;
}
