/**
 * @file c11_shmem_test_some.c
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
static size_t wait_some_test_c11(void *ivar, int count, size_t *indices,
                                 int cmp, long value, const char *typename) {
  int iterations = 0;
  time_t start_time = time(NULL);
  size_t result = 0;

  if (strcmp(typename, "int") == 0) {
    int *arr = (int *)ivar;
    while ((result = shmem_test_some(arr, count, indices, NULL, cmp,
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
    while ((result = shmem_test_some(arr, count, indices, NULL, cmp, value)) ==
           0) {
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
    while ((result = shmem_test_some(arr, count, indices, NULL, cmp,
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
    while ((result = shmem_test_some(arr, count, indices, NULL, cmp,
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
    while ((result = shmem_test_some(arr, count, indices, NULL, cmp,
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
    while ((result = shmem_test_some(arr, count, indices, NULL, cmp,
                                     (unsigned long long)value)) == 0) {
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
    while ((result = shmem_test_some(arr, count, indices, NULL, cmp,
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
    while ((result = shmem_test_some(arr, count, indices, NULL, cmp,
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
    while ((result = shmem_test_some(arr, count, indices, NULL, cmp,
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
    while ((result = shmem_test_some(arr, count, indices, NULL, cmp,
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
    while ((result = shmem_test_some(arr, count, indices, NULL, cmp,
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
    while ((result = shmem_test_some(arr, count, indices, NULL, cmp,
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

#define TEST_C11_SHMEM_TEST_SOME(TYPE)                                         \
  ({                                                                           \
    log_routine("c11_shmem_test_some(" #TYPE ")");                             \
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
        /* Map TYPE to typename string for wait_some_test_c11 */               \
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
        size_t num_ready = wait_some_test_c11(wait_vars, npes, indices,        \
                                              SHMEM_CMP_NE, 0, typename);      \
        if (num_ready == 0) {                                                  \
          log_fail("PE %d: wait_some_test_c11 timed out", mype);               \
          errors++;                                                            \
          break;                                                               \
        }                                                                      \
        log_info("PE %d: shmem_test_some found %zu ready variables", mype,     \
                 num_ready);                                                   \
        /* Process the ready variables */                                      \
        for (size_t i = 0; i < num_ready; i++) {                               \
          int who = indices[i];                                                \
          TYPE expected = (TYPE)(who + 1);                                     \
          if (wait_vars[who] != expected) {                                    \
            log_fail("PE %d: wait_vars[%d] = %d, expected %d", mype, who,      \
                     (int)wait_vars[who], (int)expected);                      \
            errors++;                                                          \
          } else {                                                             \
            log_info("PE %d: Received correct value %d from PE %d", mype,      \
                     (int)wait_vars[who], who);                                \
          }                                                                    \
          wait_vars[who] = 0;                                                  \
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

  // result &= TEST_C11_SHMEM_TEST_SOME(short);
  result &= TEST_C11_SHMEM_TEST_SOME(int);
  result &= TEST_C11_SHMEM_TEST_SOME(long);
  result &= TEST_C11_SHMEM_TEST_SOME(long long);
  // result &= TEST_C11_SHMEM_TEST_SOME(unsigned short);
  result &= TEST_C11_SHMEM_TEST_SOME(unsigned int);
  result &= TEST_C11_SHMEM_TEST_SOME(unsigned long);
  result &= TEST_C11_SHMEM_TEST_SOME(unsigned long long);
  result &= TEST_C11_SHMEM_TEST_SOME(int32_t);
  result &= TEST_C11_SHMEM_TEST_SOME(int64_t);
  result &= TEST_C11_SHMEM_TEST_SOME(uint32_t);
  result &= TEST_C11_SHMEM_TEST_SOME(uint64_t);
  result &= TEST_C11_SHMEM_TEST_SOME(size_t);
  result &= TEST_C11_SHMEM_TEST_SOME(ptrdiff_t);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_test_some", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
