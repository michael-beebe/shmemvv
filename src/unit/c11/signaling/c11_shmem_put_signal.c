/**
 * @file c11_shmem_put_signal.c
 * @brief Unit test for the shmem_put_signal() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C11_SHMEM_PUT_SIGNAL(TYPE)                                        \
  ({                                                                           \
    log_routine("shmem_put_signal(" #TYPE ")");                                \
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
        log_info("calling shmem_put_signal(dest=%p, value=%p, "                \
                 "signal=%p, target_pe=%d)",                                   \
                 dest, value, signal, target_pe);                              \
        shmem_put_signal(dest, value, 1, signal, 1, SHMEM_SIGNAL_SET,          \
                         target_pe);                                           \
      }                                                                        \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 1) {                                                         \
        /* Wait for signal to be set */                                        \
        shmem_wait_until(signal, SHMEM_CMP_EQ, 1);                             \
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

#define TEST_C11_CTX_SHMEM_PUT_SIGNAL(TYPE)                                    \
  ({                                                                           \
    log_routine("shmem_put_signal(ctx, " #TYPE ")");                           \
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
        int target_pe = 1; /* Simple: PE 0 sends to PE 1 */                    \
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
            log_info("calling shmem_put_signal(ctx, dest=%p, value=%p, "       \
                     "signal=%p, target_pe=%d)",                               \
                     dest, value, signal, target_pe);                          \
            shmem_put_signal(ctx, dest, value, 1, signal, 1, SHMEM_SIGNAL_SET, \
                             target_pe);                                       \
          }                                                                    \
                                                                               \
          shmem_barrier_all();                                                 \
                                                                               \
          if (mype == 1) {                                                     \
            /* Wait for signal to be set */                                    \
            shmem_wait_until(signal, SHMEM_CMP_EQ, 1);                         \
            TYPE expected_value = (sizeof(TYPE) == 1) ? (TYPE)42 : (TYPE)456;  \
            if (*dest != expected_value || *signal != 1) {                     \
              log_fail("validation failed: dest = %d (expected %d), "          \
                       "signal = %d (expected 1)",                             \
                       (int)*dest, (int)expected_value, (int)*signal);         \
              success = false;                                                 \
            } else {                                                           \
              log_info("result is valid");                                     \
            }                                                                  \
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

  /* Test standard C11 shmem_put_signal for different types */
  result &= TEST_C11_SHMEM_PUT_SIGNAL(float);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(double);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(long double);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(char);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(signed char);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(short);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(int);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(long);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(long long);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(unsigned char);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(unsigned short);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(unsigned int);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(unsigned long);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(unsigned long long);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(int8_t);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(int16_t);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(int32_t);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(int64_t);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(uint8_t);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(uint16_t);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(uint32_t);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(uint64_t);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(size_t);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(ptrdiff_t);

  if (!result) {
    rc = EXIT_FAILURE;
  }

  if (mype == 0) {
    display_test_result("C11 shmem_put_signal", result, false);
  }

  /* Test context-specific variants in C11 */
  int result_ctx = true;

  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL(float);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL(double);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL(long double);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL(char);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL(signed char);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL(short);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL(int);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL(long);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL(long long);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL(unsigned char);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL(unsigned short);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL(unsigned int);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL(unsigned long);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL(unsigned long long);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL(int8_t);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL(int16_t);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL(int32_t);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL(int64_t);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL(uint8_t);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL(uint16_t);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL(uint32_t);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL(uint64_t);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL(size_t);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL(ptrdiff_t);

  if (!result_ctx) {
    rc = EXIT_FAILURE;
  }

  if (mype == 0) {
    display_test_result("C11 shmem_put_signal with ctx", result_ctx, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
