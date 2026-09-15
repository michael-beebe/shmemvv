/**
 * @file c11_shmem_ibput.c
 * @brief Unit test for the shmem_ibput() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"
#include "type_tables.h"

#define TEST_C11_SHMEM_IBPUT(TYPE, DST, SST, BSIZE, NBLOCKS)                   \
  ({                                                                           \
    log_routine("shmem_ibput(" #TYPE ")");                                     \
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
      log_info("Doing shmem ibput from PE 0 to all PEs");                      \
      log_info("PE 0: dest=%p, src=%p, dest_stride=%d, src_stride=%d, "        \
          "bsize=%d, nblocks=%d", (void *)dest, (void *)src, DST, SST,         \
          BSIZE, NBLOCKS);                                                     \
      for (int i = 0; i < npes; i ++){                                         \
        shmem_ibput(dest, src, DST, SST, BSIZE, NBLOCKS, i);                   \
      }                                                                        \
      log_info("PE 0: Completed strided block put operations");                \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype != 0) {                                                           \
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



#define TEST_C11_CTX_SHMEM_IBPUT(TYPE, DST, SST, BSIZE, NBLOCKS)               \
  ({                                                                           \
    log_routine("shmem_ibput(" #TYPE ")");                                     \
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
      log_info("Doing shmem ibput from PE 0 to all PEs");                      \
      log_info("PE 0: dest=%p, src=%p, dest_stride=%d, src_stride=%d, "        \
          "bsize=%d, nblocks=%d", (void *)dest, (void *)src, DST, SST,         \
          BSIZE, NBLOCKS);                                                     \
      for (int i = 0; i < npes; i ++){                                         \
        shmem_ibput(ctx, dest, src, DST, SST, BSIZE, NBLOCKS, i);          \
      }                                                                        \
      log_info("PE 0: Completed strided block put operations");                \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype != 0) {                                                           \
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

  /* Test standard shmem_ibput variants */
  #define X(type, shmem_types) \
    result &= TEST_C11_SHMEM_IBPUT(type, 4, 4, 4, 2); /* Same stride */ \
    result &= TEST_C11_SHMEM_IBPUT(type, 4, 7, 4, 2); /* larger source stride*/ \
    result &= TEST_C11_SHMEM_IBPUT(type, 7, 4, 4, 2); /* larger dest stride */ \

    SHMEM_STANDARD_RMA_TYPE_TABLE(X)
  #undef X

  shmem_barrier_all();

  reduce_test_result("C11 shmem_ibput", &result, false);


  /* Test context-specific shmem_ibput variants */  
  #define X(type, shmem_types) \
    result_ctx &= TEST_C11_CTX_SHMEM_IBPUT(type, 4, 4, 4, 2); \
    result_ctx &= TEST_C11_CTX_SHMEM_IBPUT(type, 4, 7, 4, 2); \
    result_ctx &= TEST_C11_CTX_SHMEM_IBPUT(type, 7, 4, 4, 2);

    SHMEM_STANDARD_RMA_TYPE_TABLE(X)
  #undef X

  shmem_barrier_all();

  reduce_test_result("C11 shmem_ibput with ctx", &result_ctx, false);

  bool passed = result & result_ctx;
  log_close(!passed);
  shmem_finalize();
  return passed ? EXIT_SUCCESS : EXIT_FAILURE;
}
