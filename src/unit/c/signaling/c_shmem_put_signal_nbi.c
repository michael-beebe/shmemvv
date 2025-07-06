/**
 * @file c_shmem_put_signal_nbi.c
 * @brief Unit test for the shmem_put_signal_nbi() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C_SHMEM_PUT_SIGNAL_NBI(TYPE, TYPENAME)                            \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_put_signal_nbi");                         \
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
        log_info("PE 0: Broadcasting to all PEs using put_signal_nbi");        \
        for (int i = 0; i < npes; i++) {                                       \
          log_info("calling shmem_" #TYPENAME "_put_signal_nbi to PE %d", i);  \
          shmem_##TYPENAME##_put_signal_nbi(dest, value, 1, signal, 1,         \
                                            SHMEM_SIGNAL_SET, i);              \
        }                                                                      \
      }                                                                        \
                                                                               \
      /* Each PE waits for its own signal */                                   \
      shmem_uint64_wait_until(signal, SHMEM_CMP_EQ, 1);                        \
                                                                               \
      if (*dest != 123 || *signal != 1) {                                      \
        log_fail("validation failed: dest = %d (expected 123), "               \
                 "signal = %d (expected 1)",                                   \
                 (int)*dest, (int)*signal);                                    \
        success = false;                                                       \
      } else {                                                                 \
        log_info("result is valid");                                           \
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

/* Test for context TYPENAME variants */
#define TEST_C_CTX_SHMEM_PUT_SIGNAL_NBI(TYPE, TYPENAME)                        \
  ({                                                                           \
    log_routine("shmem_ctx_" #TYPENAME "_put_signal_nbi");                     \
    bool success = true;                                                       \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
                                                                               \
    if (npes < 2) {                                                            \
      log_fail("Test requires at least 2 PEs, but only %d PE(s) available",    \
               npes);                                                          \
      success = false;                                                         \
    } else {                                                                   \
      TYPE *dest = (TYPE *)shmem_malloc(sizeof(TYPE));                         \
      TYPE *value = (TYPE *)shmem_malloc(sizeof(TYPE));                        \
      uint64_t *signal = (uint64_t *)shmem_malloc(sizeof(uint64_t));           \
                                                                               \
      if (!dest || !value || !signal) {                                        \
        log_fail("Failed to allocate symmetric memory");                       \
        success = false;                                                       \
      } else {                                                                 \
        log_info("dest @ %p, value @ %p, signal @ %p", dest, value, signal);   \
                                                                               \
        /* Initialize values - use smaller value for small types */            \
        *dest = 0;                                                             \
        *value = (sizeof(TYPE) == 1) ? (TYPE)42 : (TYPE)456;                   \
        *signal = 0;                                                           \
                                                                               \
        shmem_ctx_t ctx;                                                       \
        int ctx_create_status = shmem_ctx_create(0, &ctx);                     \
                                                                               \
        if (ctx_create_status != 0) {                                          \
          log_fail("Failed to create context");                                \
          success = false;                                                     \
        } else {                                                               \
          log_info("Successfully created context");                            \
                                                                               \
          shmem_barrier_all();                                                 \
                                                                               \
          if (mype == 0) {                                                     \
            log_info(                                                          \
                "PE 0: Broadcasting to all PEs using ctx put_signal_nbi");     \
            for (int i = 0; i < npes; i++) {                                   \
              log_info("calling shmem_ctx_" #TYPENAME                          \
                       "_put_signal_nbi to PE %d",                             \
                       i);                                                     \
              shmem_ctx_##TYPENAME##_put_signal_nbi(                           \
                  ctx, dest, value, 1, signal, 1, SHMEM_SIGNAL_SET, i);        \
            }                                                                  \
          }                                                                    \
                                                                               \
          /* Each PE waits for its own signal */                               \
          shmem_uint64_wait_until(signal, SHMEM_CMP_EQ, 1);                    \
                                                                               \
          TYPE expected_value = (sizeof(TYPE) == 1) ? (TYPE)42 : (TYPE)456;    \
          if (*dest != expected_value || *signal != 1) {                       \
            log_fail("validation failed: dest = %d (expected %d), "            \
                     "signal = %d (expected 1)",                               \
                     (int)*dest, (int)expected_value, (int)*signal);           \
            success = false;                                                   \
          } else {                                                             \
            log_info("result is valid");                                       \
          }                                                                    \
                                                                               \
          shmem_ctx_destroy(ctx);                                              \
          log_info("Context destroyed");                                       \
        }                                                                      \
      }                                                                        \
                                                                               \
      if (dest)                                                                \
        shmem_free(dest);                                                      \
      if (value)                                                               \
        shmem_free(value);                                                     \
      if (signal)                                                              \
        shmem_free(signal);                                                    \
    }                                                                          \
    success;                                                                   \
  })

/* Test for SIZE-specific variants */
#define TEST_C_SHMEM_PUTSIZE_SIGNAL_NBI(SIZE)                                  \
  ({                                                                           \
    log_routine("shmem_put" #SIZE "_signal_nbi");                              \
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
        log_info("PE 0: Broadcasting to all PEs using put" #SIZE               \
                 "_signal_nbi");                                               \
        for (int i = 0; i < npes; i++) {                                       \
          log_info("calling shmem_put" #SIZE "_signal_nbi to PE %d", i);       \
          shmem_put##SIZE##_signal_nbi(dest, value, 1, signal, 1,              \
                                       SHMEM_SIGNAL_SET, i);                   \
        }                                                                      \
      }                                                                        \
                                                                               \
      /* Each PE waits for its own signal */                                   \
      shmem_uint64_wait_until(signal, SHMEM_CMP_EQ, 1);                        \
                                                                               \
      if (*dest != 123 || *signal != 1) {                                      \
        log_fail("validation failed: dest = %d (expected 123), "               \
                 "signal = %d (expected 1)",                                   \
                 (int)*dest, (int)*signal);                                    \
        success = false;                                                       \
      } else {                                                                 \
        log_info("result is valid");                                           \
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

/* Test for context SIZE-specific variants */
#define TEST_C_CTX_SHMEM_PUTSIZE_SIGNAL_NBI(SIZE)                              \
  ({                                                                           \
    log_routine("shmem_ctx_put" #SIZE "_signal_nbi");                          \
    bool success = true;                                                       \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
                                                                               \
    if (npes < 2) {                                                            \
      log_fail("Test requires at least 2 PEs, but only %d PE(s) available",    \
               npes);                                                          \
      success = false;                                                         \
    } else {                                                                   \
      uint##SIZE##_t *dest =                                                   \
          (uint##SIZE##_t *)shmem_malloc(sizeof(uint##SIZE##_t));              \
      uint##SIZE##_t *value =                                                  \
          (uint##SIZE##_t *)shmem_malloc(sizeof(uint##SIZE##_t));              \
      uint64_t *signal = (uint64_t *)shmem_malloc(sizeof(uint64_t));           \
                                                                               \
      if (!dest || !value || !signal) {                                        \
        log_fail("Failed to allocate symmetric memory");                       \
        success = false;                                                       \
      } else {                                                                 \
        log_info("CTX SIZE=%d: dest @ %p, value @ %p, signal @ %p", SIZE,      \
                 dest, value, signal);                                         \
                                                                               \
        /* Initialize values - use smaller value for small types */            \
        *dest = 0;                                                             \
        *value = (SIZE == 8) ? 42 : 456;                                       \
        *signal = 0;                                                           \
                                                                               \
        shmem_ctx_t ctx;                                                       \
        int ctx_create_status = shmem_ctx_create(0, &ctx);                     \
                                                                               \
        if (ctx_create_status != 0) {                                          \
          log_fail("Failed to create context");                                \
          success = false;                                                     \
        } else {                                                               \
          log_info("Successfully created context");                            \
                                                                               \
          shmem_barrier_all();                                                 \
                                                                               \
          if (mype == 0) {                                                     \
            log_info("PE 0: Broadcasting to all PEs using ctx put" #SIZE       \
                     "_signal_nbi");                                           \
            for (int i = 0; i < npes; i++) {                                   \
              log_info("calling shmem_ctx_put" #SIZE "_signal_nbi to PE %d",   \
                       i);                                                     \
              shmem_ctx_put##SIZE##_signal_nbi(ctx, dest, value, 1, signal, 1, \
                                               SHMEM_SIGNAL_SET, i);           \
            }                                                                  \
          }                                                                    \
                                                                               \
          /* Each PE waits for its own signal */                               \
          shmem_uint64_wait_until(signal, SHMEM_CMP_EQ, 1);                    \
                                                                               \
          uint##SIZE##_t expected = (SIZE == 8) ? 42 : 456;                    \
          if (*dest != expected || *signal != 1) {                             \
            log_fail("validation failed: dest = %d (expected %d), "            \
                     "signal = %d (expected 1)",                               \
                     (int)*dest, (int)expected, (int)*signal);                 \
            success = false;                                                   \
          } else {                                                             \
            log_info("result is valid");                                       \
          }                                                                    \
                                                                               \
          shmem_ctx_destroy(ctx);                                              \
          log_info("Context destroyed");                                       \
        }                                                                      \
      }                                                                        \
                                                                               \
      if (dest)                                                                \
        shmem_free(dest);                                                      \
      if (value)                                                               \
        shmem_free(value);                                                     \
      if (signal)                                                              \
        shmem_free(signal);                                                    \
    }                                                                          \
    success;                                                                   \
  })

/* Test for memory-specific variant */
#define TEST_C_SHMEM_PUTMEM_SIGNAL_NBI()                                       \
  ({                                                                           \
    log_routine("shmem_putmem_signal_nbi");                                    \
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
        log_info("PE 0: Broadcasting to all PEs using putmem_signal_nbi");     \
        for (int i = 0; i < npes; i++) {                                       \
          log_info("calling shmem_putmem_signal_nbi to PE %d", i);             \
          shmem_putmem_signal_nbi(dest, value, 6, signal, 1, SHMEM_SIGNAL_SET, \
                                  i);                                          \
        }                                                                      \
      }                                                                        \
                                                                               \
      /* Each PE waits for its own signal */                                   \
      shmem_uint64_wait_until(signal, SHMEM_CMP_EQ, 1);                        \
                                                                               \
      if (strcmp(dest, "Hello") != 0 || *signal != 1) {                        \
        log_fail("validation failed: dest = '%s' (expected 'Hello'), "         \
                 "signal = %d (expected 1)",                                   \
                 dest, (int)*signal);                                          \
        success = false;                                                       \
      } else {                                                                 \
        log_info("result is valid");                                           \
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

/* Test for context memory-specific variant */
#define TEST_C_CTX_SHMEM_PUTMEM_SIGNAL_NBI()                                   \
  ({                                                                           \
    log_routine("shmem_ctx_putmem_signal_nbi");                                \
    bool success = true;                                                       \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
                                                                               \
    if (npes < 2) {                                                            \
      log_fail("Test requires at least 2 PEs, but only %d PE(s) available",    \
               npes);                                                          \
      success = false;                                                         \
    } else {                                                                   \
      char *dest = (char *)shmem_malloc(10);                                   \
      char *value = (char *)shmem_malloc(10);                                  \
      uint64_t *signal = (uint64_t *)shmem_malloc(sizeof(uint64_t));           \
                                                                               \
      if (!dest || !value || !signal) {                                        \
        log_fail("Failed to allocate symmetric memory");                       \
        success = false;                                                       \
      } else {                                                                 \
        log_info("CTX MEM: dest @ %p, value @ %p, signal @ %p", dest, value,   \
                 signal);                                                      \
                                                                               \
        /* Initialize values */                                                \
        memset(dest, 0, 10);                                                   \
        strcpy(value, "World");                                                \
        *signal = 0;                                                           \
                                                                               \
        shmem_ctx_t ctx;                                                       \
        int ctx_create_status = shmem_ctx_create(0, &ctx);                     \
                                                                               \
        if (ctx_create_status != 0) {                                          \
          log_fail("Failed to create context");                                \
          success = false;                                                     \
        } else {                                                               \
          log_info("Successfully created context");                            \
                                                                               \
          shmem_barrier_all();                                                 \
                                                                               \
          if (mype == 0) {                                                     \
            log_info(                                                          \
                "PE 0: Broadcasting to all PEs using ctx putmem_signal_nbi");  \
            for (int i = 0; i < npes; i++) {                                   \
              log_info("calling shmem_ctx_putmem_signal_nbi to PE %d", i);     \
              shmem_ctx_putmem_signal_nbi(ctx, dest, value, 6, signal, 1,      \
                                          SHMEM_SIGNAL_SET, i);                \
            }                                                                  \
          }                                                                    \
                                                                               \
          /* Each PE waits for its own signal */                               \
          shmem_uint64_wait_until(signal, SHMEM_CMP_EQ, 1);                    \
                                                                               \
          if (strcmp(dest, "World") != 0 || *signal != 1) {                    \
            log_fail("validation failed: dest = '%s' (expected 'World'), "     \
                     "signal = %d (expected 1)",                               \
                     dest, (int)*signal);                                      \
            success = false;                                                   \
          } else {                                                             \
            log_info("result is valid");                                       \
          }                                                                    \
                                                                               \
          shmem_ctx_destroy(ctx);                                              \
          log_info("Context destroyed");                                       \
        }                                                                      \
      }                                                                        \
                                                                               \
      if (dest)                                                                \
        shmem_free(dest);                                                      \
      if (value)                                                               \
        shmem_free(value);                                                     \
      if (signal)                                                              \
        shmem_free(signal);                                                    \
    }                                                                          \
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

  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(float, float);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(double, double);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(long double, longdouble);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(char, char);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(signed char, schar);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(short, short);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(int, int);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(long, long);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(long long, longlong);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(unsigned char, uchar);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(unsigned short, ushort);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(unsigned int, uint);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(unsigned long, ulong);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(int8_t, int8);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(int16_t, int16);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(int32_t, int32);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(int64_t, int64);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(uint8_t, uint8);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(uint16_t, uint16);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(uint32_t, uint32);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(uint64_t, uint64);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(size_t, size);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(ptrdiff_t, ptrdiff);

  if (!result) {
    rc = EXIT_FAILURE;
  }

  if (mype == 0) {
    display_test_result("C shmem_put_signal_nbi", result, false);
  }

  /* Test SIZE-specific variants */
  int result_size = true;
  result_size &= TEST_C_SHMEM_PUTSIZE_SIGNAL_NBI(8);
  result_size &= TEST_C_SHMEM_PUTSIZE_SIGNAL_NBI(16);
  result_size &= TEST_C_SHMEM_PUTSIZE_SIGNAL_NBI(32);
  result_size &= TEST_C_SHMEM_PUTSIZE_SIGNAL_NBI(64);
/* 128-bit operations may not be supported on all platforms */
#if defined(HAVE_FEATURE_PSHMEM) && defined(SHMEM_HAVE_EXTENDEDTYPES)
  result_size &= TEST_C_SHMEM_PUTSIZE_SIGNAL_NBI(128);
#endif

  if (!result_size) {
    rc = EXIT_FAILURE;
  }

  if (mype == 0) {
    display_test_result("C shmem_put<size>_signal_nbi", result_size, false);
  }

  /* Test memory-specific variant */
  int result_mem = TEST_C_SHMEM_PUTMEM_SIGNAL_NBI();

  if (!result_mem) {
    rc = EXIT_FAILURE;
  }

  if (mype == 0) {
    display_test_result("C shmem_putmem_signal_nbi", result_mem, false);
  }

  /* Test context TYPENAME variants */
  int result_ctx = true;
  result_ctx &= TEST_C_CTX_SHMEM_PUT_SIGNAL_NBI(float, float);
  result_ctx &= TEST_C_CTX_SHMEM_PUT_SIGNAL_NBI(double, double);
  result_ctx &= TEST_C_CTX_SHMEM_PUT_SIGNAL_NBI(long double, longdouble);
  result_ctx &= TEST_C_CTX_SHMEM_PUT_SIGNAL_NBI(char, char);
  result_ctx &= TEST_C_CTX_SHMEM_PUT_SIGNAL_NBI(signed char, schar);
  result_ctx &= TEST_C_CTX_SHMEM_PUT_SIGNAL_NBI(short, short);
  result_ctx &= TEST_C_CTX_SHMEM_PUT_SIGNAL_NBI(int, int);
  result_ctx &= TEST_C_CTX_SHMEM_PUT_SIGNAL_NBI(long, long);
  result_ctx &= TEST_C_CTX_SHMEM_PUT_SIGNAL_NBI(long long, longlong);
  result_ctx &= TEST_C_CTX_SHMEM_PUT_SIGNAL_NBI(unsigned char, uchar);
  result_ctx &= TEST_C_CTX_SHMEM_PUT_SIGNAL_NBI(unsigned short, ushort);
  result_ctx &= TEST_C_CTX_SHMEM_PUT_SIGNAL_NBI(unsigned int, uint);
  result_ctx &= TEST_C_CTX_SHMEM_PUT_SIGNAL_NBI(unsigned long, ulong);
  result_ctx &= TEST_C_CTX_SHMEM_PUT_SIGNAL_NBI(unsigned long long, ulonglong);
  result_ctx &= TEST_C_CTX_SHMEM_PUT_SIGNAL_NBI(int8_t, int8);
  result_ctx &= TEST_C_CTX_SHMEM_PUT_SIGNAL_NBI(int16_t, int16);
  result_ctx &= TEST_C_CTX_SHMEM_PUT_SIGNAL_NBI(int32_t, int32);
  result_ctx &= TEST_C_CTX_SHMEM_PUT_SIGNAL_NBI(int64_t, int64);
  result_ctx &= TEST_C_CTX_SHMEM_PUT_SIGNAL_NBI(uint8_t, uint8);
  result_ctx &= TEST_C_CTX_SHMEM_PUT_SIGNAL_NBI(uint16_t, uint16);
  result_ctx &= TEST_C_CTX_SHMEM_PUT_SIGNAL_NBI(uint32_t, uint32);
  result_ctx &= TEST_C_CTX_SHMEM_PUT_SIGNAL_NBI(uint64_t, uint64);
  result_ctx &= TEST_C_CTX_SHMEM_PUT_SIGNAL_NBI(size_t, size);
  result_ctx &= TEST_C_CTX_SHMEM_PUT_SIGNAL_NBI(ptrdiff_t, ptrdiff);

  if (!result_ctx) {
    rc = EXIT_FAILURE;
  }

  if (mype == 0) {
    display_test_result("C shmem_ctx_put_signal_nbi", result_ctx, false);
  }

  /* Test context SIZE-specific variants */
  int result_ctx_size = true;
  result_ctx_size &= TEST_C_CTX_SHMEM_PUTSIZE_SIGNAL_NBI(8);
  result_ctx_size &= TEST_C_CTX_SHMEM_PUTSIZE_SIGNAL_NBI(16);
  result_ctx_size &= TEST_C_CTX_SHMEM_PUTSIZE_SIGNAL_NBI(32);
  result_ctx_size &= TEST_C_CTX_SHMEM_PUTSIZE_SIGNAL_NBI(64);
/* 128-bit operations may not be supported on all platforms */
#if defined(HAVE_FEATURE_PSHMEM) && defined(SHMEM_HAVE_EXTENDEDTYPES)
  result_ctx_size &= TEST_C_CTX_SHMEM_PUTSIZE_SIGNAL_NBI(128);
#endif

  if (!result_ctx_size) {
    rc = EXIT_FAILURE;
  }

  if (mype == 0) {
    display_test_result("C shmem_ctx_put<size>_signal_nbi", result_ctx_size,
                        false);
  }

  /* Test context memory-specific variant */
  int result_ctx_mem = TEST_C_CTX_SHMEM_PUTMEM_SIGNAL_NBI();

  if (!result_ctx_mem) {
    rc = EXIT_FAILURE;
  }

  if (mype == 0) {
    display_test_result("C shmem_ctx_putmem_signal_nbi", result_ctx_mem, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
