/**
 * @file c_shmem_signal_wait_until.c
 * @brief Unit test shmem_signal_wait_until() routine.
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "log.h"
#include "shmemvv.h"

#define TIMEOUT 10

#define TEST_C_SHMEM_SIGNAL_WAIT_UNTIL()                                       \
  ({                                                                           \
    log_routine("shmem_signal_wait_until()");                                  \
    bool success = true;                                                       \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    uint64_t *wait_vars = (uint64_t *)shmem_calloc(npes, sizeof(uint64_t));    \
    log_info("Allocated wait_vars array (%zu bytes) at address %p",            \
             npes * sizeof(uint64_t), (void *)wait_vars);                      \
    if (wait_vars == NULL) {                                                   \
      log_fail("Memory allocation failed - shmem_calloc returned NULL");       \
      success = false;                                                         \
    } else {                                                                   \
      log_info("PE %d: Sending value %d to all PEs", mype, (int)(mype + 1));   \
      /* Put mype+1 to every PE */                                             \
      for (int i = 0; i < npes; i++) {                                         \
        uint64_t value = (uint64_t)(mype + 1);                                 \
        shmem_put(&wait_vars[mype], &value, 1, i);                             \
      }                                                                        \
      shmem_quiet();                                                           \
      log_info("PE %d: Completed sending to all PEs", mype);                   \
                                                                               \
      int nrecv = 0, errors = 0;                                               \
      log_info("PE %d: Starting to wait for messages from all PEs", mype);     \
      /* Wait for all messages to arrive using shmem_signal_wait_until */      \
      for (int who = 0; who < npes; who++) {                                   \
        if (wait_vars[who] == 0) {                                             \
          log_info("PE %d: Waiting for message from PE %d", mype, who);        \
          shmem_signal_wait_until(&wait_vars[who], SHMEM_CMP_NE, 0);           \
        }                                                                      \
        uint64_t expected = (uint64_t)(who + 1);                               \
        if (wait_vars[who] != expected) {                                      \
          log_fail("PE %d: wait_vars[%d] = %lu, expected %lu", mype, who,      \
                   wait_vars[who], expected);                                  \
          errors++;                                                            \
        } else {                                                               \
          log_info("PE %d: Received correct value %lu from PE %d", mype,       \
                   wait_vars[who], who);                                       \
        }                                                                      \
        nrecv++;                                                               \
      }                                                                        \
      log_info("PE %d: Received %d messages with %d errors", mype, nrecv,      \
               errors);                                                        \
      if (errors > 0)                                                          \
        success = false;                                                       \
      log_info("Freeing allocated memory at %p", (void *)wait_vars);           \
      shmem_free(wait_vars);                                                   \
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
