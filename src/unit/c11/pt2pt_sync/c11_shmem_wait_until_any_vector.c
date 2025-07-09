/**
 * @file c11_shmem_wait_until_any_vector.c
 * @brief Unit test shmem_wait_until_any_vector() routine.
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

#define TEST_C11_SHMEM_WAIT_UNTIL_ANY_VECTOR(TYPE)                             \
  ({                                                                           \
    log_routine("c11_shmem_wait_until_any_vector(" #TYPE ")");                 \
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
      /* Wait for messages to arrive using shmem_wait_until_any_vector */      \
      while (nrecv < npes) {                                                   \
        TYPE cmp_values[npes];                                                 \
        /* Set up comparison values - wait for each PE's expected value */     \
        for (int i = 0; i < npes; i++) {                                       \
          cmp_values[i] = (TYPE)(i + 1);                                       \
        }                                                                      \
        log_info("PE %d: Waiting for any message using "                       \
                 "shmem_wait_until_any_vector",                                \
                 mype);                                                        \
        size_t index = shmem_wait_until_any_vector(wait_vars, npes, NULL,      \
                                                   SHMEM_CMP_EQ, cmp_values);  \
        if (index == SIZE_MAX) {                                               \
          log_fail("PE %d: shmem_wait_until_any_vector returned SIZE_MAX",     \
                   mype);                                                      \
          errors++;                                                            \
          break;                                                               \
        }                                                                      \
        log_info("PE %d: shmem_wait_until_any_vector found message at "        \
                 "index %zu",                                                  \
                 mype, index);                                                 \
        TYPE expected = (TYPE)(index + 1);                                     \
        if (wait_vars[index] != expected) {                                    \
          log_fail("PE %d: wait_vars[%zu] = %d, expected %d", mype, index,     \
                   (int)wait_vars[index], (int)expected);                      \
          errors++;                                                            \
        } else {                                                               \
          log_info("PE %d: Received correct value %d from PE %zu", mype,       \
                   (int)wait_vars[index], index);                              \
        }                                                                      \
        wait_vars[index] = 0; /* Reset so we don't find it again */            \
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

  result &= TEST_C11_SHMEM_WAIT_UNTIL_ANY_VECTOR(int);
  result &= TEST_C11_SHMEM_WAIT_UNTIL_ANY_VECTOR(long);
  result &= TEST_C11_SHMEM_WAIT_UNTIL_ANY_VECTOR(long long);
  result &= TEST_C11_SHMEM_WAIT_UNTIL_ANY_VECTOR(unsigned int);
  result &= TEST_C11_SHMEM_WAIT_UNTIL_ANY_VECTOR(unsigned long);
  result &= TEST_C11_SHMEM_WAIT_UNTIL_ANY_VECTOR(unsigned long long);
  result &= TEST_C11_SHMEM_WAIT_UNTIL_ANY_VECTOR(int32_t);
  result &= TEST_C11_SHMEM_WAIT_UNTIL_ANY_VECTOR(int64_t);
  result &= TEST_C11_SHMEM_WAIT_UNTIL_ANY_VECTOR(uint32_t);
  result &= TEST_C11_SHMEM_WAIT_UNTIL_ANY_VECTOR(uint64_t);
  result &= TEST_C11_SHMEM_WAIT_UNTIL_ANY_VECTOR(size_t);
  result &= TEST_C11_SHMEM_WAIT_UNTIL_ANY_VECTOR(ptrdiff_t);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_wait_until_any_vector", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
