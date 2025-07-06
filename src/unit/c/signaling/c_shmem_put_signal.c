/**
 * @file c_shmem_put_signal.c
 * @brief Unit test for the shmem_put_signal() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C_SHMEM_PUT_SIGNAL(TYPE, TYPENAME)                                \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_put_signal()");                           \
    bool success = true;                                                       \
    TYPE *dest = (TYPE *)shmem_malloc(sizeof(TYPE));                           \
    TYPE *value = (TYPE *)shmem_malloc(sizeof(TYPE));                          \
    uint64_t *signal = (uint64_t *)shmem_malloc(sizeof(uint64_t));             \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
                                                                               \
    if (!dest || !value || !signal) {                                          \
      log_fail("Failed to allocate symmetric memory");                         \
      success = false;                                                         \
    } else if (npes < 2) {                                                     \
      log_fail("Test requires at least 2 PEs, but only %d PE(s) available",    \
               npes);                                                          \
      success = false;                                                         \
    } else {                                                                   \
      int target_pe = 1; /* Simple: PE 0 sends to PE 1 */                      \
      log_info("dest @ %p, value @ %p, signal @ %p", dest, value, signal);     \
                                                                               \
      /* Initialize values */                                                  \
      *dest = 0;                                                               \
      *value = (TYPE)123;                                                      \
      *signal = 0;                                                             \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 0) {                                                         \
        log_info("calling shmem_" #TYPENAME "_put_signal(dest=%p, value=%p, "  \
                 "signal=%p, target_pe=%d)",                                   \
                 dest, value, signal, target_pe);                              \
        shmem_##TYPENAME##_put_signal(dest, value, 1, signal, 1,               \
                                      SHMEM_SIGNAL_SET, target_pe);            \
      }                                                                        \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 1) {                                                         \
        /* Wait for signal to be set */                                        \
        shmem_uint64_wait_until(signal, SHMEM_CMP_EQ, 1);                      \
        if (*dest != 123 || *signal != 1) {                                    \
          log_fail("validation failed: dest = %d (expected 123), "             \
                   "signal = %d (expected 1)",                                 \
                   (int)*dest, (int)*signal);                                  \
          success = false;                                                     \
        } else {                                                               \
          log_info("result is valid");                                         \
        }                                                                      \
      }                                                                        \
    }                                                                          \
                                                                               \
    if (dest)                                                                  \
      shmem_free(dest);                                                        \
    if (value)                                                                 \
      shmem_free(value);                                                       \
    if (signal)                                                                \
      shmem_free(signal);                                                      \
    success;                                                                   \
  })

/* Test for SIZE-specific variants */
#define TEST_C_SHMEM_PUTSIZE_SIGNAL(SIZE)                                      \
  ({                                                                           \
    log_routine("shmem_put" #SIZE "_signal()");                                \
    bool success = true;                                                       \
    uint##SIZE##_t *dest =                                                     \
        (uint##SIZE##_t *)shmem_malloc(sizeof(uint##SIZE##_t));                \
    uint##SIZE##_t *value =                                                    \
        (uint##SIZE##_t *)shmem_malloc(sizeof(uint##SIZE##_t));                \
    uint64_t *signal = (uint64_t *)shmem_malloc(sizeof(uint64_t));             \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
                                                                               \
    if (!dest || !value || !signal) {                                          \
      log_fail("Failed to allocate symmetric memory");                         \
      success = false;                                                         \
    } else if (npes < 2) {                                                     \
      log_fail("Test requires at least 2 PEs, but only %d PE(s) available",    \
               npes);                                                          \
      success = false;                                                         \
    } else {                                                                   \
      int target_pe = 1; /* Simple: PE 0 sends to PE 1 */                      \
      log_info("SIZE=%d: dest @ %p, value @ %p, signal @ %p", SIZE, dest,      \
               value, signal);                                                 \
                                                                               \
      /* Initialize values */                                                  \
      *dest = 0;                                                               \
      *value = 123;                                                            \
      *signal = 0;                                                             \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 0) {                                                         \
        log_info("calling shmem_put" #SIZE "_signal(dest=%p, value=%p, "       \
                 "signal=%p, target_pe=%d)",                                   \
                 dest, value, signal, target_pe);                              \
        shmem_put##SIZE##_signal(dest, value, 1, signal, 1, SHMEM_SIGNAL_SET,  \
                                 target_pe);                                   \
      }                                                                        \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 1) {                                                         \
        /* Wait for signal to be set */                                        \
        shmem_uint64_wait_until(signal, SHMEM_CMP_EQ, 1);                      \
        if (*dest != 123 || *signal != 1) {                                    \
          log_fail("validation failed: dest = %d (expected 123), "             \
                   "signal = %d (expected 1)",                                 \
                   (int)*dest, (int)*signal);                                  \
          success = false;                                                     \
        } else {                                                               \
          log_info("result is valid");                                         \
        }                                                                      \
      }                                                                        \
    }                                                                          \
                                                                               \
    if (dest)                                                                  \
      shmem_free(dest);                                                        \
    if (value)                                                                 \
      shmem_free(value);                                                       \
    if (signal)                                                                \
      shmem_free(signal);                                                      \
    success;                                                                   \
  })

