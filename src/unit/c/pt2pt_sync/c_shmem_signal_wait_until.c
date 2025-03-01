/**
 * @file c_shmem_signal_wait_until.c
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

#define TEST_C_SHMEM_SIGNAL_WAIT_UNTIL()                                       \
  ({                                                                           \
    log_routine("shmem_signal_wait_until()");                                  \
    bool success = true;                                                       \
    uint64_t *flag = (uint64_t *)shmem_malloc(sizeof(uint64_t));               \
    log_info("Allocated flag variable (%zu bytes) at address %p",              \
             sizeof(uint64_t), (void *)flag);                                  \
    if (flag == NULL) {                                                        \
      log_fail("Memory allocation failed - shmem_malloc returned NULL");       \
      success = false;                                                         \
    } else {                                                                   \
      *flag = 0;                                                               \
      log_info("Initialized flag value to 0");                                 \
      int mype = shmem_my_pe();                                                \
      int npes = shmem_n_pes();                                                \
      uint64_t value = 1;                                                      \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 0) {                                                         \
        for (int pe = 1; pe < npes; ++pe) {                                    \
          log_info("PE 0: Setting flag to 1 on remote PE %d", pe);             \
          shmem_uint64_p(flag, value, pe);                                     \
        }                                                                      \
        log_info("PE 0: Completed setting flags, calling shmem_quiet()");      \
        shmem_quiet();                                                         \
      } else {                                                                 \
        log_info("PE %d: Waiting for PE 0 to set local flag", mype);           \
      }                                                                        \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype != 0) {                                                         \
        time_t start_time = time(NULL);                                        \
        log_info("PE %d: Starting signal_wait_until loop (flag=%p, "           \
                 "condition=SHMEM_CMP_EQ, target=1)",                          \
                 mype, (void *)flag);                                          \
        while (!shmem_test(flag, SHMEM_CMP_EQ, value) &&                       \
               time(NULL) - start_time < TIMEOUT) {                            \
          shmem_signal_wait_until(flag, SHMEM_CMP_EQ, value);                  \
        }                                                                      \
        log_info("PE %d: signal_wait_until completed, flag value is %lu",      \
                 mype, *flag);                                                 \
        if (*flag != value) {                                                  \
          log_fail("PE %d: Test failed - flag value mismatch after wait. "     \
                   "Expected %lu but got %lu",                                 \
                   mype, value, *flag);                                        \
          success = false;                                                     \
        } else {                                                               \
          log_info("PE %d: Successfully received expected flag value", mype);  \
        }                                                                      \
      }                                                                        \
      log_info("Freeing allocated memory at %p", (void *)flag);                \
      shmem_free(flag);                                                        \
    }                                                                          \
    success;                                                                   \
  })

int main(int argc, char **argv) {
  shmem_init();
  log_init(__FILE__);

  int result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C_SHMEM_SIGNAL_WAIT_UNTIL();

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_signal_wait_until", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
