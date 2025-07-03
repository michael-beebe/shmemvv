/**
 * @file c11_shmem_test_any.c
 * @brief Tests the shmem_test_any() routine.
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

/* Wait for any entry in the given ivar array to match the wait criteria and
 * return the index of the entry that satisfied the test. */
static int wait_any_test_c11(void *ivar, int count, int cmp, long value,
                             const char *typename) {
  int idx = -1;
  int iterations = 0;
  time_t start_time = time(NULL);

  if (strcmp(typename, "int") == 0) {
    int *arr = (int *)ivar;
    while ((idx = shmem_test_any(arr, count, NULL, cmp, (int)value)) ==
           SIZE_MAX) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_any_test timed out after %d iterations", iterations);
        return -1;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "long") == 0) {
    long *arr = (long *)ivar;
    while ((idx = shmem_test_any(arr, count, NULL, cmp, value)) == SIZE_MAX) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_any_test timed out after %d iterations", iterations);
        return -1;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "longlong") == 0) {
    long long *arr = (long long *)ivar;
    while ((idx = shmem_test_any(arr, count, NULL, cmp, (long long)value)) ==
           SIZE_MAX) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_any_test timed out after %d iterations", iterations);
        return -1;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "uint") == 0) {
    unsigned int *arr = (unsigned int *)ivar;
    while ((idx = shmem_test_any(arr, count, NULL, cmp, (unsigned int)value)) ==
           SIZE_MAX) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_any_test timed out after %d iterations", iterations);
        return -1;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "ulong") == 0) {
    unsigned long *arr = (unsigned long *)ivar;
    while ((idx = shmem_test_any(arr, count, NULL, cmp,
                                 (unsigned long)value)) == SIZE_MAX) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_any_test timed out after %d iterations", iterations);
        return -1;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "ulonglong") == 0) {
    unsigned long long *arr = (unsigned long long *)ivar;
    while ((idx = shmem_test_any(arr, count, NULL, cmp,
                                 (unsigned long long)value)) == SIZE_MAX) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_any_test timed out after %d iterations", iterations);
        return -1;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "int32") == 0) {
    int32_t *arr = (int32_t *)ivar;
    while ((idx = shmem_test_any(arr, count, NULL, cmp, (int32_t)value)) ==
           SIZE_MAX) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_any_test timed out after %d iterations", iterations);
        return -1;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "int64") == 0) {
    int64_t *arr = (int64_t *)ivar;
    while ((idx = shmem_test_any(arr, count, NULL, cmp, (int64_t)value)) ==
           SIZE_MAX) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_any_test timed out after %d iterations", iterations);
        return -1;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "uint32") == 0) {
    uint32_t *arr = (uint32_t *)ivar;
    while ((idx = shmem_test_any(arr, count, NULL, cmp, (uint32_t)value)) ==
           SIZE_MAX) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_any_test timed out after %d iterations", iterations);
        return -1;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "uint64") == 0) {
    uint64_t *arr = (uint64_t *)ivar;
    while ((idx = shmem_test_any(arr, count, NULL, cmp, (uint64_t)value)) ==
           SIZE_MAX) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_any_test timed out after %d iterations", iterations);
        return -1;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "size") == 0) {
    size_t *arr = (size_t *)ivar;
    while ((idx = shmem_test_any(arr, count, NULL, cmp, (size_t)value)) ==
           SIZE_MAX) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_any_test timed out after %d iterations", iterations);
        return -1;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "ptrdiff") == 0) {
    ptrdiff_t *arr = (ptrdiff_t *)ivar;
    while ((idx = shmem_test_any(arr, count, NULL, cmp, (ptrdiff_t)value)) ==
           SIZE_MAX) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_any_test timed out after %d iterations", iterations);
        return -1;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  }

  return idx;
}

