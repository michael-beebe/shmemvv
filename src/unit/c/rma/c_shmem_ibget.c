/**
 * @file c_shmem_ibget.c
 * @brief Unit test for the shmem_ibget() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C_SHMEM_IBGET(TYPE, TYPENAME, DST, SST, BSIZE, NBLOCKS)         \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_ibget()");                                \
    bool success = true;                                                       \
    static TYPE src[SST * NBLOCKS], dest[DST * NBLOCKS];                       \
    log_info("Allocated static arrays: src at %p, dest at %p", (void *)&src,   \
             (void *)&dest);                                                   \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    int offset;                                                                \
    log_info("Running on PE %d of %d total PEs", mype, npes);                  \
                                                                               \
    if (mype == 0) {                                                           \
      for (int i = 0; i < NBLOCKS; i++) {                                      \
        for (int j = 0; j < BSIZE; j ++){                                      \
          offset = i * SST + j;                                                \
          src[offset] = i * BSIZE + j;                                         \
        }                                                                      \
      }                                                                        \
      log_info("PE 0: Initialized src array with values [0..%d]",              \
          BSIZE * NBLOCKS -1);                                                 \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype != 0) {                                                           \
      log_info("PE %d: Starting strided get from PE 0", mype);                 \
      log_info("PE %d: dest=%p, src=%p, dest_stride=%d, src_stride=%d, "       \
          "bsize=%d, nblocks=%d", mype, (void *)dest, (void *)src, DST, SST,   \
          BSIZE, NBLOCKS);                                                     \
      shmem_##TYPENAME##_ibget(dest, src, DST, SST, BSIZE, NBLOCKS, 0);        \
      log_info("PE %d: Completed strided get operation", mype);                \
                                                                               \
      log_info("PE %d: Beginning validation of received data", mype);          \
      /*ensure even indexes contain transfered data*/                          \
      int expected;                                                            \
      for (int i = 0; i < NBLOCKS; i++){                                       \
        for (int j = 0; j < DST; j ++){                                        \
          /* Indeces where data should be transfered */                        \
          expected = i * BSIZE + j;                                            \
          offset = i * DST + j;                                                \
          if (j < BSIZE && dest[offset] != expected){                          \
            log_fail("PE %d: Validation failed - dest[%d] = %d, expected %d",  \
                     mype, offset, (int)dest[offset], expected);               \
            success = false;                                                   \
          } /* Test that the right indeces have no data transfered */          \
          else if (j >= BSIZE && dest[offset] != 0){                           \
            log_fail("PE %d: Validation failed - dest[%d] = %d, expected %d",  \
                     mype, offset, (int)dest[offset], 0);                      \
            success = false;                                                   \
          }                                                                    \
        }                                                                      \
      }                                                                        \
      if (success) {                                                           \
        log_info("PE %d: Validation successful - all elements match expected " \
                 "values", mype);                                              \
      }                                                                        \
    } else {                                                                   \
      log_info("PE 0: Waiting for other PEs to complete validation");          \
    }                                                                          \
                                                                               \
    success;                                                                   \
  })

