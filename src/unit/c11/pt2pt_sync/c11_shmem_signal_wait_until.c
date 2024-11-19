/**
 * @file c11_shmem_signal_wait_until.c
 * @brief Unit test shmem_signal_wait_until() routine.
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "log.h"
#include "shmemvv.h"

#define TIMEOUT 2

#define TEST_CXX_SHMEM_SIGNAL_WAIT_UNTIL()                                     \
  ({                                                                           \
    log_routine("c11_shmem_signal_wait_until()");                              \
    bool success = true;                                                       \
    uint64_t *flag = (uint64_t *)shmem_malloc(sizeof(uint64_t));               \
    log_info("shmem_malloc'd flag (%d bytes) at %p", sizeof(uint64_t),         \
             (void *)flag);                                                    \
    if (flag == NULL) {                                                        \
      log_fail("shmem_malloc failed!");                                        \
      success = false;                                                         \
    } else {                                                                   \
      *flag = 0;                                                               \
      log_info("set flag to 0");                                               \
      int mype = shmem_my_pe();                                                \
      int npes = shmem_n_pes();                                                \
      uint64_t value = 1;                                                      \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 0) {                                                         \
        for (int pe = 1; pe < npes; ++pe) {                                    \
          log_info("setting flag to 1 on pe %d", pe);                          \
          shmem_uint64_p(flag, value, pe);                                     \
        }                                                                      \
        shmem_quiet();                                                         \
      } else {                                                                 \
        log_info("waiting for pe 0 to set the flag");                          \
      }                                                                        \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype != 0) {                                                         \
        time_t start_time = time(NULL);                                        \
        log_info("executing signal_wait_until_some(flag = %p, status = "       \
                 "SHMEM_CMP_EQ, to = 1)",                                      \
                 (void *)flag);                                                \
        while (!shmem_test(flag, SHMEM_CMP_EQ, value) &&                       \
               time(NULL) - start_time < TIMEOUT) {                            \
          shmem_signal_wait_until(flag, SHMEM_CMP_EQ, value);                  \
        }                                                                      \
        log_info("signal_wait_until returned");                                \
        if (*flag != value) {                                                  \
          log_fail("signal_wait_until returned, but flag didn't match! "       \
                   "(expected 1, got %d)",                                     \
                   flag);                                                      \
          success = false;                                                     \
        }                                                                      \
      }                                                                        \
      shmem_free(flag);                                                        \
    }                                                                          \
    success;                                                                   \
  })

int main(int argc, char **argv) {
  shmem_init();
  log_init(__FILE__);

  int result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_CXX_SHMEM_SIGNAL_WAIT_UNTIL();

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_signal_wait_until()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
