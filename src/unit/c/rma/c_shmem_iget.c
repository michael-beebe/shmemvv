/**
 * @file c_shmem_iget.c
 * @brief Unit test for the shmem_iget() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C_SHMEM_IGET(TYPE, TYPENAME)                                      \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_iget()");                                 \
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
        log_info("PE %d: Initialized src[%d] = %d", mype, i, i);               \
      }                                                                        \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Starting iget operation from PE 0");                     \
      log_info("PE 1: dest=%p, src=%p, dst_stride=2, src_stride=2, nelems=5",  \
               (void *)dest, (void *)src);                                     \
      shmem_##TYPENAME##_iget(dest, src, 2, 2, 5, 0);                          \
      log_info("PE 1: Completed iget operation");                              \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Beginning validation of received data");                 \
      for (int i = 0; i < 10; i += 2) {                                        \
        if (dest[i] != i / 2) {                                                \
          log_fail("PE 1: Validation failed - dest[%d] = %d, expected %d", i,  \
                   (int)dest[i], i / 2);                                       \
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

#define TEST_C_CTX_SHMEM_IGET(TYPE, TYPENAME)                                  \
  ({                                                                           \
    log_routine("shmem_ctx_" #TYPENAME "_iget()");                             \
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
        log_info("PE %d: Initialized src[%d] = %d", mype, i, i + 20);          \
      }                                                                        \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Starting context-based iget operation from PE 0");       \
      log_info("PE 1: dest=%p, src=%p, dst_stride=2, src_stride=2, nelems=5",  \
               (void *)dest, (void *)src);                                     \
      shmem_ctx_##TYPENAME##_iget(ctx, dest, src, 2, 2, 5, 0);                 \
      log_info("PE 1: Completed context-based iget operation");                \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Beginning validation of received data");                 \
      for (int i = 0; i < 10; i += 2) {                                        \
        int expected = (i / 2) + 20;                                           \
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

/* Test for SIZE-specific variants */
#define TEST_C_SHMEM_IGETSIZE(SIZE)                                            \
  ({                                                                           \
    log_routine("shmem_iget" #SIZE "()");                                      \
    bool success = true;                                                       \
    static uint##SIZE##_t src[10], dest[10];                                   \
    log_info("Allocated static arrays: src at %p, dest at %p", (void *)&src,   \
             (void *)&dest);                                                   \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    log_info("Running on PE %d of %d total PEs", mype, npes);                  \
                                                                               \
    if (mype == 0) {                                                           \
      for (int i = 0; i < 10; i++) {                                           \
        src[i] = i;                                                            \
        log_info("PE %d: Initialized src[%d] = %d", mype, i, i);               \
      }                                                                        \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Starting SIZE-specific iget operation from PE 0");       \
      log_info("PE 1: dest=%p, src=%p, dst_stride=2, src_stride=2, nelems=5",  \
               (void *)dest, (void *)src);                                     \
      shmem_iget##SIZE(dest, src, 2, 2, 5, 0);                                 \
      log_info("PE 1: Completed SIZE-specific iget operation");                \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Beginning validation of received data");                 \
      for (int i = 0; i < 10; i += 2) {                                        \
        if (dest[i] != i / 2) {                                                \
          log_fail("PE 1: Validation failed - dest[%d] = %d, expected %d", i,  \
                   (int)dest[i], i / 2);                                       \
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

/* Test for context SIZE-specific variants */
#define TEST_C_CTX_SHMEM_IGETSIZE(SIZE)                                        \
  ({                                                                           \
    log_routine("shmem_ctx_iget" #SIZE "()");                                  \
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
    if (mype == 0) {                                                           \
      for (int i = 0; i < 10; i++) {                                           \
        src[i] = i + 20; /* Different values for context test */               \
        log_info("PE %d: Initialized src[%d] = %d", mype, i, i + 20);          \
      }                                                                        \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Starting context-based SIZE-specific iget from PE 0");   \
      log_info("PE 1: dest=%p, src=%p, dst_stride=2, src_stride=2, nelems=5",  \
               (void *)dest, (void *)src);                                     \
      shmem_ctx_iget##SIZE(ctx, dest, src, 2, 2, 5, 0);                        \
      log_info("PE 1: Completed context-based SIZE-specific iget operation");  \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Beginning validation of received data");                 \
      for (int i = 0; i < 10; i += 2) {                                        \
        int expected = (i / 2) + 20;                                           \
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

  /* Test standard shmem_iget variants */
  result &= TEST_C_SHMEM_IGET(float, float);
  result &= TEST_C_SHMEM_IGET(double, double);
  result &= TEST_C_SHMEM_IGET(long double, longdouble);
  result &= TEST_C_SHMEM_IGET(char, char);
  result &= TEST_C_SHMEM_IGET(signed char, schar);
  result &= TEST_C_SHMEM_IGET(short, short);
  result &= TEST_C_SHMEM_IGET(int, int);
  result &= TEST_C_SHMEM_IGET(long, long);
  result &= TEST_C_SHMEM_IGET(long long, longlong);
  result &= TEST_C_SHMEM_IGET(unsigned char, uchar);
  result &= TEST_C_SHMEM_IGET(unsigned short, ushort);
  result &= TEST_C_SHMEM_IGET(unsigned int, uint);
  result &= TEST_C_SHMEM_IGET(unsigned long, ulong);
  result &= TEST_C_SHMEM_IGET(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_IGET(int8_t, int8);
  result &= TEST_C_SHMEM_IGET(int16_t, int16);
  result &= TEST_C_SHMEM_IGET(int32_t, int32);
  result &= TEST_C_SHMEM_IGET(int64_t, int64);
  result &= TEST_C_SHMEM_IGET(uint8_t, uint8);
  result &= TEST_C_SHMEM_IGET(uint16_t, uint16);
  result &= TEST_C_SHMEM_IGET(uint32_t, uint32);
  result &= TEST_C_SHMEM_IGET(uint64_t, uint64);
  result &= TEST_C_SHMEM_IGET(size_t, size);
  result &= TEST_C_SHMEM_IGET(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_iget", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  /* Test SIZE-specific variants */
  int result_size = true;
  result_size &= TEST_C_SHMEM_IGETSIZE(8);
  result_size &= TEST_C_SHMEM_IGETSIZE(16);
  result_size &= TEST_C_SHMEM_IGETSIZE(32);
  result_size &= TEST_C_SHMEM_IGETSIZE(64);
/* 128-bit operations may not be supported on all platforms */
#if defined(HAVE_FEATURE_PSHMEM) && defined(SHMEM_HAVE_EXTENDEDTYPES)
  result_size &= TEST_C_SHMEM_IGETSIZE(128);
#endif

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_iget<size>", result_size, false);
  }

  if (!result_size) {
    rc = EXIT_FAILURE;
  }

  /* Test context-specific shmem_iget variants */
  int result_ctx = true;

  result_ctx &= TEST_C_CTX_SHMEM_IGET(float, float);
  result_ctx &= TEST_C_CTX_SHMEM_IGET(double, double);
  result_ctx &= TEST_C_CTX_SHMEM_IGET(long double, longdouble);
  result_ctx &= TEST_C_CTX_SHMEM_IGET(char, char);
  result_ctx &= TEST_C_CTX_SHMEM_IGET(signed char, schar);
  result_ctx &= TEST_C_CTX_SHMEM_IGET(short, short);
  result_ctx &= TEST_C_CTX_SHMEM_IGET(int, int);
  result_ctx &= TEST_C_CTX_SHMEM_IGET(long, long);
  result_ctx &= TEST_C_CTX_SHMEM_IGET(long long, longlong);
  result_ctx &= TEST_C_CTX_SHMEM_IGET(unsigned char, uchar);
  result_ctx &= TEST_C_CTX_SHMEM_IGET(unsigned short, ushort);
  result_ctx &= TEST_C_CTX_SHMEM_IGET(unsigned int, uint);
  result_ctx &= TEST_C_CTX_SHMEM_IGET(unsigned long, ulong);
  result_ctx &= TEST_C_CTX_SHMEM_IGET(unsigned long long, ulonglong);
  result_ctx &= TEST_C_CTX_SHMEM_IGET(int8_t, int8);
  result_ctx &= TEST_C_CTX_SHMEM_IGET(int16_t, int16);
  result_ctx &= TEST_C_CTX_SHMEM_IGET(int32_t, int32);
  result_ctx &= TEST_C_CTX_SHMEM_IGET(int64_t, int64);
  result_ctx &= TEST_C_CTX_SHMEM_IGET(uint8_t, uint8);
  result_ctx &= TEST_C_CTX_SHMEM_IGET(uint16_t, uint16);
  result_ctx &= TEST_C_CTX_SHMEM_IGET(uint32_t, uint32);
  result_ctx &= TEST_C_CTX_SHMEM_IGET(uint64_t, uint64);
  result_ctx &= TEST_C_CTX_SHMEM_IGET(size_t, size);
  result_ctx &= TEST_C_CTX_SHMEM_IGET(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_ctx_iget", result_ctx, false);
  }

  if (!result_ctx) {
    rc = EXIT_FAILURE;
  }

  /* Test context SIZE-specific variants */
  int result_ctx_size = true;
  result_ctx_size &= TEST_C_CTX_SHMEM_IGETSIZE(8);
  result_ctx_size &= TEST_C_CTX_SHMEM_IGETSIZE(16);
  result_ctx_size &= TEST_C_CTX_SHMEM_IGETSIZE(32);
  result_ctx_size &= TEST_C_CTX_SHMEM_IGETSIZE(64);
/* 128-bit operations may not be supported on all platforms */
#if defined(HAVE_FEATURE_PSHMEM) && defined(SHMEM_HAVE_EXTENDEDTYPES)
  result_ctx_size &= TEST_C_CTX_SHMEM_IGETSIZE(128);
#endif

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_ctx_iget<size>", result_ctx_size, false);
  }

  if (!result_ctx_size) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
