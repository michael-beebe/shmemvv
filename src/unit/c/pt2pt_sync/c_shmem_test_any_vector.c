/**
 * @file c_shmem_test_any_vector.c
 * @brief Unit test shmem_test_any_vector() routine.
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

/* Wait for any entry in the given ivar array to match the wait criteria using
 * vector comparison */

#define TEST_C_SHMEM_TEST_ANY_VECTOR(TYPE, TYPENAME)                           \
  ({                                                                           \
    log_routine("shmem_test_any_vector(" #TYPE ")");                           \
    bool success = true;                                                       \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    TYPE *wait_vars = (TYPE *)shmem_calloc(npes, sizeof(TYPE));                \
    int *status = (int *)calloc(npes, sizeof(int));                            \
    TYPE *cmp_values = (TYPE *)malloc(npes * sizeof(TYPE));                    \
    log_info("Allocated wait_vars array (%zu bytes) at address %p",            \
             npes * sizeof(TYPE), (void *)wait_vars);                          \
    if (wait_vars == NULL || status == NULL || cmp_values == NULL) {           \
      log_fail("Memory allocation failed");                                    \
      success = false;                                                         \
    } else {                                                                   \
      log_info("PE %d: Sending value %d to all PEs using shmem_atomic_set",    \
               mype, mype);                                                    \
      /* Each PE sends its PE number to all PEs using atomic_set */            \
      for (int i = 0; i < npes; i++) {                                         \
        shmem_##TYPENAME##_atomic_set(&wait_vars[mype], (TYPE)mype, i);        \
        cmp_values[i] = (TYPE)i; /* Expect PE i's value */                     \
      }                                                                        \
      log_info("PE %d: Completed sending to all PEs", mype);   \
      shmem_barrier_all();                 \
                                                                               \
      int nrecv = 0, errors = 0;                                               \
      int expected_sum = (npes - 1) * npes / 2;                                \
      int total_sum = 0;                                                       \
      log_info("PE %d: Starting to wait for messages from all PEs", mype);     \
      /* Wait for all messages to arrive using shmem_test_any_vector */        \
      while (nrecv < npes) {                                                   \
        log_info("PE %d: Waiting for any message using shmem_test_any_vector", \
                 mype);                                                        \
        size_t index = shmem_##TYPENAME##_test_any_vector(                     \
            wait_vars, npes, status, SHMEM_CMP_EQ, cmp_values);                \
        if (index == SIZE_MAX) {                                               \
          log_fail("PE %d: shmem_test_any_vector returned SIZE_MAX", mype);    \
          errors++;                                                            \
          break;                                                               \
        }                                                                      \
        log_info("PE %d: shmem_test_any_vector found message at index %zu",    \
                 mype, index);                                                 \
        TYPE expected = (TYPE)index;                                           \
        if (wait_vars[index] != expected) {                                    \
          log_fail("PE %d: wait_vars[%zu] = %d, expected %d", mype, index,     \
                   (int)wait_vars[index], (int)expected);                      \
          errors++;                                                            \
        } else {                                                               \
          log_info("PE %d: Received correct value %d from PE %zu", mype,       \
                   (int)wait_vars[index], index);                              \
          total_sum += wait_vars[index];                                       \
        }                                                                      \
        status[index] = 1; /* Mark as processed */                             \
        nrecv++;                                                               \
      }                                                                        \
      log_info("PE %d: Received %d messages with %d errors", mype, nrecv,      \
               errors);                                                        \
      log_info("PE %d: Expected sum %d, actual sum %d", mype, expected_sum,    \
               total_sum);                                                     \
      if (errors > 0 || total_sum != expected_sum)                             \
        success = false;                                                       \
      log_info("Freeing allocated memory");                                    \
      shmem_free(wait_vars);                                                   \
      free(status);                                                            \
      free(cmp_values);                                                        \
    }                                                                          \
    success;                                                                   \
  })

int main(int argc, char **argv) {
  shmem_init();
  log_init(__FILE__);

  int result = true;
  int rc = EXIT_SUCCESS;

  // result &= TEST_C_SHMEM_TEST_ANY_VECTOR(short, short);
  result &= TEST_C_SHMEM_TEST_ANY_VECTOR(int, int);
  result &= TEST_C_SHMEM_TEST_ANY_VECTOR(long, long);
  result &= TEST_C_SHMEM_TEST_ANY_VECTOR(long long, longlong);
  // result &= TEST_C_SHMEM_TEST_ANY_VECTOR(unsigned short, ushort);
  result &= TEST_C_SHMEM_TEST_ANY_VECTOR(unsigned int, uint);
  result &= TEST_C_SHMEM_TEST_ANY_VECTOR(unsigned long, ulong);
  result &= TEST_C_SHMEM_TEST_ANY_VECTOR(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_TEST_ANY_VECTOR(int32_t, int32);
  result &= TEST_C_SHMEM_TEST_ANY_VECTOR(int64_t, int64);
  result &= TEST_C_SHMEM_TEST_ANY_VECTOR(uint32_t, uint32);
  result &= TEST_C_SHMEM_TEST_ANY_VECTOR(uint64_t, uint64);
  result &= TEST_C_SHMEM_TEST_ANY_VECTOR(size_t, size);
  result &= TEST_C_SHMEM_TEST_ANY_VECTOR(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_test_any_vector", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
