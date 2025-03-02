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
    static TYPE dest = 0;                                                      \
    static TYPE value = (TYPE)123;                                             \
    static uint64_t signal = 0;                                                \
    log_info(                                                                  \
        "Test variables initialized - signal @ %p, value @ %p, dest @ %p",     \
        &signal, &value, &dest);                                               \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    log_info("Running on PE %d of %d total PEs", mype, npes);                  \
                                                                               \
    if (npes < 2) {                                                            \
      log_fail("Test requires at least 2 PEs, but only %d PE(s) available",    \
               npes);                                                          \
      success = false;                                                         \
    } else {                                                                   \
      int target_pe = (mype + 1) % npes;                                       \
      log_info("PE %d will send data to PE %d", mype, target_pe);              \
                                                                               \
      log_info("Entering barrier before data transfer");                       \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 0) {                                                         \
        log_info("PE 0: Initiating non-blocking put with signal to PE %d",     \
                 target_pe);                                                   \
        log_info("Sending value %d with signal value 1", (int)value);          \
        shmem_##TYPENAME##_put_signal_nbi(&dest, &value, 1, &signal, 1,        \
                                          target_pe, SHMEM_SIGNAL_SET);        \
        log_info("Calling quiet to ensure transfer completion");               \
        shmem_quiet();                                                         \
      }                                                                        \
                                                                               \
      log_info("Entering barrier after data transfer");                        \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 1) {                                                         \
        log_info("PE 1: Validating received data and signal");                 \
        log_info("Expected: dest = 123, signal = 1");                          \
        log_info("Received: dest = %d, signal = %lu", (int)dest,               \
                 (unsigned long)signal);                                       \
        if (dest != 123 || signal != 1) {                                      \
          log_fail("Validation failed: Data or signal mismatch");              \
          success = false;                                                     \
        } else {                                                               \
          log_info(                                                            \
              "Validation successful: Data and signal match expected values"); \
        }                                                                      \
      } else {                                                                 \
        log_info("PE %d: Waiting while PE 1 validates results", mype);         \
      }                                                                        \
    }                                                                          \
    log_info("Test completed with %s", success ? "SUCCESS" : "FAILURE");       \
    success;                                                                   \
  })

/* Test for context TYPENAME variants */
#define TEST_C_CTX_SHMEM_PUT_SIGNAL_NBI(TYPE, TYPENAME)                        \
  ({                                                                           \
    log_routine("shmem_ctx_" #TYPENAME "_put_signal_nbi");                     \
    bool success = true;                                                       \
    static TYPE dest = 0;                                                      \
    static TYPE value = (TYPE)456;                                             \
    static uint64_t signal = 0;                                                \
    log_info(                                                                  \
        "Test variables initialized - signal @ %p, value @ %p, dest @ %p",     \
        &signal, &value, &dest);                                               \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    log_info("Running on PE %d of %d total PEs", mype, npes);                  \
                                                                               \
    if (npes < 2) {                                                            \
      log_fail("Test requires at least 2 PEs, but only %d PE(s) available",    \
               npes);                                                          \
      success = false;                                                         \
    } else {                                                                   \
      int target_pe = (mype + 1) % npes;                                       \
      log_info("PE %d will send data to PE %d", mype, target_pe);              \
                                                                               \
      shmem_ctx_t ctx;                                                         \
      int ctx_create_status = shmem_ctx_create(0, &ctx);                       \
                                                                               \
      if (ctx_create_status != 0) {                                            \
        log_fail("Failed to create context");                                  \
        return false;                                                          \
      }                                                                        \
      log_info("Successfully created context");                                \
                                                                               \
      log_info("Entering barrier before data transfer");                       \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 0) {                                                         \
        log_info("PE 0: Initiating context non-blocking put with signal to PE %d", \
                 target_pe);                                                   \
        log_info("Sending value %d with signal value 1", (int)value);          \
        shmem_ctx_##TYPENAME##_put_signal_nbi(ctx, &dest, &value, 1, &signal,  \
                                             1, target_pe, SHMEM_SIGNAL_SET);  \
        log_info("Calling quiet to ensure transfer completion");               \
        shmem_ctx_quiet(ctx);                                                  \
      }                                                                        \
                                                                               \
      log_info("Entering barrier after data transfer");                        \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 1) {                                                         \
        log_info("PE 1: Validating received data and signal");                 \
        log_info("Expected: dest = 456, signal = 1");                          \
        log_info("Received: dest = %d, signal = %lu", (int)dest,               \
                 (unsigned long)signal);                                       \
        if (dest != 456 || signal != 1) {                                      \
          log_fail("Validation failed: Data or signal mismatch");              \
          success = false;                                                     \
        } else {                                                               \
          log_info(                                                            \
              "Validation successful: Data and signal match expected values"); \
        }                                                                      \
      } else {                                                                 \
        log_info("PE %d: Waiting while PE 1 validates results", mype);         \
      }                                                                        \
                                                                               \
      shmem_ctx_destroy(ctx);                                                  \
      log_info("Context destroyed");                                           \
    }                                                                          \
    log_info("Test completed with %s", success ? "SUCCESS" : "FAILURE");       \
    success;                                                                   \
  })