#define TEST_C_CTX_SHMEM_IBGET(TYPE, TYPENAME, DST, SST, BSIZE, NBLOCKS)     \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_ibget()");                                \
    bool success = true;                                                       \
    static TYPE src[SST * NBLOCKS], dest[DST * NBLOCKS];                       \
    log_info("Allocated static arrays: src at %p, dest at %p", (void *)&src,   \
             (void *)&dest);                                                   \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    int offset;                                                                \
    log_info("Running on PE %d of %d total PEs", mype, npes);                  \
                                                                               \
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
      for (int i = 0; i < NBLOCKS; i++) {                                      \
        for (int j = 0; j < BSIZE; j ++){                                      \
          offset = i * SST + j;                                                \
          src[offset] = i * BSIZE + j;                                         \
        }                                                                      \
      }                                                                        \
      log_info("PE 0: Initialized src array with values [0..%d]",              \
          BSIZE * NBLOCKS -1);                                                 \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype != 0) {                                                           \
      log_info("PE %d: Starting strided get from PE 0", mype);                 \
      log_info("PE %d: dest=%p, src=%p, dest_stride=%d, src_stride=%d, "       \
          "bsize=%d, nblocks=%d", mype, (void *)dest, (void *)src, DST, SST,   \
          BSIZE, NBLOCKS);                                                     \
      shmem_ctx_##TYPENAME##_ibget(ctx, dest, src, DST, SST, BSIZE, NBLOCKS, 0);   \
      log_info("PE %d: Completed strided get operation", mype);                \
                                                                               \
      log_info("PE %d: Beginning validation of received data", mype);          \
      /*ensure even indexes contain transfered data*/                          \
      int expected;                                                            \
      for (int i = 0; i < NBLOCKS; i++){                                       \
        for (int j = 0; j < DST; j ++){                                        \
          /* Indeces where data should be transfered */                        \
          expected = i * BSIZE + j;                                            \
          offset = i * DST + j;                                                \
          if (j < BSIZE && dest[offset] != expected){                          \
            log_fail("PE %d: Validation failed - dest[%d] = %d, expected %d",  \
                     mype, offset, (int)dest[offset], expected);               \
            success = false;                                                   \
          } /* Test that the right indeces have no data transfered */          \
          else if (j >= BSIZE && dest[offset] != 0){                           \
            log_fail("PE %d: Validation failed - dest[%d] = %d, expected %d",  \
                     mype, offset, (int)dest[offset], 0);                      \
            success = false;                                                   \
          }                                                                    \
        }                                                                      \
      }                                                                        \
      if (success) {                                                           \
        log_info("PE %d: Validation successful - all elements match expected " \
                 "values", mype);                                              \
      }                                                                        \
    } else {                                                                   \
      log_info("PE 0: Waiting for other PEs to complete validation");          \
    }                                                                          \
                                                                               \
    /* Destroy the context */                                                  \
    shmem_ctx_destroy(ctx);                                                    \
    log_info("Context destroyed");                                             \
    success;                                                                   \
  })