/* Test for memory-specific variant */
#define TEST_C_SHMEM_PUTMEM_SIGNAL()                                           \
  ({                                                                           \
    log_routine("shmem_putmem_signal()");                                      \
    bool success = true;                                                       \
    char *dest = (char *)shmem_malloc(10);                                     \
    char *value = (char *)shmem_malloc(10);                                    \
    uint64_t *signal = (uint64_t *)shmem_malloc(sizeof(uint64_t));             \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
                                                                               \
    if (!dest || !value || !signal) {                                          \
      log_fail("Failed to allocate symmetric memory");                         \
      success = false;                                                         \
    } else if (npes < 2) {                                                     \
      log_fail("Test requires at least 2 PEs, but only %d PE(s) available",    \
               npes);                                                          \
      success = false;                                                         \
    } else {                                                                   \
      int target_pe = 1; /* Simple: PE 0 sends to PE 1 */                      \
      log_info("MEM: dest @ %p, value @ %p, signal @ %p", dest, value,         \
               signal);                                                        \
                                                                               \
      /* Initialize values */                                                  \
      memset(dest, 0, 10);                                                     \
      strcpy(value, "Hello");                                                  \
      *signal = 0;                                                             \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 0) {                                                         \
        log_info("calling shmem_putmem_signal(dest=%p, value=%p, nelems=6, "   \
                 "signal=%p, target_pe=%d)",                                   \
                 dest, value, signal, target_pe);                              \
        shmem_putmem_signal(dest, value, 6, signal, 1, SHMEM_SIGNAL_SET,       \
                            target_pe);                                        \
      }                                                                        \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 1) {                                                         \
        /* Wait for signal to be set */                                        \
        shmem_uint64_wait_until(signal, SHMEM_CMP_EQ, 1);                      \
        if (strcmp(dest, "Hello") != 0 || *signal != 1) {                      \
          log_fail("validation failed: dest = '%s' (expected 'Hello'), "       \
                   "signal = %d (expected 1)",                                 \
                   dest, (int)*signal);                                        \
          success = false;                                                     \
        } else {                                                               \
          log_info("result is valid");                                         \
        }                                                                      \
      }                                                                        \
    }                                                                          \
                                                                               \
    if (dest)                                                                  \
      shmem_free(dest);                                                        \
    if (value)                                                                 \
      shmem_free(value);                                                       \
    if (signal)                                                                \
      shmem_free(signal);                                                      \
    success;                                                                   \
  })

