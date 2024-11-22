/**
 * @file cxx_shmem_put_signal.cpp
 * @brief Unit test for the shmem_put_signal() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_CXX_SHMEM_PUT_SIGNAL(TYPE, TYPENAME)                              \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_put_signal");                             \
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
        log_info("PE 0: Initiating put with signal to PE %d", target_pe);      \
        log_info("Sending value %d with signal value 1", (int)value);          \
        shmem_##TYPENAME##_put_signal(&dest, &value, 1, &signal, 1, target_pe, \
                                      SHMEM_SIGNAL_SET);                       \
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

  result &= TEST_CXX_SHMEM_PUT_SIGNAL(float, float);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(double, double);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(long double, longdouble);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(char, char);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(signed char, schar);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(short, short);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(int, int);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(long, long);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(long long, longlong);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(unsigned char, uchar);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(unsigned short, ushort);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(unsigned int, uint);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(int8_t, int8);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(int16_t, int16);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(int32_t, int32);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(int64_t, int64);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(uint8_t, uint8);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(uint16_t, uint16);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(size_t, size);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(ptrdiff_t, ptrdiff);

  if (!result) {
    rc = EXIT_FAILURE;
  }

  if (mype == 0) {
    display_test_result("CXX shmem_put_signal()", result, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}