/* Test for SIZE-specific variants */
#define TEST_C_SHMEM_PUTSIZE_SIGNAL_NBI(SIZE)                                  \
  ({                                                                           \
    log_routine("shmem_put" #SIZE "_signal_nbi");                              \
    bool success = true;                                                       \
    static uint##SIZE##_t dest = 0;                                            \
    static uint##SIZE##_t value = 123;                                         \
    static uint64_t signal = 0;                                                \
    log_info(                                                                  \
        "Test variables initialized - signal @ %p, value @ %p, dest @ %p",     \
        &signal, &value, &dest);                                               \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    log_info("Running on PE %d of %d total PEs", mype, npes);                  \
                                                                               \
    if (npes < 2) {                                                            \
      log_fail("Test requires at least 2 PEs, but only %d PE(s) available",    \
               npes);                                                          \
      success = false;                                                         \
    } else {                                                                   \
      int target_pe = (mype + 1) % npes;                                       \
      log_info("PE %d will send data to PE %d", mype, target_pe);              \
                                                                               \
      log_info("Entering barrier before data transfer");                       \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 0) {                                                         \
        log_info("PE 0: Initiating SIZE-specific non-blocking put with signal to PE %d", \
                 target_pe);                                                   \
        log_info("Sending value %d with signal value 1", (int)value);          \
        shmem_put##SIZE##_signal_nbi(&dest, &value, 1, &signal, 1, target_pe,  \
                                    SHMEM_SIGNAL_SET);                         \
        log_info("Calling quiet to ensure transfer completion");               \
        shmem_quiet();                                                         \
      }                                                                        \
                                                                               \
      log_info("Entering barrier after data transfer");                        \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 1) {                                                         \
        log_info("PE 1: Validating received data and signal");                 \
        log_info("Expected: dest = 123, signal = 1");                          \
        log_info("Received: dest = %d, signal = %lu", (int)dest,               \
                 (unsigned long)signal);                                       \
        if (dest != 123 || signal != 1) {                                      \
          log_fail("Validation failed: Data or signal mismatch");              \
          success = false;                                                     \
        } else {                                                               \
          log_info(                                                            \
              "Validation successful: Data and signal match expected values"); \
        }                                                                      \
      } else {                                                                 \
        log_info("PE %d: Waiting while PE 1 validates results", mype);         \
      }                                                                        \
    }                                                                          \
    log_info("Test completed with %s", success ? "SUCCESS" : "FAILURE");       \
    success;                                                                   \
  })

