/**
 * @file c_shmem_get.c
 * @brief Unit test for the shmem_get() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C_SHMEM_GET(TYPE, TYPENAME)                                       \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_get()");                                  \
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
      log_info("PE 1: Starting get of 10 elements from PE 0");                 \
      shmem_##TYPENAME##_get(dest, src, 10, 0);                                \
      log_info("PE 1: Completed get operation");                               \
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

#define TEST_C_CTX_SHMEM_GET(TYPE, TYPENAME)                                   \
  ({                                                                           \
    log_routine("shmem_ctx_" #TYPENAME "_get()");                              \
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
      log_info("PE 1: Starting context-based get of 10 elements from PE 0");   \
      shmem_ctx_##TYPENAME##_get(ctx, dest, src, 10, 0);                       \
      log_info("PE 1: Completed context-based get operation");                 \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Beginning validation of received data");                 \
      for (int i = 0; i < 10; i++) {                                           \
        if (dest[i] != i + 20) {                                               \
          log_fail("PE 1: Validation failed - dest[%d] = %d, expected %d", i,  \
                   (int)dest[i], i + 20);                                      \
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
#define TEST_C_SHMEM_GETSIZE(SIZE)                                             \
  ({                                                                           \
    log_routine("shmem_get" #SIZE "()");                                       \
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
      }                                                                        \
      log_info("PE 0: Initialized src array with values [0..9]");              \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Starting get of 10 elements from PE 0");                 \
      shmem_get##SIZE(dest, src, 10, 0);                                       \
      log_info("PE 1: Completed get operation");                               \
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
#define TEST_C_CTX_SHMEM_GETSIZE(SIZE)                                         \
  ({                                                                           \
    log_routine("shmem_ctx_get" #SIZE "()");                                   \
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
      }                                                                        \
      log_info("PE 0: Initialized src array with values [20..29]");            \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Starting context-based get of 10 elements from PE 0");   \
      shmem_ctx_get##SIZE(ctx, dest, src, 10, 0);                              \
      log_info("PE 1: Completed context-based get operation");                 \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Beginning validation of received data");                 \
      for (int i = 0; i < 10; i++) {                                           \
        if (dest[i] != i + 20) {                                               \
          log_fail("PE 1: Validation failed - dest[%d] = %d, expected %d", i,  \
                   (int)dest[i], i + 20);                                      \
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

/* Test for memory-specific variant */
#define TEST_C_SHMEM_GETMEM()                                                  \
  ({                                                                           \
    log_routine("shmem_getmem()");                                             \
    bool success = true;                                                       \
    static char src[10], dest[10];                                             \
    log_info("Allocated static arrays: src at %p, dest at %p", (void *)&src,   \
             (void *)&dest);                                                   \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    log_info("Running on PE %d of %d total PEs", mype, npes);                  \
                                                                               \
    if (mype == 0) {                                                           \
      for (int i = 0; i < 10; i++) {                                           \
        src[i] = 'a' + i;                                                      \
      }                                                                        \
      log_info("PE 0: Initialized src array with values ['a'..'j']");          \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Starting getmem of 10 bytes from PE 0");                 \
      shmem_getmem(dest, src, 10, 0);                                          \
      log_info("PE 1: Completed getmem operation");                            \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Beginning validation of received data");                 \
      for (int i = 0; i < 10; i++) {                                           \
        if (dest[i] != 'a' + i) {                                              \
          log_fail("PE 1: Validation failed - dest[%d] = %c, expected %c", i,  \
                   dest[i], 'a' + i);                                          \
          success = false;                                                     \
          break;                                                               \
        }                                                                      \
        log_info("PE 1: dest[%d] = %c (valid)", i, dest[i]);                   \
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

/* Test for context memory-specific variant */
#define TEST_C_CTX_SHMEM_GETMEM()                                              \
  ({                                                                           \
    log_routine("shmem_ctx_getmem()");                                         \
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
    if (mype == 0) {                                                           \
      for (int i = 0; i < 10; i++) {                                           \
        src[i] = 'A' + i; /* Different values for context test */              \
      }                                                                        \
      log_info("PE 0: Initialized src array with values ['A'..'J']");          \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Starting context-based getmem of 10 bytes from PE 0");   \
      shmem_ctx_getmem(ctx, dest, src, 10, 0);                                 \
      log_info("PE 1: Completed context-based getmem operation");              \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Beginning validation of received data");                 \
      for (int i = 0; i < 10; i++) {                                           \
        if (dest[i] != 'A' + i) {                                              \
          log_fail("PE 1: Validation failed - dest[%d] = %c, expected %c", i,  \
                   dest[i], 'A' + i);                                          \
          success = false;                                                     \
          break;                                                               \
        }                                                                      \
        log_info("PE 1: dest[%d] = %c (valid)", i, dest[i]);                   \
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

  /* Test standard shmem_get variants */
  result &= TEST_C_SHMEM_GET(float, float);
  result &= TEST_C_SHMEM_GET(double, double);
  result &= TEST_C_SHMEM_GET(long double, longdouble);
  result &= TEST_C_SHMEM_GET(char, char);
  result &= TEST_C_SHMEM_GET(signed char, schar);
  result &= TEST_C_SHMEM_GET(short, short);
  result &= TEST_C_SHMEM_GET(int, int);
  result &= TEST_C_SHMEM_GET(long, long);
  result &= TEST_C_SHMEM_GET(long long, longlong);
  result &= TEST_C_SHMEM_GET(unsigned char, uchar);
  result &= TEST_C_SHMEM_GET(unsigned short, ushort);
  result &= TEST_C_SHMEM_GET(unsigned int, uint);
  result &= TEST_C_SHMEM_GET(unsigned long, ulong);
  result &= TEST_C_SHMEM_GET(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_GET(int8_t, int8);
  result &= TEST_C_SHMEM_GET(int16_t, int16);
  result &= TEST_C_SHMEM_GET(int32_t, int32);
  result &= TEST_C_SHMEM_GET(int64_t, int64);
  result &= TEST_C_SHMEM_GET(uint8_t, uint8);
  result &= TEST_C_SHMEM_GET(uint16_t, uint16);
  result &= TEST_C_SHMEM_GET(uint32_t, uint32);
  result &= TEST_C_SHMEM_GET(uint64_t, uint64);
  result &= TEST_C_SHMEM_GET(size_t, size);
  result &= TEST_C_SHMEM_GET(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_get", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  /* Test SIZE-specific variants */
  int result_size = true;
  result_size &= TEST_C_SHMEM_GETSIZE(8);
  result_size &= TEST_C_SHMEM_GETSIZE(16);
  result_size &= TEST_C_SHMEM_GETSIZE(32);
  result_size &= TEST_C_SHMEM_GETSIZE(64);
/* 128-bit operations may not be supported on all platforms */
#if defined(HAVE_FEATURE_PSHMEM) && defined(SHMEM_HAVE_EXTENDEDTYPES)
  result_size &= TEST_C_SHMEM_GETSIZE(128);
#endif

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_get<size>", result_size, false);
  }

  if (!result_size) {
    rc = EXIT_FAILURE;
  }

  /* Test memory-specific variant */
  int result_mem = TEST_C_SHMEM_GETMEM();

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_getmem", result_mem, false);
  }

  if (!result_mem) {
    rc = EXIT_FAILURE;
  }

  /* Test context-specific shmem_get variants */
  int result_ctx = true;

  result_ctx &= TEST_C_CTX_SHMEM_GET(float, float);
  result_ctx &= TEST_C_CTX_SHMEM_GET(double, double);
  result_ctx &= TEST_C_CTX_SHMEM_GET(long double, longdouble);
  result_ctx &= TEST_C_CTX_SHMEM_GET(char, char);
  result_ctx &= TEST_C_CTX_SHMEM_GET(signed char, schar);
  result_ctx &= TEST_C_CTX_SHMEM_GET(short, short);
  result_ctx &= TEST_C_CTX_SHMEM_GET(int, int);
  result_ctx &= TEST_C_CTX_SHMEM_GET(long, long);
  result_ctx &= TEST_C_CTX_SHMEM_GET(long long, longlong);
  result_ctx &= TEST_C_CTX_SHMEM_GET(unsigned char, uchar);
  result_ctx &= TEST_C_CTX_SHMEM_GET(unsigned short, ushort);
  result_ctx &= TEST_C_CTX_SHMEM_GET(unsigned int, uint);
  result_ctx &= TEST_C_CTX_SHMEM_GET(unsigned long, ulong);
  result_ctx &= TEST_C_CTX_SHMEM_GET(unsigned long long, ulonglong);
  result_ctx &= TEST_C_CTX_SHMEM_GET(int8_t, int8);
  result_ctx &= TEST_C_CTX_SHMEM_GET(int16_t, int16);
  result_ctx &= TEST_C_CTX_SHMEM_GET(int32_t, int32);
  result_ctx &= TEST_C_CTX_SHMEM_GET(int64_t, int64);
  result_ctx &= TEST_C_CTX_SHMEM_GET(uint8_t, uint8);
  result_ctx &= TEST_C_CTX_SHMEM_GET(uint16_t, uint16);
  result_ctx &= TEST_C_CTX_SHMEM_GET(uint32_t, uint32);
  result_ctx &= TEST_C_CTX_SHMEM_GET(uint64_t, uint64);
  result_ctx &= TEST_C_CTX_SHMEM_GET(size_t, size);
  result_ctx &= TEST_C_CTX_SHMEM_GET(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_ctx_get", result_ctx, false);
  }

  if (!result_ctx) {
    rc = EXIT_FAILURE;
  }

  /* Test context SIZE-specific variants */
  int result_ctx_size = true;
  result_ctx_size &= TEST_C_CTX_SHMEM_GETSIZE(8);
  result_ctx_size &= TEST_C_CTX_SHMEM_GETSIZE(16);
  result_ctx_size &= TEST_C_CTX_SHMEM_GETSIZE(32);
  result_ctx_size &= TEST_C_CTX_SHMEM_GETSIZE(64);
/* 128-bit operations may not be supported on all platforms */
#if defined(HAVE_FEATURE_PSHMEM) && defined(SHMEM_HAVE_EXTENDEDTYPES)
  result_ctx_size &= TEST_C_CTX_SHMEM_GETSIZE(128);
#endif

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_ctx_get<size>", result_ctx_size, false);
  }

  if (!result_ctx_size) {
    rc = EXIT_FAILURE;
  }

  /* Test context memory-specific variant */
  int result_ctx_mem = TEST_C_CTX_SHMEM_GETMEM();

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_ctx_getmem", result_ctx_mem, false);
  }

  if (!result_ctx_mem) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