/* Test for SIZE-specific variants */
#define TEST_C_SHMEM_IBGETSIZE(SIZE, DST, SST, BSIZE, NBLOCKS)               \
  ({                                                                           \
    log_routine("shmem_ibget" #SIZE "()");                                     \
    bool success = true;                                                       \
    static uint##SIZE##_t src[SST * NBLOCKS], dest[DST * NBLOCKS];             \
    log_info("Allocated static arrays: src at %p, dest at %p", (void *)&src,   \
             (void *)&dest);                                                   \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    int offset;                                                                \
    log_info("Running on PE %d of %d total PEs", mype, npes);                  \
                                                                               \
    if (mype == 0) {                                                           \
      for (int i = 0; i < NBLOCKS; i++) {                                      \
        for (int j = 0; j < BSIZE; j ++){                                      \
          offset = i * SST + j;                                                \
          src[offset] = i * BSIZE + j;                                         \
        }                                                                      \
      }                                                                        \
      log_info("PE 0: Initialized src array with values [0..%d]",              \
          BSIZE * NBLOCKS -1);                                                 \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype != 0) {                                                           \
      log_info("PE %d: Starting SIZE-specific strided block get from PE 0",    \
          mype);                                                               \
      log_info("PE %d: dest=%p, src=%p, dest_stride=%d, src_stride=%d, "       \
          "bsize=%d, nblocks=%d", mype, (void *)dest, (void *)src, DST, SST,   \
          BSIZE, NBLOCKS);                                                     \
      shmem_ibget##SIZE(dest, src, DST, SST, BSIZE, NBLOCKS, 0);               \
      log_info("PE 1: Completed SIZE-specific ibget operation");               \
                                                                               \
      log_info("PE %d: Beginning validation of received data", mype);          \
      /*ensure even indexes contain transfered data*/                          \
      int expected;                                                            \
      for (int i = 0; i < NBLOCKS; i++){                                       \
        for (int j = 0; j < DST; j ++){                                        \
          /* Indeces where data should be transfered */                        \
          expected = i * BSIZE + j;                                            \
          offset = i * DST + j;                                                \
          if (j < BSIZE && dest[offset] != expected){                          \
            log_fail("PE %d: Validation failed - dest[%d] = %d, expected %d",  \
                     mype, offset, (int)dest[offset], expected);               \
            success = false;                                                   \
          } /* Test that the right indeces have no data transfered */          \
          else if (j >= BSIZE && dest[offset] != 0){                           \
            log_fail("PE %d: Validation failed - dest[%d] = %d, expected %d",  \
                     mype, offset, (int)dest[offset], 0);                      \
            success = false;                                                   \
          }                                                                    \
        }                                                                      \
      }                                                                        \
      if (success) {                                                           \
        log_info("PE %d: Validation successful - all elements match expected " \
                 "values", mype);                                              \
      }                                                                        \
    } else {                                                                   \
      log_info("PE 0: Waiting for other PEs to complete validation");          \
    }                                                                          \
                                                                               \
    success;                                                                   \
  })

#define TEST_C_CTX_SHMEM_IBGETSIZE(SIZE, DST, SST, BSIZE, NBLOCKS)           \
  ({                                                                           \
    log_routine("shmem_ctx_ibget" #SIZE "()");                                 \
    bool success = true;                                                       \
    static uint##SIZE##_t src[SST * NBLOCKS], dest[DST * NBLOCKS];             \
    log_info("Allocated static arrays: src at %p, dest at %p", (void *)&src,   \
             (void *)&dest);                                                   \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    int offset;                                                                \
    log_info("Running on PE %d of %d total PEs", mype, npes);                  \
                                                                               \
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
      for (int i = 0; i < NBLOCKS; i++) {                                      \
        for (int j = 0; j < BSIZE; j ++){                                      \
          offset = i * SST + j;                                                \
          src[offset] = i * BSIZE + j;                                         \
        }                                                                      \
      }                                                                        \
      log_info("PE 0: Initialized src array with values [0..%d]",              \
          BSIZE * NBLOCKS -1);                                                 \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype != 0) {                                                           \
      log_info("PE %d: Starting SIZE-specific strided block get from PE 0",    \
          mype);                                                               \
      log_info("PE %d: dest=%p, src=%p, dest_stride=%d, src_stride=%d, "       \
          "bsize=%d, nblocks=%d", mype, (void *)dest, (void *)src, DST, SST,   \
          BSIZE, NBLOCKS);                                                     \
      shmem_ctx_ibget##SIZE(ctx, dest, src, DST, SST, BSIZE, NBLOCKS, 0);       \
      log_info("PE %d: Completed context-based SIZE-specific strided get "     \
          "operation", mype);                                                  \
                                                                               \
      log_info("PE %d: Beginning validation of received data", mype);          \
      /*ensure even indexes contain transfered data*/                          \
      int expected;                                                            \
      for (int i = 0; i < NBLOCKS; i++){                                       \
        for (int j = 0; j < DST; j ++){                                        \
          /* Indeces where data should be transfered */                        \
          expected = i * BSIZE + j;                                            \
          offset = i * DST + j;                                                \
          if (j < BSIZE && dest[offset] != expected){                          \
            log_fail("PE %d: Validation failed - dest[%d] = %d, expected %d",  \
                     mype, offset, (int)dest[offset], expected);               \
            success = false;                                                   \
          } /* Test that the right indeces have no data transfered */          \
          else if (j >= BSIZE && dest[offset] != 0){                           \
            log_fail("PE %d: Validation failed - dest[%d] = %d, expected %d",  \
                     mype, offset, (int)dest[offset], 0);                      \
            success = false;                                                   \
          }                                                                    \
        }                                                                      \
      }                                                                        \
      if (success) {                                                           \
        log_info("PE %d: Validation successful - all elements match expected " \
                 "values", mype);                                              \
      }                                                                        \
    } else {                                                                   \
      log_info("PE 0: Waiting for other PEs to complete validation");          \
    }                                                                          \
                                                                               \
    /* Destroy the context */                                                  \
    shmem_ctx_destroy(ctx);                                                    \
    log_info("Context destroyed");                                             \
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

  static bool result = true;
  static bool result_ctx = true;
  static bool result_size = true;
  static bool result_ctx_size = true;

  /* Test standard shmem_ibget variants */
  #define X(type, typename) \
    result &= TEST_C_SHMEM_IBGET(type, typename, 4, 4, 4, 2); /* Same stride */ \
    result &= TEST_C_SHMEM_IBGET(type, typename, 4, 7, 4, 2); /* larger source stride*/ \
    result &= TEST_C_SHMEM_IBGET(type, typename, 7, 4, 4, 2); /* larger dest stride */ \

    SHMEM_STANDARD_RMA_TYPE_TABLE(X)
  #undef X

  shmem_barrier_all();

  reduce_test_result("C shmem_ibget", &result, false);

  /* Test SIZE-specific variants */
  result_size &= TEST_C_SHMEM_IBGETSIZE(8, 8, 8, 4, 2);
  result_size &= TEST_C_SHMEM_IBGETSIZE(16, 8, 8, 4, 2);
  result_size &= TEST_C_SHMEM_IBGETSIZE(32, 8, 8, 4, 2);
  result_size &= TEST_C_SHMEM_IBGETSIZE(64, 8, 8, 4, 2);
/* 128-bit operations may not be supported on all platforms */
#if defined(HAVE_FEATURE_PSHMEM) && defined(SHMEM_HAVE_EXTENDEDTYPES)
  result_size &= TEST_C_SHMEM_IBGETSIZE(128, 8, 8, 4, 2);
#endif

  shmem_barrier_all();
  
  reduce_test_result("C shmem_ibget<size>", &result_size, false);

  /* Test context-specific shmem_ibget variants */  
  #define X(type, typename) \
    result_ctx &= TEST_C_CTX_SHMEM_IBGET(type, typename, 4, 4, 4, 2); \
    result_ctx &= TEST_C_CTX_SHMEM_IBGET(type, typename, 4, 7, 4, 2); \
    result_ctx &= TEST_C_CTX_SHMEM_IBGET(type, typename, 7, 4, 4, 2);

    SHMEM_STANDARD_RMA_TYPE_TABLE(X)
  #undef X

  shmem_barrier_all();

  reduce_test_result("C shmem_ibget with ctx", &result_ctx, false);

  /* Test SIZE-specific context variants */
  result_ctx_size &= TEST_C_CTX_SHMEM_IBGETSIZE(8, 8, 8, 4, 2);
  result_ctx_size &= TEST_C_CTX_SHMEM_IBGETSIZE(16, 8, 8, 4, 2);
  result_ctx_size &= TEST_C_CTX_SHMEM_IBGETSIZE(32, 8, 8, 4, 2);
  result_ctx_size &= TEST_C_CTX_SHMEM_IBGETSIZE(64, 8, 8, 4, 2);
/* 128-bit operations may not be supported on all platforms */
#if defined(HAVE_FEATURE_PSHMEM) && defined(SHMEM_HAVE_EXTENDEDTYPES)
  result_ctx_size &= TEST_C_CTX_SHMEM_IBGETSIZE(128, 8, 8, 4, 2);
#endif

  shmem_barrier_all();
  
  reduce_test_result("C shmem_ctx_ibget<size>", &result_ctx_size, false);

  bool passed = result & result_ctx & result_size & result_ctx_size;
  log_close(!passed);
  shmem_finalize();
  return passed ? EXIT_SUCCESS : EXIT_FAILURE;
}