/* Test for context SIZE-specific variants */
#define TEST_C_CTX_SHMEM_PUTSIZE_SIGNAL_NBI(SIZE)                              \
  ({                                                                           \
    log_routine("shmem_ctx_put" #SIZE "_signal_nbi");                          \
    bool success = true;                                                       \
    static uint##SIZE##_t dest = 0;                                            \
    /* Choose an appropriate value based on SIZE */                            \
    static uint##SIZE##_t value = (SIZE == 8)    ? 200                         \
                                  : (SIZE == 16) ? 456                         \
                                                 : 456;                        \
    static uint64_t signal = 0;                                                \
    log_info(                                                                  \
        "Test variables initialized - signal @ %p, value @ %p, dest @ %p",     \
        &signal, &value, &dest);                                               \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    log_info("Running on PE %d of %d total PEs", mype, npes);                  \
                                                                               \
    if (npes < 2) {                                                            \
      log_fail("Test requires at least 2 PEs, but only %d PE(s) available",    \
               npes);                                                          \
      success = false;                                                         \
    } else {                                                                   \
      int target_pe = (mype + 1) % npes;                                       \
      log_info("PE %d will send data to PE %d", mype, target_pe);              \
                                                                               \
      shmem_ctx_t ctx;                                                         \
      int ctx_create_status = shmem_ctx_create(0, &ctx);                       \
                                                                               \
      if (ctx_create_status != 0) {                                            \
        log_fail("Failed to create context");                                  \
        return false;                                                          \
      }                                                                        \
      log_info("Successfully created context");                                \
                                                                               \
      log_info("Entering barrier before data transfer");                       \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 0) {                                                         \
        log_info("PE 0: Initiating context SIZE-specific non-blocking put with signal to PE %d", \
                 target_pe);                                                   \
        log_info("Sending value %d with signal value 1", (int)value);          \
        shmem_ctx_put##SIZE##_signal_nbi(ctx, &dest, &value, 1, &signal, 1,    \
                                        target_pe, SHMEM_SIGNAL_SET);          \
        log_info("Calling quiet to ensure transfer completion");               \
        shmem_ctx_quiet(ctx);                                                  \
      }                                                                        \
                                                                               \
      log_info("Entering barrier after data transfer");                        \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 1) {                                                         \
        log_info("PE 1: Validating received data and signal");                 \
        /* Use the appropriate expected value based on SIZE */                 \
        uint##SIZE##_t expected = (SIZE == 8)    ? 200                         \
                                  : (SIZE == 16) ? 456                         \
                                                 : 456;                        \
        log_info("Expected: dest = %d, signal = 1", (int)expected);            \
        log_info("Received: dest = %d, signal = %lu", (int)dest,               \
                 (unsigned long)signal);                                       \
        if (dest != expected || signal != 1) {                                 \
          log_fail("Validation failed: Data or signal mismatch");              \
          success = false;                                                     \
        } else {                                                               \
          log_info(                                                            \
              "Validation successful: Data and signal match expected values"); \
        }                                                                      \
      } else {                                                                 \
        log_info("PE %d: Waiting while PE 1 validates results", mype);         \
      }                                                                        \
                                                                               \
      shmem_ctx_destroy(ctx);                                                  \
      log_info("Context destroyed");                                           \
    }                                                                          \
    log_info("Test completed with %s", success ? "SUCCESS" : "FAILURE");       \
    success;                                                                   \
  })

/* Test for memory-specific variant */
#define TEST_C_SHMEM_PUTMEM_SIGNAL_NBI()                                       \
  ({                                                                           \
    log_routine("shmem_putmem_signal_nbi");                                    \
    bool success = true;                                                       \
    static char dest[10];                                                      \
    static char value[10] = "Hello";                                           \
    static uint64_t signal = 0;                                                \
    log_info(                                                                  \
        "Test variables initialized - signal @ %p, value @ %p, dest @ %p",     \
        &signal, value, dest);                                                 \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    log_info("Running on PE %d of %d total PEs", mype, npes);                  \
                                                                               \
    if (npes < 2) {                                                            \
      log_fail("Test requires at least 2 PEs, but only %d PE(s) available",    \
               npes);                                                          \
      success = false;                                                         \
    } else {                                                                   \
      int target_pe = (mype + 1) % npes;                                       \
      log_info("PE %d will send data to PE %d", mype, target_pe);              \
                                                                               \
      log_info("Entering barrier before data transfer");                       \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 0) {                                                         \
        log_info("PE 0: Initiating memory-specific non-blocking put with signal to PE %d", \
                 target_pe);                                                   \
        log_info("Sending value '%s' with signal value 1", value);             \
        shmem_putmem_signal_nbi(dest, value, 6, &signal, 1, target_pe,         \
                               SHMEM_SIGNAL_SET);                              \
        log_info("Calling quiet to ensure transfer completion");               \
        shmem_quiet();                                                         \
      }                                                                        \
                                                                               \
      log_info("Entering barrier after data transfer");                        \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 1) {                                                         \
        log_info("PE 1: Validating received data and signal");                 \
        log_info("Expected: dest = 'Hello', signal = 1");                      \
        log_info("Received: dest = '%s', signal = %lu", dest,                  \
                 (unsigned long)signal);                                       \
        if (strcmp(dest, "Hello") != 0 || signal != 1) {                       \
          log_fail("Validation failed: Data or signal mismatch");              \
          success = false;                                                     \
        } else {                                                               \
          log_info(                                                            \
              "Validation successful: Data and signal match expected values"); \
        }                                                                      \
      } else {                                                                 \
        log_info("PE %d: Waiting while PE 1 validates results", mype);         \
      }                                                                        \
    }                                                                          \
    log_info("Test completed with %s", success ? "SUCCESS" : "FAILURE");       \
    success;                                                                   \
  })

