/**
 * @file c11_shmem_g.c
 * @brief Unit test for the shmem_g() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"
#include "type_tables.h"

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
                                                                               \
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
                                                                               \
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


  #define X(type, shmem_types) result &= TEST_C11_SHMEM_G(type);
    SHMEM_STANDARD_RMA_TYPE_TABLE(X)
  #undef X

  shmem_barrier_all();

  if (!result) {
    rc = EXIT_FAILURE;
  }

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_g", result, false);
  }

  int result_ctx = true;

  /* Test context-specific shmem_g variants */
  #define X(type, shmem_types) result_ctx &= TEST_C11_CTX_SHMEM_G(type);
    SHMEM_STANDARD_RMA_TYPE_TABLE(X)
  #undef X

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
