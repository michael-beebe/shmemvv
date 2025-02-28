/**
 * @file cxx_shmem_signal_fetch.cpp
 * @brief Unit test for the shmem_signal_fetch() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_CXX_SHMEM_SIGNAL_FETCH()                                          \
  ({                                                                           \
    log_routine("shmem_signal_fetch");                                         \
    bool success = true;                                                       \
    static uint64_t signal = 1;                                                \
    uint64_t fetched_signal = 0;                                               \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    log_info("Running on PE %d of %d total PEs", mype, npes);                  \
                                                                               \
    if (npes < 2) {                                                            \
      log_fail("Test requires at least 2 PEs, but only %d PE(s) available",    \
               npes);                                                          \
      success = false;                                                         \
    } else {                                                                   \
      log_info("Entering barrier before signal fetch");                        \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 1) {                                                         \
        log_info("PE 1: Fetching signal value");                               \
        fetched_signal = shmem_signal_fetch(&signal);                          \
        log_info("Expected signal value: 1, Fetched signal value: %lu",        \
                 (unsigned long)fetched_signal);                               \
        if (fetched_signal != 1) {                                             \
          log_fail("Validation failed: Signal value mismatch");                \
          success = false;                                                     \
        } else {                                                               \
          log_info("Validation successful: Signal value matches expected");    \
        }                                                                      \
      } else {                                                                 \
        log_info("PE %d: Waiting while PE 1 fetches signal", mype);            \
      }                                                                        \
    }                                                                          \
    log_info("Test completed with %s", success ? "SUCCESS" : "FAILURE");       \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  if (!(npes <= 2)) {
    if (mype == 0) {
      display_not_enough_pes("SIGNALING");
    }
    shmem_finalize();
    return EXIT_SUCCESS;
  }

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_CXX_SHMEM_SIGNAL_FETCH();

  if (!result) {
    rc = EXIT_FAILURE;
  }

  if (mype == 0) {
    display_test_result("CXX shmem_signal_fetch()", result, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