/* Test for context memory-specific variant */
#define TEST_C_CTX_SHMEM_PUTMEM_SIGNAL_NBI()                                   \
  ({                                                                           \
    log_routine("shmem_ctx_putmem_signal_nbi");                                \
    bool success = true;                                                       \
    static char dest[10];                                                      \
    static char value[10] = "World";                                           \
    static uint64_t signal = 0;                                                \
    log_info(                                                                  \
        "Test variables initialized - signal @ %p, value @ %p, dest @ %p",     \
        &signal, value, dest);                                                 \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    log_info("Running on PE %d of %d total PEs", mype, npes);                  \
                                                                               \
    if (npes < 2) {                                                            \
      log_fail("Test requires at least 2 PEs, but only %d PE(s) available",    \
               npes);                                                          \
      success = false;                                                         \
    } else {                                                                   \
      int target_pe = (mype + 1) % npes;                                       \
      log_info("PE %d will send data to PE %d", mype, target_pe);              \
                                                                               \
      shmem_ctx_t ctx;                                                         \
      int ctx_create_status = shmem_ctx_create(0, &ctx);                       \
                                                                               \
      if (ctx_create_status != 0) {                                            \
        log_fail("Failed to create context");                                  \
        return false;                                                          \
      }                                                                        \
      log_info("Successfully created context");                                \
                                                                               \
      log_info("Entering barrier before data transfer");                       \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 0) {                                                         \
        log_info("PE 0: Initiating context memory-specific non-blocking put with signal to PE %d", \
                 target_pe);                                                   \
        log_info("Sending value '%s' with signal value 1", value);             \
        shmem_ctx_putmem_signal_nbi(ctx, dest, value, 6, &signal, 1, target_pe, \
                                   SHMEM_SIGNAL_SET);                          \
        log_info("Calling quiet to ensure transfer completion");               \
        shmem_ctx_quiet(ctx);                                                  \
      }                                                                        \
                                                                               \
      log_info("Entering barrier after data transfer");                        \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 1) {                                                         \
        log_info("PE 1: Validating received data and signal");                 \
        log_info("Expected: dest = 'World', signal = 1");                      \
        log_info("Received: dest = '%s', signal = %lu", dest,                  \
                 (unsigned long)signal);                                       \
        if (strcmp(dest, "World") != 0 || signal != 1) {                       \
          log_fail("Validation failed: Data or signal mismatch");              \
          success = false;                                                     \
        } else {                                                               \
          log_info(                                                            \
              "Validation successful: Data and signal match expected values"); \
        }                                                                      \
      } else {                                                                 \
        log_info("PE %d: Waiting while PE 1 validates results", mype);         \
      }                                                                        \
                                                                               \
      shmem_ctx_destroy(ctx);                                                  \
      log_info("Context destroyed");                                           \
    }                                                                          \
    log_info("Test completed with %s", success ? "SUCCESS" : "FAILURE");       \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  int npes = shmem_n_pes();
  int mype = shmem_my_pe();

  if (!(npes <= 2)) {
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
    display_test_result("C shmem_ctx_put<size>_signal_nbi", result_ctx_size, false);
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
