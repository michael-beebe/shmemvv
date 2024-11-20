/**
 * @file c_shmem_put_signal_nbi.c
 * @brief Unit test for the shmem_put_signal_nbi() routine.
 */

#include <stdio.h>
#include <stdlib.h>
#include <shmem.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C11_SHMEM_PUT_SIGNAL_NBI(TYPE)                                    \
  ({                                                                           \
    log_routine("shmem_put_signal_nbi(" #TYPE ")");                            \
    bool success = true;                                                       \
    static TYPE dest = 0;                                                      \
    static TYPE value = (TYPE)123;                                             \
    static uint64_t signal = 0;                                                \
    log_info("signal @ %p, value @ %p, dest @ %p", &signal, &value, &dest);    \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
                                                                               \
    if (npes < 2) {                                                            \
      success = false;                                                         \
    } else {                                                                   \
      int target_pe = (mype + 1) % npes;                                       \
      log_info("targetting pe %d", target_pe);                                 \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 0) {                                                         \
        log_info("calling shmem_put_signal()");                                \
        shmem_put_signal_nbi(&dest, &value, 1, &signal, 1, target_pe,          \
                             SHMEM_SIGNAL_SET);                                \
        shmem_quiet();                                                         \
      }                                                                        \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 1) {                                                         \
        log_info("validating result");                                         \
        if (dest != 123 || signal != 1) {                                      \
          log_info("failed: expected dest = 123, signal = 1, got dest = %d, "  \
                   "signal = %d",                                              \
                   (char)dest, (char)signal);                                  \
          success = false;                                                     \
        }                                                                      \
        log_info("result is valid");                                           \
      } else {                                                                 \
        log_info("pe 1 is validating...");                                     \
      }                                                                        \
    }                                                                          \
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
    display_test_result("C11 shmem_put_signal_nbi()", result, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
