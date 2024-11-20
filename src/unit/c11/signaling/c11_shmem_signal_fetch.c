/**
 * @file c_shmem_signal_fetch.c
 * @brief Unit test for the shmem_signal_fetch() routine.
 */

#include <stdio.h>
#include <stdlib.h>
#include <shmem.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C_SHMEM_SIGNAL_FETCH()                                            \
  ({                                                                           \
    log_routine("shmem_signal_fetch()");                                       \
    bool success = true;                                                       \
    static uint64_t signal = 1;                                                \
    log_info("signal @ %p", &signal);                                          \
    uint64_t fetched_signal = 0;                                               \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
                                                                               \
    if (npes < 2) {                                                            \
      success = false;                                                         \
    } else {                                                                   \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 1) {                                                         \
        log_info("calling shmem_signal_fetch(signal = %p)", &signal);          \
        fetched_signal = shmem_signal_fetch(&signal);                          \
        if (fetched_signal != 1) {                                             \
          log_fail("unexpected return value: expected 1, found %d",            \
                   fetched_signal);                                            \
          success = false;                                                     \
        }                                                                      \
        log_info("result is valid");                                           \
      }                                                                        \
    }                                                                          \
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

  result &= TEST_C_SHMEM_SIGNAL_FETCH();

  if (!result) {
    rc = EXIT_FAILURE;
  }

  if (mype == 0) {
    display_test_result("C11 shmem_signal_fetch()", result, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
