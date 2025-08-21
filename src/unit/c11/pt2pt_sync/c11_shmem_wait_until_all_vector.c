/**
 * @file c11_shmem_wait_until_all_vector.c
 * @brief Unit test shmem_wait_until_all_vector() routine.
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

#define TEST_C11_SHMEM_WAIT_UNTIL_ALL_VECTOR(TYPE)                             \
  ({                                                                           \
    log_routine("c11_shmem_wait_until_all_vector(" #TYPE ")");                 \
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
      /* Ensure all PEs have completed their puts before proceeding */         \
      shmem_barrier_all();                                                     \
                                                                               \
      TYPE cmp_values[npes];                                                   \
      /* Set up comparison values - wait for each PE's expected value */       \
      for (int i = 0; i < npes; i++) {                                         \
        cmp_values[i] = (TYPE)(i + 1);                                         \
      }                                                                        \
      log_info("PE %d: Waiting for all messages to arrive using "              \
               "shmem_wait_until_all_vector",                                  \
               mype);                                                          \
      /* Wait for all messages to arrive using shmem_wait_until_all_vector */  \
      shmem_wait_until_all_vector(wait_vars, npes, NULL, SHMEM_CMP_EQ,         \
                                  cmp_values);                                 \
      log_info("PE %d: shmem_wait_until_all_vector completed", mype);          \
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
      /* Debug: show all values received */                                    \
      if (errors > 0) {                                                        \
        log_info("PE %d: Debug - all wait_vars values:", mype);                \
        for (int who = 0; who < npes; who++) {                                 \
          log_info("PE %d: wait_vars[%d] = %d", mype, who,                     \
                   (int)wait_vars[who]);                                       \
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

  // result &= TEST_C11_SHMEM_WAIT_UNTIL_ALL_VECTOR(short);
  result &= TEST_C11_SHMEM_WAIT_UNTIL_ALL_VECTOR(int);
  result &= TEST_C11_SHMEM_WAIT_UNTIL_ALL_VECTOR(long);
  result &= TEST_C11_SHMEM_WAIT_UNTIL_ALL_VECTOR(long long);
  // result &= TEST_C11_SHMEM_WAIT_UNTIL_ALL_VECTOR(unsigned short);
  result &= TEST_C11_SHMEM_WAIT_UNTIL_ALL_VECTOR(unsigned int);
  result &= TEST_C11_SHMEM_WAIT_UNTIL_ALL_VECTOR(unsigned long);
  result &= TEST_C11_SHMEM_WAIT_UNTIL_ALL_VECTOR(unsigned long long);
  result &= TEST_C11_SHMEM_WAIT_UNTIL_ALL_VECTOR(int32_t);
  result &= TEST_C11_SHMEM_WAIT_UNTIL_ALL_VECTOR(int64_t);
  result &= TEST_C11_SHMEM_WAIT_UNTIL_ALL_VECTOR(uint32_t);
  result &= TEST_C11_SHMEM_WAIT_UNTIL_ALL_VECTOR(uint64_t);
  result &= TEST_C11_SHMEM_WAIT_UNTIL_ALL_VECTOR(size_t);
  result &= TEST_C11_SHMEM_WAIT_UNTIL_ALL_VECTOR(ptrdiff_t);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_wait_until_all_vector", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
