/**
 * @file c_shmem_wait_until_some.c
 * @brief Unit test shmem_wait_until_some() routine.
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

#define TEST_C_SHMEM_WAIT_UNTIL_SOME(TYPE, TYPENAME)                           \
  ({                                                                           \
    log_routine("shmem_wait_until_some(" #TYPE ")");                           \
    bool success = true;                                                       \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    TYPE *wait_vars = (TYPE *)shmem_calloc(npes, sizeof(TYPE));                \
    log_info("Allocated wait_vars array (%zu bytes) at address %p",            \
             npes * sizeof(TYPE), (void *)wait_vars);                          \
    if (wait_vars == NULL) {                                                   \
      log_fail("Memory allocation failed - shmem_calloc returned NULL");       \
      success = false;                                                         \
    } else {                                                                   \
      log_info("PE %d: Sending value %d to all PEs", mype, (int)(mype + 1));   \
      /* Put mype+1 to every PE */                                             \
      for (int i = 0; i < npes; i++) {                                         \
        TYPE value = (TYPE)(mype + 1);                                         \
        shmem_put(&wait_vars[mype], &value, 1, i);                             \
      }                                                                        \
      shmem_quiet();                                                           \
      log_info("PE %d: Completed sending to all PEs", mype);                   \
                                                                               \
      int nrecv = 0, errors = 0;                                               \
      log_info("PE %d: Starting to wait for messages from all PEs", mype);     \
      /* Wait for messages to arrive using shmem_wait_until_some */            \
      while (nrecv < npes) {                                                   \
        size_t indices[npes];                                                  \
        log_info("PE %d: Waiting for some messages using "                     \
                 "shmem_wait_until_some",                                      \
                 mype);                                                        \
        size_t count = shmem_##TYPENAME##_wait_until_some(                     \
            wait_vars, npes, indices, NULL, SHMEM_CMP_NE, 0);                  \
        if (count == 0) {                                                      \
          log_fail("PE %d: shmem_wait_until_some returned 0", mype);           \
          errors++;                                                            \
          break;                                                               \
        }                                                                      \
        log_info("PE %d: shmem_wait_until_some found %zu messages", mype,      \
                 count);                                                       \
        /* Validate the found messages */                                      \
        for (size_t i = 0; i < count; i++) {                                   \
          size_t who = indices[i];                                             \
          TYPE expected = (TYPE)(who + 1);                                     \
          if (wait_vars[who] != expected) {                                    \
            log_fail("PE %d: wait_vars[%zu] = %d, expected %d", mype, who,     \
                     (int)wait_vars[who], (int)expected);                      \
            errors++;                                                          \
          } else {                                                             \
            log_info("PE %d: Received correct value %d from PE %zu", mype,     \
                     (int)wait_vars[who], who);                                \
          }                                                                    \
          wait_vars[who] = 0; /* Reset so we don't find it again */            \
          nrecv++;                                                             \
        }                                                                      \
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

  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME(int, int);
  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME(long, long);
  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME(long long, longlong);
  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME(unsigned int, uint);
  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME(unsigned long, ulong);
  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME(int32_t, int32);
  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME(int64_t, int64);
  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME(uint32_t, uint32);
  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME(uint64_t, uint64);
  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME(size_t, size);
  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_wait_until_some", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
