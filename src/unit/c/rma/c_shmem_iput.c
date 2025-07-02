/**
 * @file c_shmem_iput.c
 * @brief Unit test for the shmem_iput() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C_SHMEM_IPUT(TYPE, TYPENAME)                                      \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_iput()");                                 \
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
      shmem_##TYPENAME##_iput(dest, src, 2, 2, 5, 1);                          \
      log_info("PE 0: Completed strided put operation");                       \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Beginning validation of received data");                 \
      for (int i = 0; i < 10; i += 2) {                                        \
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

#define TEST_C_CTX_SHMEM_IPUT(TYPE, TYPENAME)                                  \
  ({                                                                           \
    log_routine("shmem_ctx_" #TYPENAME "_iput()");                             \
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
      shmem_ctx_##TYPENAME##_iput(ctx, dest, src, 2, 2, 5, 1);                 \
      log_info("PE 0: Completed context-based strided put operation");         \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Beginning validation of received data");                 \
      for (int i = 0; i < 10; i += 2) {                                        \
        int expected = i + 20; /* PE 0's value */                              \
        if (dest[i] != expected) {                                             \
          log_fail("PE 1: Validation failed - dest[%d] = %d, expected %d", i,  \
                   (int)dest[i], expected);                                    \
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

/* Test for SIZE-specific variants */
#define TEST_C_SHMEM_IPUTSIZE(SIZE)                                            \
  ({                                                                           \
    log_routine("shmem_iput" #SIZE "()");                                      \
    bool success = true;                                                       \
    static uint##SIZE##_t src[10], dest[10];                                   \
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
      log_info("PE 0: Starting SIZE-specific strided put to PE 1");            \
      log_info("PE 0: dest=%p, src=%p, dest_stride=2, src_stride=2, nelems=5", \
               (void *)dest, (void *)src);                                     \
      shmem_iput##SIZE(dest, src, 2, 2, 5, 1);                                 \
      log_info("PE 0: Completed SIZE-specific strided put operation");         \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Beginning validation of received data");                 \
      for (int i = 0; i < 10; i += 2) {                                        \
        if (dest[i] != i) {                                                    \
          log_fail("PE 1: Validation failed - dest[%d] = %d, expected %d", i,  \
                   (int)dest[i], i);                                           \
          success = false;                                                     \
          break;                                                               \
        }                                                                      \
        log_info("PE 1: dest[%d] = %d (valid)", i, (int)dest[i]);              \
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

/* Test for context SIZE-specific variants */
#define TEST_C_CTX_SHMEM_IPUTSIZE(SIZE)                                        \
  ({                                                                           \
    log_routine("shmem_ctx_iput" #SIZE "()");                                  \
    bool success = true;                                                       \
    static uint##SIZE##_t src[10], dest[10];                                   \
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
      log_info(                                                                \
          "PE 0: Starting context-based SIZE-specific strided put to PE 1");   \
      log_info("PE 0: dest=%p, src=%p, dest_stride=2, src_stride=2, nelems=5", \
               (void *)dest, (void *)src);                                     \
      shmem_ctx_iput##SIZE(ctx, dest, src, 2, 2, 5, 1);                        \
      log_info("PE 0: Completed context-based SIZE-specific strided put "      \
               "operation");                                                   \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Beginning validation of received data");                 \
      for (int i = 0; i < 10; i += 2) {                                        \
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

  /* Test standard shmem_iput variants */
  result &= TEST_C_SHMEM_IPUT(float, float);
  result &= TEST_C_SHMEM_IPUT(double, double);
  result &= TEST_C_SHMEM_IPUT(long double, longdouble);
  result &= TEST_C_SHMEM_IPUT(char, char);
  result &= TEST_C_SHMEM_IPUT(signed char, schar);
  result &= TEST_C_SHMEM_IPUT(short, short);
  result &= TEST_C_SHMEM_IPUT(int, int);
  result &= TEST_C_SHMEM_IPUT(long, long);
  result &= TEST_C_SHMEM_IPUT(long long, longlong);
  result &= TEST_C_SHMEM_IPUT(unsigned char, uchar);
  result &= TEST_C_SHMEM_IPUT(unsigned short, ushort);
  result &= TEST_C_SHMEM_IPUT(unsigned int, uint);
  result &= TEST_C_SHMEM_IPUT(unsigned long, ulong);
  result &= TEST_C_SHMEM_IPUT(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_IPUT(int8_t, int8);
  result &= TEST_C_SHMEM_IPUT(int16_t, int16);
  result &= TEST_C_SHMEM_IPUT(int32_t, int32);
  result &= TEST_C_SHMEM_IPUT(int64_t, int64);
  result &= TEST_C_SHMEM_IPUT(uint8_t, uint8);
  result &= TEST_C_SHMEM_IPUT(uint16_t, uint16);
  result &= TEST_C_SHMEM_IPUT(uint32_t, uint32);
  result &= TEST_C_SHMEM_IPUT(uint64_t, uint64);
  result &= TEST_C_SHMEM_IPUT(size_t, size);
  result &= TEST_C_SHMEM_IPUT(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (!result) {
    rc = EXIT_FAILURE;
  }

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_iput", result, false);
  }

  /* Test SIZE-specific variants */
  int result_size = true;
  result_size &= TEST_C_SHMEM_IPUTSIZE(8);
  result_size &= TEST_C_SHMEM_IPUTSIZE(16);
  result_size &= TEST_C_SHMEM_IPUTSIZE(32);
  result_size &= TEST_C_SHMEM_IPUTSIZE(64);
/* 128-bit operations may not be supported on all platforms */
#if defined(HAVE_FEATURE_PSHMEM) && defined(SHMEM_HAVE_EXTENDEDTYPES)
  result_size &= TEST_C_SHMEM_IPUTSIZE(128);
#endif

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_iput<size>", result_size, false);
  }

  if (!result_size) {
    rc = EXIT_FAILURE;
  }

  /* Test context-specific shmem_iput variants */
  int result_ctx = true;

  result_ctx &= TEST_C_CTX_SHMEM_IPUT(float, float);
  result_ctx &= TEST_C_CTX_SHMEM_IPUT(double, double);
  result_ctx &= TEST_C_CTX_SHMEM_IPUT(long double, longdouble);
  result_ctx &= TEST_C_CTX_SHMEM_IPUT(char, char);
  result_ctx &= TEST_C_CTX_SHMEM_IPUT(signed char, schar);
  result_ctx &= TEST_C_CTX_SHMEM_IPUT(short, short);
  result_ctx &= TEST_C_CTX_SHMEM_IPUT(int, int);
  result_ctx &= TEST_C_CTX_SHMEM_IPUT(long, long);
  result_ctx &= TEST_C_CTX_SHMEM_IPUT(long long, longlong);
  result_ctx &= TEST_C_CTX_SHMEM_IPUT(unsigned char, uchar);
  result_ctx &= TEST_C_CTX_SHMEM_IPUT(unsigned short, ushort);
  result_ctx &= TEST_C_CTX_SHMEM_IPUT(unsigned int, uint);
  result_ctx &= TEST_C_CTX_SHMEM_IPUT(unsigned long, ulong);
  result_ctx &= TEST_C_CTX_SHMEM_IPUT(unsigned long long, ulonglong);
  result_ctx &= TEST_C_CTX_SHMEM_IPUT(int8_t, int8);
  result_ctx &= TEST_C_CTX_SHMEM_IPUT(int16_t, int16);
  result_ctx &= TEST_C_CTX_SHMEM_IPUT(int32_t, int32);
  result_ctx &= TEST_C_CTX_SHMEM_IPUT(int64_t, int64);
  result_ctx &= TEST_C_CTX_SHMEM_IPUT(uint8_t, uint8);
  result_ctx &= TEST_C_CTX_SHMEM_IPUT(uint16_t, uint16);
  result_ctx &= TEST_C_CTX_SHMEM_IPUT(uint32_t, uint32);
  result_ctx &= TEST_C_CTX_SHMEM_IPUT(uint64_t, uint64);
  result_ctx &= TEST_C_CTX_SHMEM_IPUT(size_t, size);
  result_ctx &= TEST_C_CTX_SHMEM_IPUT(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (!result_ctx) {
    rc = EXIT_FAILURE;
  }

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_ctx_iput", result_ctx, false);
  }

  /* Test context SIZE-specific variants */
  int result_ctx_size = true;
  result_ctx_size &= TEST_C_CTX_SHMEM_IPUTSIZE(8);
  result_ctx_size &= TEST_C_CTX_SHMEM_IPUTSIZE(16);
  result_ctx_size &= TEST_C_CTX_SHMEM_IPUTSIZE(32);
  result_ctx_size &= TEST_C_CTX_SHMEM_IPUTSIZE(64);
/* 128-bit operations may not be supported on all platforms */
#if defined(HAVE_FEATURE_PSHMEM) && defined(SHMEM_HAVE_EXTENDEDTYPES)
  result_ctx_size &= TEST_C_CTX_SHMEM_IPUTSIZE(128);
#endif

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_ctx_iput<size>", result_ctx_size, false);
  }

  if (!result_ctx_size) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