#define TEST_C11_SHMEM_TEST_ANY(TYPE)                                          \
  ({                                                                           \
    log_routine("c11_shmem_test_any(" #TYPE ")");                              \
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
      /* Wait for all messages to arrive using shmem_test_any */               \
      while (nrecv < npes) {                                                   \
        /* Map TYPE to typename string for wait_any_test_c11 */                \
        const char *typename;                                                  \
        if (sizeof(TYPE) == sizeof(int) &&                                     \
            _Generic((TYPE)0, int: 1, default: 0))                             \
          typename = "int";                                                    \
        else if (sizeof(TYPE) == sizeof(long) &&                               \
                 _Generic((TYPE)0, long: 1, default: 0))                       \
          typename = "long";                                                   \
        else if (sizeof(TYPE) == sizeof(long long) &&                          \
                 _Generic((TYPE)0, long long: 1, default: 0))                  \
          typename = "longlong";                                               \
        else if (sizeof(TYPE) == sizeof(unsigned int) &&                       \
                 _Generic((TYPE)0, unsigned int: 1, default: 0))               \
          typename = "uint";                                                   \
        else if (sizeof(TYPE) == sizeof(unsigned long) &&                      \
                 _Generic((TYPE)0, unsigned long: 1, default: 0))              \
          typename = "ulong";                                                  \
        else if (sizeof(TYPE) == sizeof(unsigned long long) &&                 \
                 _Generic((TYPE)0, unsigned long long: 1, default: 0))         \
          typename = "ulonglong";                                              \
        else if (sizeof(TYPE) == sizeof(int32_t) &&                            \
                 _Generic((TYPE)0, int32_t: 1, default: 0))                    \
          typename = "int32";                                                  \
        else if (sizeof(TYPE) == sizeof(int64_t) &&                            \
                 _Generic((TYPE)0, int64_t: 1, default: 0))                    \
          typename = "int64";                                                  \
        else if (sizeof(TYPE) == sizeof(uint32_t) &&                           \
                 _Generic((TYPE)0, uint32_t: 1, default: 0))                   \
          typename = "uint32";                                                 \
        else if (sizeof(TYPE) == sizeof(uint64_t) &&                           \
                 _Generic((TYPE)0, uint64_t: 1, default: 0))                   \
          typename = "uint64";                                                 \
        else if (sizeof(TYPE) == sizeof(size_t) &&                             \
                 _Generic((TYPE)0, size_t: 1, default: 0))                     \
          typename = "size";                                                   \
        else if (sizeof(TYPE) == sizeof(ptrdiff_t) &&                          \
                 _Generic((TYPE)0, ptrdiff_t: 1, default: 0))                  \
          typename = "ptrdiff";                                                \
        else                                                                   \
          typename = "unknown";                                                \
                                                                               \
        int who =                                                              \
            wait_any_test_c11(wait_vars, npes, SHMEM_CMP_NE, 0, typename);     \
        if (who == -1) {                                                       \
          log_fail("PE %d: wait_any_test_c11 timed out", mype);                \
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

  // result &= TEST_C11_SHMEM_TEST_ANY(short);
  result &= TEST_C11_SHMEM_TEST_ANY(int);
  result &= TEST_C11_SHMEM_TEST_ANY(long);
  result &= TEST_C11_SHMEM_TEST_ANY(long long);
  // result &= TEST_C11_SHMEM_TEST_ANY(unsigned short);
  result &= TEST_C11_SHMEM_TEST_ANY(unsigned int);
  result &= TEST_C11_SHMEM_TEST_ANY(unsigned long);
  result &= TEST_C11_SHMEM_TEST_ANY(unsigned long long);
  result &= TEST_C11_SHMEM_TEST_ANY(int32_t);
  result &= TEST_C11_SHMEM_TEST_ANY(int64_t);
  result &= TEST_C11_SHMEM_TEST_ANY(uint32_t);
  result &= TEST_C11_SHMEM_TEST_ANY(uint64_t);
  result &= TEST_C11_SHMEM_TEST_ANY(size_t);
  result &= TEST_C11_SHMEM_TEST_ANY(ptrdiff_t);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_test_any", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