/* Test for context-specific SIZE variants */
#define TEST_C_CTX_SHMEM_PUTSIZE_SIGNAL(SIZE)                                  \
  ({                                                                           \
    log_routine("shmem_ctx_put" #SIZE "_signal()");                            \
    bool success = true;                                                       \
    uint##SIZE##_t *dest =                                                     \
        (uint##SIZE##_t *)shmem_malloc(sizeof(uint##SIZE##_t));                \
    uint##SIZE##_t *value =                                                    \
        (uint##SIZE##_t *)shmem_malloc(sizeof(uint##SIZE##_t));                \
    uint64_t *signal = (uint64_t *)shmem_malloc(sizeof(uint64_t));             \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
                                                                               \
    if (!dest || !value || !signal) {                                          \
      log_fail("Failed to allocate symmetric memory");                         \
      success = false;                                                         \
    } else if (npes < 2) {                                                     \
      log_fail("Test requires at least 2 PEs, but only %d PE(s) available",    \
               npes);                                                          \
      success = false;                                                         \
    } else {                                                                   \
      int target_pe = 1; /* Simple: PE 0 sends to PE 1 */                      \
      log_info("CTX SIZE=%d: dest @ %p, value @ %p, signal @ %p", SIZE, dest,  \
               value, signal);                                                 \
                                                                               \
      /* Initialize values */                                                  \
      *dest = 0;                                                               \
      /* Choose an appropriate value based on SIZE */                          \
      *value = (SIZE == 8) ? 200 : (SIZE == 16) ? 456 : 456;                   \
      *signal = 0;                                                             \
                                                                               \
      shmem_ctx_t ctx;                                                         \
      int ctx_create_status = shmem_ctx_create(0, &ctx);                       \
                                                                               \
      if (ctx_create_status != 0) {                                            \
        log_fail("Failed to create context");                                  \
        success = false;                                                       \
      } else {                                                                 \
        log_info("Successfully created context");                              \
                                                                               \
        shmem_barrier_all();                                                   \
                                                                               \
        if (mype == 0) {                                                       \
          log_info("calling shmem_ctx_put" #SIZE "_signal(ctx, dest=%p, "      \
                   "value=%p, signal=%p, target_pe=%d)",                       \
                   dest, value, signal, target_pe);                            \
          shmem_ctx_put##SIZE##_signal(ctx, dest, value, 1, signal, 1,         \
                                       SHMEM_SIGNAL_SET, target_pe);           \
        }                                                                      \
                                                                               \
        shmem_barrier_all();                                                   \
                                                                               \
        if (mype == 1) {                                                       \
          /* Wait for signal to be set */                                      \
          shmem_uint64_wait_until(signal, SHMEM_CMP_EQ, 1);                    \
          /* Use the appropriate expected value based on SIZE */               \
          uint##SIZE##_t expected = (SIZE == 8)    ? 200                       \
                                    : (SIZE == 16) ? 456                       \
                                                   : 456;                      \
          if (*dest != expected || *signal != 1) {                             \
            log_fail("validation failed: dest = %d (expected %d), "            \
                     "signal = %d (expected 1)",                               \
                     (int)*dest, (int)expected, (int)*signal);                 \
            success = false;                                                   \
          } else {                                                             \
            log_info("result is valid");                                       \
          }                                                                    \
        }                                                                      \
                                                                               \
        shmem_ctx_destroy(ctx);                                                \
        log_info("Context destroyed");                                         \
      }                                                                        \
    }                                                                          \
                                                                               \
    if (dest)                                                                  \
      shmem_free(dest);                                                        \
    if (value)                                                                 \
      shmem_free(value);                                                       \
    if (signal)                                                                \
      shmem_free(signal);                                                      \
    success;                                                                   \
  })

/* Test for context-specific memory variant */
#define TEST_C_CTX_SHMEM_PUTMEM_SIGNAL()                                       \
  ({                                                                           \
    log_routine("shmem_ctx_putmem_signal()");                                  \
    bool success = true;                                                       \
    char *dest = (char *)shmem_malloc(10);                                     \
    char *value = (char *)shmem_malloc(10);                                    \
    uint64_t *signal = (uint64_t *)shmem_malloc(sizeof(uint64_t));             \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
                                                                               \
    if (!dest || !value || !signal) {                                          \
      log_fail("Failed to allocate symmetric memory");                         \
      success = false;                                                         \
    } else if (npes < 2) {                                                     \
      log_fail("Test requires at least 2 PEs, but only %d PE(s) available",    \
               npes);                                                          \
      success = false;                                                         \
    } else {                                                                   \
      int target_pe = 1; /* Simple: PE 0 sends to PE 1 */                      \
      log_info("CTX MEM: dest @ %p, value @ %p, signal @ %p", dest, value,     \
               signal);                                                        \
                                                                               \
      /* Initialize values */                                                  \
      memset(dest, 0, 10);                                                     \
      strcpy(value, "World");                                                  \
      *signal = 0;                                                             \
                                                                               \
      shmem_ctx_t ctx;                                                         \
      int ctx_create_status = shmem_ctx_create(0, &ctx);                       \
                                                                               \
      if (ctx_create_status != 0) {                                            \
        log_fail("Failed to create context");                                  \
        success = false;                                                       \
      } else {                                                                 \
        log_info("Successfully created context");                              \
                                                                               \
        shmem_barrier_all();                                                   \
                                                                               \
        if (mype == 0) {                                                       \
          log_info("calling shmem_ctx_putmem_signal(ctx, dest=%p, value=%p, "  \
                   "nelems=6, signal=%p, target_pe=%d)",                       \
                   dest, value, signal, target_pe);                            \
          shmem_ctx_putmem_signal(ctx, dest, value, 6, signal, 1,              \
                                  SHMEM_SIGNAL_SET, target_pe);                \
        }                                                                      \
                                                                               \
        shmem_barrier_all();                                                   \
                                                                               \
        if (mype == 1) {                                                       \
          /* Wait for signal to be set */                                      \
          shmem_uint64_wait_until(signal, SHMEM_CMP_EQ, 1);                    \
          if (strcmp(dest, "World") != 0 || *signal != 1) {                    \
            log_fail("validation failed: dest = '%s' (expected 'World'), "     \
                     "signal = %d (expected 1)",                               \
                     dest, (int)*signal);                                      \
            success = false;                                                   \
          } else {                                                             \
            log_info("result is valid");                                       \
          }                                                                    \
        }                                                                      \
                                                                               \
        shmem_ctx_destroy(ctx);                                                \
        log_info("Context destroyed");                                         \
      }                                                                        \
    }                                                                          \
                                                                               \
    if (dest)                                                                  \
      shmem_free(dest);                                                        \
    if (value)                                                                 \
      shmem_free(value);                                                       \
    if (signal)                                                                \
      shmem_free(signal);                                                      \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  int npes = shmem_n_pes();
  int mype = shmem_my_pe();

  if (!(npes >= 2)) {
    if (mype == 0) {
      display_not_enough_pes("SIGNALING");
    }
    shmem_finalize();
    return EXIT_SUCCESS;
  }

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C_SHMEM_PUT_SIGNAL(float, float);
  result &= TEST_C_SHMEM_PUT_SIGNAL(double, double);
  result &= TEST_C_SHMEM_PUT_SIGNAL(long double, longdouble);
  result &= TEST_C_SHMEM_PUT_SIGNAL(char, char);
  result &= TEST_C_SHMEM_PUT_SIGNAL(signed char, schar);
  result &= TEST_C_SHMEM_PUT_SIGNAL(short, short);
  result &= TEST_C_SHMEM_PUT_SIGNAL(int, int);
  result &= TEST_C_SHMEM_PUT_SIGNAL(long, long);
  result &= TEST_C_SHMEM_PUT_SIGNAL(long long, longlong);
  result &= TEST_C_SHMEM_PUT_SIGNAL(unsigned char, uchar);
  result &= TEST_C_SHMEM_PUT_SIGNAL(unsigned short, ushort);
  result &= TEST_C_SHMEM_PUT_SIGNAL(unsigned int, uint);
  result &= TEST_C_SHMEM_PUT_SIGNAL(unsigned long, ulong);
  result &= TEST_C_SHMEM_PUT_SIGNAL(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_PUT_SIGNAL(int8_t, int8);
  result &= TEST_C_SHMEM_PUT_SIGNAL(int16_t, int16);
  result &= TEST_C_SHMEM_PUT_SIGNAL(int32_t, int32);
  result &= TEST_C_SHMEM_PUT_SIGNAL(int64_t, int64);
  result &= TEST_C_SHMEM_PUT_SIGNAL(uint8_t, uint8);
  result &= TEST_C_SHMEM_PUT_SIGNAL(uint16_t, uint16);
  result &= TEST_C_SHMEM_PUT_SIGNAL(uint32_t, uint32);
  result &= TEST_C_SHMEM_PUT_SIGNAL(uint64_t, uint64);
  result &= TEST_C_SHMEM_PUT_SIGNAL(size_t, size);
  result &= TEST_C_SHMEM_PUT_SIGNAL(ptrdiff_t, ptrdiff);

  if (!result) {
    rc = EXIT_FAILURE;
  }

  if (mype == 0) {
    display_test_result("C shmem_put_signal", result, false);
  }

  /* Test SIZE-specific variants */
  int result_size = true;
  result_size &= TEST_C_SHMEM_PUTSIZE_SIGNAL(8);
  result_size &= TEST_C_SHMEM_PUTSIZE_SIGNAL(16);
  result_size &= TEST_C_SHMEM_PUTSIZE_SIGNAL(32);
  result_size &= TEST_C_SHMEM_PUTSIZE_SIGNAL(64);
/* 128-bit operations may not be supported on all platforms */
#if defined(HAVE_FEATURE_PSHMEM) && defined(SHMEM_HAVE_EXTENDEDTYPES)
  result_size &= TEST_C_SHMEM_PUTSIZE_SIGNAL(128);
#endif

  if (!result_size) {
    rc = EXIT_FAILURE;
  }

  if (mype == 0) {
    display_test_result("C shmem_put<size>_signal", result_size, false);
  }

  /* Test memory-specific variant */
  int result_mem = TEST_C_SHMEM_PUTMEM_SIGNAL();

  if (!result_mem) {
    rc = EXIT_FAILURE;
  }

  if (mype == 0) {
    display_test_result("C shmem_putmem_signal", result_mem, false);
  }

  /* Test context-specific SIZE variants */
  int result_ctx_size = true;
  result_ctx_size &= TEST_C_CTX_SHMEM_PUTSIZE_SIGNAL(8);
  result_ctx_size &= TEST_C_CTX_SHMEM_PUTSIZE_SIGNAL(16);
  result_ctx_size &= TEST_C_CTX_SHMEM_PUTSIZE_SIGNAL(32);
  result_ctx_size &= TEST_C_CTX_SHMEM_PUTSIZE_SIGNAL(64);
/* 128-bit operations may not be supported on all platforms */
#if defined(HAVE_FEATURE_PSHMEM) && defined(SHMEM_HAVE_EXTENDEDTYPES)
  result_ctx_size &= TEST_C_CTX_SHMEM_PUTSIZE_SIGNAL(128);
#endif

  if (!result_ctx_size) {
    rc = EXIT_FAILURE;
  }

  if (mype == 0) {
    display_test_result("C shmem_ctx_put<size>_signal", result_ctx_size, false);
  }

  /* Test context-specific memory variant */
  int result_ctx_mem = TEST_C_CTX_SHMEM_PUTMEM_SIGNAL();

  if (!result_ctx_mem) {
    rc = EXIT_FAILURE;
  }

  if (mype == 0) {
    display_test_result("C shmem_ctx_putmem_signal", result_ctx_mem, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}