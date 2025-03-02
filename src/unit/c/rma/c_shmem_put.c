/**
 * @file c_shmem_put.c
 * @brief Unit test for the shmem_put() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C_SHMEM_PUT(TYPE, TYPENAME)                                       \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_put()");                                  \
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
      log_info("PE 0: Starting put operation to PE 1");                        \
      log_info("PE 0: dest=%p, src=%p, nelems=10", (void *)dest, (void *)src); \
      shmem_##TYPENAME##_put(dest, src, 10, 1);                                \
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

#define TEST_C_CTX_SHMEM_PUT(TYPE, TYPENAME)                                   \
  ({                                                                           \
    log_routine("shmem_ctx_" #TYPENAME "_put()");                              \
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
      log_info("PE 0: Starting context-based put to PE 1");                    \
      log_info("PE 0: dest=%p, src=%p, nelems=10", (void *)dest, (void *)src); \
      shmem_ctx_##TYPENAME##_put(ctx, dest, src, 10, 1);                       \
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

/* Test for SIZE-specific variants */
#define TEST_C_SHMEM_PUTSIZE(SIZE)                                             \
  ({                                                                           \
    log_routine("shmem_put" #SIZE "()");                                       \
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
      log_info("PE %d: Initialized src[%d] = %d", mype, i, i + mype);          \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 0) {                                                           \
      log_info("PE 0: Starting SIZE-specific put to PE 1");                    \
      log_info("PE 0: dest=%p, src=%p, nelems=10", (void *)dest, (void *)src); \
      shmem_put##SIZE(dest, src, 10, 1);                                       \
      log_info("PE 0: Completed SIZE-specific put operation");                 \
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

/* Test for context SIZE-specific variants */
#define TEST_C_CTX_SHMEM_PUTSIZE(SIZE)                                         \
  ({                                                                           \
    log_routine("shmem_ctx_put" #SIZE "()");                                   \
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
      log_info("PE %d: Initialized src[%d] = %d", mype, i, i + 20 + mype);     \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 0) {                                                           \
      log_info("PE 0: Starting context-based SIZE-specific put to PE 1");      \
      log_info("PE 0: dest=%p, src=%p, nelems=10", (void *)dest, (void *)src); \
      shmem_ctx_put##SIZE(ctx, dest, src, 10, 1);                              \
      log_info("PE 0: Completed context-based SIZE-specific put operation");   \
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

/* Test for memory-specific variant */
#define TEST_C_SHMEM_PUTMEM()                                                  \
  ({                                                                           \
    log_routine("shmem_putmem()");                                             \
    bool success = true;                                                       \
    static char src[10], dest[10];                                             \
    log_info("Allocated static arrays: src at %p, dest at %p", (void *)&src,   \
             (void *)&dest);                                                   \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    log_info("Running on PE %d of %d total PEs", mype, npes);                  \
                                                                               \
    for (int i = 0; i < 10; i++) {                                             \
      src[i] = 'a' + i + mype;                                                 \
      log_info("PE %d: Initialized src[%d] = '%c'", mype, i, src[i]);          \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 0) {                                                           \
      log_info("PE 0: Starting memory-specific put to PE 1");                  \
      log_info("PE 0: dest=%p, src=%p, nelems=10", (void *)dest, (void *)src); \
      shmem_putmem(dest, src, 10, 1);                                          \
      log_info("PE 0: Completed memory-specific put operation");               \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Beginning validation of received data");                 \
      for (int i = 0; i < 10; i++) {                                           \
        char expected = 'a' + i;                                               \
        if (dest[i] != expected) {                                             \
          log_fail("PE 1: Validation failed - dest[%d] = '%c', expected '%c'", \
                   i, dest[i], expected);                                      \
          success = false;                                                     \
          break;                                                               \
        }                                                                      \
        log_info("PE 1: dest[%d] = '%c' (valid)", i, dest[i]);                 \
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

/* Test for context memory-specific variant */
#define TEST_C_CTX_SHMEM_PUTMEM()                                              \
  ({                                                                           \
    log_routine("shmem_ctx_putmem()");                                         \
    bool success = true;                                                       \
    static char src[10], dest[10];                                             \
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
      src[i] = 'A' + i + mype; /* Different values (uppercase) for ctx test */ \
      log_info("PE %d: Initialized src[%d] = '%c'", mype, i, src[i]);          \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 0) {                                                           \
      log_info("PE 0: Starting context memory-specific put to PE 1");          \
      log_info("PE 0: dest=%p, src=%p, nelems=10", (void *)dest, (void *)src); \
      shmem_ctx_putmem(ctx, dest, src, 10, 1);                                 \
      log_info("PE 0: Completed context memory-specific put operation");       \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Beginning validation of received data");                 \
      for (int i = 0; i < 10; i++) {                                           \
        char expected = 'A' + i; /* PE 0's value */                            \
        if (dest[i] != expected) {                                             \
          log_fail("PE 1: Validation failed - dest[%d] = '%c', expected '%c'", \
                   i, dest[i], expected);                                      \
          success = false;                                                     \
          break;                                                               \
        }                                                                      \
        log_info("PE 1: dest[%d] = '%c' (valid)", i, dest[i]);                 \
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
    if (shmem_my_pe() == 0) {
      display_not_enough_pes("RMA");
    }
    shmem_finalize();
    return EXIT_SUCCESS;
  }

  int result = true;
  int rc = EXIT_SUCCESS;

  /* Test standard shmem_put variants */
  result &= TEST_C_SHMEM_PUT(float, float);
  result &= TEST_C_SHMEM_PUT(double, double);
  result &= TEST_C_SHMEM_PUT(long double, longdouble);
  result &= TEST_C_SHMEM_PUT(char, char);
  result &= TEST_C_SHMEM_PUT(signed char, schar);
  result &= TEST_C_SHMEM_PUT(short, short);
  result &= TEST_C_SHMEM_PUT(int, int);
  result &= TEST_C_SHMEM_PUT(long, long);
  result &= TEST_C_SHMEM_PUT(long long, longlong);
  result &= TEST_C_SHMEM_PUT(unsigned char, uchar);
  result &= TEST_C_SHMEM_PUT(unsigned short, ushort);
  result &= TEST_C_SHMEM_PUT(unsigned int, uint);
  result &= TEST_C_SHMEM_PUT(unsigned long, ulong);
  result &= TEST_C_SHMEM_PUT(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_PUT(int8_t, int8);
  result &= TEST_C_SHMEM_PUT(int16_t, int16);
  result &= TEST_C_SHMEM_PUT(int32_t, int32);
  result &= TEST_C_SHMEM_PUT(int64_t, int64);
  result &= TEST_C_SHMEM_PUT(uint8_t, uint8);
  result &= TEST_C_SHMEM_PUT(uint16_t, uint16);
  result &= TEST_C_SHMEM_PUT(uint32_t, uint32);
  result &= TEST_C_SHMEM_PUT(uint64_t, uint64);
  result &= TEST_C_SHMEM_PUT(size_t, size);
  result &= TEST_C_SHMEM_PUT(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_put", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  /* Test SIZE-specific variants */
  int result_size = true;
  result_size &= TEST_C_SHMEM_PUTSIZE(8);
  result_size &= TEST_C_SHMEM_PUTSIZE(16);
  result_size &= TEST_C_SHMEM_PUTSIZE(32);
  result_size &= TEST_C_SHMEM_PUTSIZE(64);
/* 128-bit operations may not be supported on all platforms */
#if defined(HAVE_FEATURE_PSHMEM) && defined(SHMEM_HAVE_EXTENDEDTYPES)
  result_size &= TEST_C_SHMEM_PUTSIZE(128);
#endif

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_put<size>", result_size, false);
  }

  if (!result_size) {
    rc = EXIT_FAILURE;
  }

  /* Test memory-specific variant */
  int result_mem = TEST_C_SHMEM_PUTMEM();

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_putmem", result_mem, false);
  }

  if (!result_mem) {
    rc = EXIT_FAILURE;
  }

  /* Test context-specific shmem_put variants */
  int result_ctx = true;

  result_ctx &= TEST_C_CTX_SHMEM_PUT(float, float);
  result_ctx &= TEST_C_CTX_SHMEM_PUT(double, double);
  result_ctx &= TEST_C_CTX_SHMEM_PUT(long double, longdouble);
  result_ctx &= TEST_C_CTX_SHMEM_PUT(char, char);
  result_ctx &= TEST_C_CTX_SHMEM_PUT(signed char, schar);
  result_ctx &= TEST_C_CTX_SHMEM_PUT(short, short);
  result_ctx &= TEST_C_CTX_SHMEM_PUT(int, int);
  result_ctx &= TEST_C_CTX_SHMEM_PUT(long, long);
  result_ctx &= TEST_C_CTX_SHMEM_PUT(long long, longlong);
  result_ctx &= TEST_C_CTX_SHMEM_PUT(unsigned char, uchar);
  result_ctx &= TEST_C_CTX_SHMEM_PUT(unsigned short, ushort);
  result_ctx &= TEST_C_CTX_SHMEM_PUT(unsigned int, uint);
  result_ctx &= TEST_C_CTX_SHMEM_PUT(unsigned long, ulong);
  result_ctx &= TEST_C_CTX_SHMEM_PUT(unsigned long long, ulonglong);
  result_ctx &= TEST_C_CTX_SHMEM_PUT(int8_t, int8);
  result_ctx &= TEST_C_CTX_SHMEM_PUT(int16_t, int16);
  result_ctx &= TEST_C_CTX_SHMEM_PUT(int32_t, int32);
  result_ctx &= TEST_C_CTX_SHMEM_PUT(int64_t, int64);
  result_ctx &= TEST_C_CTX_SHMEM_PUT(uint8_t, uint8);
  result_ctx &= TEST_C_CTX_SHMEM_PUT(uint16_t, uint16);
  result_ctx &= TEST_C_CTX_SHMEM_PUT(uint32_t, uint32);
  result_ctx &= TEST_C_CTX_SHMEM_PUT(uint64_t, uint64);
  result_ctx &= TEST_C_CTX_SHMEM_PUT(size_t, size);
  result_ctx &= TEST_C_CTX_SHMEM_PUT(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (!result_ctx) {
    rc = EXIT_FAILURE;
  }

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_ctx_put", result_ctx, false);
  }

  /* Test context SIZE-specific variants */
  int result_ctx_size = true;
  result_ctx_size &= TEST_C_CTX_SHMEM_PUTSIZE(8);
  result_ctx_size &= TEST_C_CTX_SHMEM_PUTSIZE(16);
  result_ctx_size &= TEST_C_CTX_SHMEM_PUTSIZE(32);
  result_ctx_size &= TEST_C_CTX_SHMEM_PUTSIZE(64);
/* 128-bit operations may not be supported on all platforms */
#if defined(HAVE_FEATURE_PSHMEM) && defined(SHMEM_HAVE_EXTENDEDTYPES)
  result_ctx_size &= TEST_C_CTX_SHMEM_PUTSIZE(128);
#endif

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_ctx_put<size>", result_ctx_size, false);
  }

  if (!result_ctx_size) {
    rc = EXIT_FAILURE;
  }

  /* Test context memory-specific variant */
  int result_ctx_mem = TEST_C_CTX_SHMEM_PUTMEM();

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_ctx_putmem", result_ctx_mem, false);
  }

  if (!result_ctx_mem) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
