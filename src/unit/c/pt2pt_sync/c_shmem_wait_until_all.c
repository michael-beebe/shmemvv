/**
 * @file c_shmem_wait_until_all.c
 * @brief Unit test shmem_wait_until_all() routine.
 */

#include "log.h"
#include "shmemvv.h"
#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define TIMEOUT 10

#define TEST_C_SHMEM_WAIT_UNTIL_ALL(TYPE, TYPENAME)                            \
  ({                                                                           \
    log_routine("shmem_wait_until_all(" #TYPE ")");                            \
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
      log_info("PE %d: Waiting for all messages to arrive using "              \
               "shmem_wait_until_all",                                         \
               mype);                                                          \
      /* Wait for all messages to arrive using shmem_wait_until_all */         \
      shmem_##TYPENAME##_wait_until_all(wait_vars, npes, NULL, SHMEM_CMP_NE,   \
                                        0);                                    \
      log_info("PE %d: shmem_wait_until_all completed", mype);                 \
                                                                               \
      int errors = 0;                                                          \
      /* Validate all received messages */                                     \
      for (int who = 0; who < npes; who++) {                                   \
        TYPE expected = (TYPE)(who + 1);                                       \
        if (wait_vars[who] != expected) {                                      \
          log_fail("PE %d: wait_vars[%d] = %d, expected %d", mype, who,        \
                   (int)wait_vars[who], (int)expected);                        \
          errors++;                                                            \
        } else {                                                               \
          log_info("PE %d: Received correct value %d from PE %d", mype,        \
                   (int)wait_vars[who], who);                                  \
        }                                                                      \
      }                                                                        \
      log_info("PE %d: Validated %d messages with %d errors", mype, npes,      \
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

  // result &= TEST_C_SHMEM_WAIT_UNTIL_ALL(short, short);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL(int, int);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL(long, long);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL(long long, longlong);
  // result &= TEST_C_SHMEM_WAIT_UNTIL_ALL(unsigned short, ushort);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL(unsigned int, uint);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL(unsigned long, ulong);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL(int32_t, int32);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL(int64_t, int64);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL(uint32_t, uint32);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL(uint64_t, uint64);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL(size_t, size);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_wait_until_all", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
