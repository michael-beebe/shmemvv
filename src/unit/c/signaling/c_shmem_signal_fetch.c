/**
 * @file c_shmem_signal_fetch.c
 * @brief Unit test for the shmem_signal_fetch() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C_SHMEM_SIGNAL_FETCH()                                            \
  ({                                                                           \
    log_routine("shmem_signal_fetch()");                                       \
    bool success = true;                                                       \
    static uint64_t signal = 1;                                                \
    log_info("signal initialized to %lu @ %p", signal, &signal);               \
    uint64_t fetched_signal = 0;                                               \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
                                                                               \
    if (npes < 2) {                                                            \
      log_fail("test requires at least 2 PEs, only have %d", npes);            \
      success = false;                                                         \
    } else {                                                                   \
      log_info("starting barrier_all with %d PEs", npes);                      \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 1) {                                                         \
        log_info("PE %d calling shmem_signal_fetch(signal = %p)", mype,        \
                 &signal);                                                     \
        fetched_signal = shmem_signal_fetch(&signal);                          \
        log_info("fetched signal value: %lu", fetched_signal);                 \
        if (fetched_signal != 1) {                                             \
          log_fail("unexpected return value: expected 1, found %lu",           \
                   fetched_signal);                                            \
          success = false;                                                     \
        } else {                                                               \
          log_info("signal fetch successful - value matches expected");        \
        }                                                                      \
      } else {                                                                 \
        log_info("PE %d waiting at barrier", mype);                            \
      }                                                                        \
    }                                                                          \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  if (!(npes >= 2)) {
    if (mype == 0) {
      display_not_enough_pes("SIGNALING");
    }
    shmem_finalize();
    return EXIT_SUCCESS;
  }

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C_SHMEM_SIGNAL_FETCH();

  if (!result) {
    rc = EXIT_FAILURE;
  }

  if (mype == 0) {
    display_test_result("C shmem_signal_fetch", result, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
