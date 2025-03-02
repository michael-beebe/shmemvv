/**
 * @file c11_shmem_put_signal_nbi.c
 * @brief Unit test for the shmem_put_signal_nbi() routine.
 */

#include <stdio.h>
#include <stdlib.h>
#include <shmem.h>
#include <string.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C11_SHMEM_PUT_SIGNAL_NBI(TYPE)                                    \
  ({                                                                           \
    log_routine("shmem_put_signal_nbi(" #TYPE ")");                            \
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
        shmem_put_signal_nbi(&dest, &value, 1, &signal, 1, target_pe,          \
                             SHMEM_SIGNAL_SET);                                \
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

#define TEST_C11_CTX_SHMEM_PUT_SIGNAL_NBI(TYPE)                                \
  ({                                                                           \
    log_routine("shmem_put_signal_nbi(ctx, " #TYPE ")");                       \
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
        shmem_put_signal_nbi(ctx, &dest, &value, 1, &signal, 1, target_pe,     \
                             SHMEM_SIGNAL_SET);                                \
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

  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(float);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(double);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(long double);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(char);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(signed char);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(short);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(int);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(long);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(long long);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(unsigned char);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(unsigned short);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(unsigned int);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(unsigned long);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(unsigned long long);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(int8_t);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(int16_t);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(int32_t);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(int64_t);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(uint8_t);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(uint16_t);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(uint32_t);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(uint64_t);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(size_t);
  result &= TEST_C11_SHMEM_PUT_SIGNAL_NBI(ptrdiff_t);

  if (!result) {
    rc = EXIT_FAILURE;
  }

  if (mype == 0) {
    display_test_result("C11 shmem_put_signal_nbi", result, false);
  }

  /* Test context-specific variants */
  int result_ctx = true;
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL_NBI(float);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL_NBI(double);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL_NBI(long double);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL_NBI(char);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL_NBI(signed char);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL_NBI(short);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL_NBI(int);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL_NBI(long);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL_NBI(long long);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL_NBI(unsigned char);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL_NBI(unsigned short);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL_NBI(unsigned int);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL_NBI(unsigned long);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL_NBI(unsigned long long);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL_NBI(int8_t);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL_NBI(int16_t);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL_NBI(int32_t);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL_NBI(int64_t);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL_NBI(uint8_t);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL_NBI(uint16_t);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL_NBI(uint32_t);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL_NBI(uint64_t);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL_NBI(size_t);
  result_ctx &= TEST_C11_CTX_SHMEM_PUT_SIGNAL_NBI(ptrdiff_t);

  if (!result_ctx) {
    rc = EXIT_FAILURE;
  }

  if (mype == 0) {
    display_test_result("C11 shmem_put_signal_nbi with ctx", result_ctx, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
