/**
 * @file c_shmem_test.c
 * @brief Unit test shmem_test() routine.
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "log.h"
#include "shmemvv.h"

// Reduce timeout to speed up tests
#define TIMEOUT 10
// Reduce sleep time between iterations
#define SLEEP_USEC 100

/* Wait for any entry in the given ivar array to match the wait criteria and
 * return the index of the entry that satisfied the test. */
static int wait_any(void *ivar, int count, int cmp, long value,
                    const char *typename) {
  int idx = 0;
  int iterations = 0;
  time_t start_time = time(NULL);

  if (strcmp(typename, "int") == 0) {
    int *arr = (int *)ivar;
    while (!shmem_int_test(&arr[idx], cmp, (int)value)) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_any timed out after %d iterations", iterations);
        return -1;
      }
      idx = (idx + 1) % count;
      iterations++;
      if (iterations % 1000 == 0)
        usleep(SLEEP_USEC);
    }
  } else if (strcmp(typename, "long") == 0) {
    long *arr = (long *)ivar;
    while (!shmem_long_test(&arr[idx], cmp, value)) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_any timed out after %d iterations", iterations);
        return -1;
      }
      idx = (idx + 1) % count;
      iterations++;
      if (iterations % 1000 == 0)
        usleep(SLEEP_USEC);
    }
  } else if (strcmp(typename, "longlong") == 0) {
    long long *arr = (long long *)ivar;
    while (!shmem_longlong_test(&arr[idx], cmp, (long long)value)) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_any timed out after %d iterations", iterations);
        return -1;
      }
      idx = (idx + 1) % count;
      iterations++;
      if (iterations % 1000 == 0)
        usleep(SLEEP_USEC);
    }
  } else if (strcmp(typename, "uint") == 0) {
    unsigned int *arr = (unsigned int *)ivar;
    while (!shmem_uint_test(&arr[idx], cmp, (unsigned int)value)) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_any timed out after %d iterations", iterations);
        return -1;
      }
      idx = (idx + 1) % count;
      iterations++;
      if (iterations % 1000 == 0)
        usleep(SLEEP_USEC);
    }
  } else if (strcmp(typename, "ulong") == 0) {
    unsigned long *arr = (unsigned long *)ivar;
    while (!shmem_ulong_test(&arr[idx], cmp, (unsigned long)value)) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_any timed out after %d iterations", iterations);
        return -1;
      }
      idx = (idx + 1) % count;
      iterations++;
      if (iterations % 1000 == 0)
        usleep(SLEEP_USEC);
    }
  } else if (strcmp(typename, "ulonglong") == 0) {
    unsigned long long *arr = (unsigned long long *)ivar;
    while (!shmem_ulonglong_test(&arr[idx], cmp, (unsigned long long)value)) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_any timed out after %d iterations", iterations);
        return -1;
      }
      idx = (idx + 1) % count;
      iterations++;
      if (iterations % 1000 == 0)
        usleep(SLEEP_USEC);
    }
  } else if (strcmp(typename, "int32") == 0) {
    int32_t *arr = (int32_t *)ivar;
    while (!shmem_int32_test(&arr[idx], cmp, (int32_t)value)) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_any timed out after %d iterations", iterations);
        return -1;
      }
      idx = (idx + 1) % count;
      iterations++;
      if (iterations % 1000 == 0)
        usleep(SLEEP_USEC);
    }
  } else if (strcmp(typename, "int64") == 0) {
    int64_t *arr = (int64_t *)ivar;
    while (!shmem_int64_test(&arr[idx], cmp, (int64_t)value)) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_any timed out after %d iterations", iterations);
        return -1;
      }
      idx = (idx + 1) % count;
      iterations++;
      if (iterations % 1000 == 0)
        usleep(SLEEP_USEC);
    }
  } else if (strcmp(typename, "uint32") == 0) {
    uint32_t *arr = (uint32_t *)ivar;
    while (!shmem_uint32_test(&arr[idx], cmp, (uint32_t)value)) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_any timed out after %d iterations", iterations);
        return -1;
      }
      idx = (idx + 1) % count;
      iterations++;
      if (iterations % 1000 == 0)
        usleep(SLEEP_USEC);
    }
  } else if (strcmp(typename, "uint64") == 0) {
    uint64_t *arr = (uint64_t *)ivar;
    while (!shmem_uint64_test(&arr[idx], cmp, (uint64_t)value)) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_any timed out after %d iterations", iterations);
        return -1;
      }
      idx = (idx + 1) % count;
      iterations++;
      if (iterations % 1000 == 0)
        usleep(SLEEP_USEC);
    }
  } else if (strcmp(typename, "size") == 0) {
    size_t *arr = (size_t *)ivar;
    while (!shmem_size_test(&arr[idx], cmp, (size_t)value)) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_any timed out after %d iterations", iterations);
        return -1;
      }
      idx = (idx + 1) % count;
      iterations++;
      if (iterations % 1000 == 0)
        usleep(SLEEP_USEC);
    }
  } else if (strcmp(typename, "ptrdiff") == 0) {
    ptrdiff_t *arr = (ptrdiff_t *)ivar;
    while (!shmem_ptrdiff_test(&arr[idx], cmp, (ptrdiff_t)value)) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_any timed out after %d iterations", iterations);
        return -1;
      }
      idx = (idx + 1) % count;
      iterations++;
      if (iterations % 1000 == 0)
        usleep(SLEEP_USEC);
    }
  }

  return idx;
}

#define TEST_C_SHMEM_TEST(TYPE, TYPENAME)                                      \
  ({                                                                           \
    log_routine("shmem_test(" #TYPE ")");                                      \
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
      /* Wait for all messages to arrive */                                    \
      while (nrecv < npes) {                                                   \
        int who = wait_any(wait_vars, npes, SHMEM_CMP_NE, 0, #TYPENAME);       \
        if (who == -1) {                                                       \
          log_fail("PE %d: wait_any timed out", mype);                         \
          errors++;                                                            \
          break;                                                               \
        }                                                                      \
        TYPE expected = (TYPE)(who + 1);                                       \
        if (wait_vars[who] != expected) {                                      \
          log_fail("PE %d: wait_vars[%d] = %d, expected %d", mype, who,        \
                   (int)wait_vars[who], (int)expected);                        \
          errors++;                                                            \
        } else {                                                               \
          log_info("PE %d: Received correct value %d from PE %d", mype,        \
                   (int)wait_vars[who], who);                                  \
        }                                                                      \
        wait_vars[who] = 0;                                                    \
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

  // result &= TEST_C_SHMEM_TEST(short, short);
  result &= TEST_C_SHMEM_TEST(int, int);
  result &= TEST_C_SHMEM_TEST(long, long);
  result &= TEST_C_SHMEM_TEST(long long, longlong);
  // result &= TEST_C_SHMEM_TEST(unsigned short, ushort);
  result &= TEST_C_SHMEM_TEST(unsigned int, uint);
  result &= TEST_C_SHMEM_TEST(unsigned long, ulong);
  result &= TEST_C_SHMEM_TEST(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_TEST(int32_t, int32);
  result &= TEST_C_SHMEM_TEST(int64_t, int64);
  result &= TEST_C_SHMEM_TEST(uint32_t, uint32);
  result &= TEST_C_SHMEM_TEST(uint64_t, uint64);
  result &= TEST_C_SHMEM_TEST(size_t, size);
  result &= TEST_C_SHMEM_TEST(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_test", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
