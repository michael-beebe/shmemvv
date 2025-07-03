/**
 * @file c_shmem_test_some.c
 * @brief Unit test shmem_test_some() routine.
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

/* Wait for some entries in the given ivar array to match the wait criteria */
static size_t wait_some_test(void *ivar, int count, size_t *indices, int cmp,
                             long value, const char *typename) {
  int iterations = 0;
  time_t start_time = time(NULL);
  size_t result = 0;

  if (strcmp(typename, "int") == 0) {
    int *arr = (int *)ivar;
    while ((result = shmem_int_test_some(arr, count, indices, NULL, cmp,
                                         (int)value)) == 0) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_some_test timed out after %d iterations", iterations);
        return 0;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "long") == 0) {
    long *arr = (long *)ivar;
    while ((result = shmem_long_test_some(arr, count, indices, NULL, cmp,
                                          value)) == 0) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_some_test timed out after %d iterations", iterations);
        return 0;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "longlong") == 0) {
    long long *arr = (long long *)ivar;
    while ((result = shmem_longlong_test_some(arr, count, indices, NULL, cmp,
                                              (long long)value)) == 0) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_some_test timed out after %d iterations", iterations);
        return 0;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "uint") == 0) {
    unsigned int *arr = (unsigned int *)ivar;
    while ((result = shmem_uint_test_some(arr, count, indices, NULL, cmp,
                                          (unsigned int)value)) == 0) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_some_test timed out after %d iterations", iterations);
        return 0;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "ulong") == 0) {
    unsigned long *arr = (unsigned long *)ivar;
    while ((result = shmem_ulong_test_some(arr, count, indices, NULL, cmp,
                                           (unsigned long)value)) == 0) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_some_test timed out after %d iterations", iterations);
        return 0;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "ulonglong") == 0) {
    unsigned long long *arr = (unsigned long long *)ivar;
    while ((result = shmem_ulonglong_test_some(arr, count, indices, NULL, cmp,
                                               (unsigned long long)value)) ==
           0) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_some_test timed out after %d iterations", iterations);
        return 0;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "int32") == 0) {
    int32_t *arr = (int32_t *)ivar;
    while ((result = shmem_int32_test_some(arr, count, indices, NULL, cmp,
                                           (int32_t)value)) == 0) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_some_test timed out after %d iterations", iterations);
        return 0;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "int64") == 0) {
    int64_t *arr = (int64_t *)ivar;
    while ((result = shmem_int64_test_some(arr, count, indices, NULL, cmp,
                                           (int64_t)value)) == 0) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_some_test timed out after %d iterations", iterations);
        return 0;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "uint32") == 0) {
    uint32_t *arr = (uint32_t *)ivar;
    while ((result = shmem_uint32_test_some(arr, count, indices, NULL, cmp,
                                            (uint32_t)value)) == 0) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_some_test timed out after %d iterations", iterations);
        return 0;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "uint64") == 0) {
    uint64_t *arr = (uint64_t *)ivar;
    while ((result = shmem_uint64_test_some(arr, count, indices, NULL, cmp,
                                            (uint64_t)value)) == 0) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_some_test timed out after %d iterations", iterations);
        return 0;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "size") == 0) {
    size_t *arr = (size_t *)ivar;
    while ((result = shmem_size_test_some(arr, count, indices, NULL, cmp,
                                          (size_t)value)) == 0) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_some_test timed out after %d iterations", iterations);
        return 0;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "ptrdiff") == 0) {
    ptrdiff_t *arr = (ptrdiff_t *)ivar;
    while ((result = shmem_ptrdiff_test_some(arr, count, indices, NULL, cmp,
                                             (ptrdiff_t)value)) == 0) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_some_test timed out after %d iterations", iterations);
        return 0;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  }

  return result;
}

#define TEST_C_SHMEM_TEST_SOME(TYPE, TYPENAME)                                 \
  ({                                                                           \
    log_routine("shmem_test_some(" #TYPE ")");                                 \
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
      /* Wait for messages to arrive using shmem_test_some */                  \
      while (nrecv < npes) {                                                   \
        size_t indices[npes];                                                  \
        log_info("PE %d: Testing for some messages using shmem_test_some",     \
                 mype);                                                        \
        size_t count = wait_some_test(wait_vars, npes, indices, SHMEM_CMP_NE,  \
                                      0, #TYPENAME);                           \
        if (count == 0) {                                                      \
          log_fail("PE %d: wait_some_test timed out", mype);                   \
          errors++;                                                            \
          break;                                                               \
        }                                                                      \
        log_info("PE %d: shmem_test_some found %zu messages", mype, count);    \
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

  // result &= TEST_C_SHMEM_TEST_SOME(short, short);
  result &= TEST_C_SHMEM_TEST_SOME(int, int);
  result &= TEST_C_SHMEM_TEST_SOME(long, long);
  result &= TEST_C_SHMEM_TEST_SOME(long long, longlong);
  // result &= TEST_C_SHMEM_TEST_SOME(unsigned short, ushort);
  result &= TEST_C_SHMEM_TEST_SOME(unsigned int, uint);
  result &= TEST_C_SHMEM_TEST_SOME(unsigned long, ulong);
  result &= TEST_C_SHMEM_TEST_SOME(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_TEST_SOME(int32_t, int32);
  result &= TEST_C_SHMEM_TEST_SOME(int64_t, int64);
  result &= TEST_C_SHMEM_TEST_SOME(uint32_t, uint32);
  result &= TEST_C_SHMEM_TEST_SOME(uint64_t, uint64);
  result &= TEST_C_SHMEM_TEST_SOME(size_t, size);
  result &= TEST_C_SHMEM_TEST_SOME(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_test_some", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
