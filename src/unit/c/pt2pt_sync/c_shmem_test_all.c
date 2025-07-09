/**
 * @file c_shmem_test_all.c
 * @brief Tests the shmem_test_all() routine.
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

/* Wait for all entries in the given ivar array to match the wait criteria */
static bool wait_all_test(void *ivar, int count, int cmp, long value,
                          const char *typename) {
  int iterations = 0;
  time_t start_time = time(NULL);
  bool result = false;

  if (strcmp(typename, "int") == 0) {
    int *arr = (int *)ivar;
    while (!(result = shmem_int_test_all(arr, count, NULL, cmp, (int)value))) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_all_test timed out after %d iterations", iterations);
        return false;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "long") == 0) {
    long *arr = (long *)ivar;
    while (!(result = shmem_long_test_all(arr, count, NULL, cmp, value))) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_all_test timed out after %d iterations", iterations);
        return false;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "longlong") == 0) {
    long long *arr = (long long *)ivar;
    while (!(result = shmem_longlong_test_all(arr, count, NULL, cmp,
                                              (long long)value))) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_all_test timed out after %d iterations", iterations);
        return false;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "uint") == 0) {
    unsigned int *arr = (unsigned int *)ivar;
    while (!(result = shmem_uint_test_all(arr, count, NULL, cmp,
                                          (unsigned int)value))) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_all_test timed out after %d iterations", iterations);
        return false;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "ulong") == 0) {
    unsigned long *arr = (unsigned long *)ivar;
    while (!(result = shmem_ulong_test_all(arr, count, NULL, cmp,
                                           (unsigned long)value))) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_all_test timed out after %d iterations", iterations);
        return false;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "ulonglong") == 0) {
    unsigned long long *arr = (unsigned long long *)ivar;
    while (!(result = shmem_ulonglong_test_all(arr, count, NULL, cmp,
                                               (unsigned long long)value))) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_all_test timed out after %d iterations", iterations);
        return false;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "int32") == 0) {
    int32_t *arr = (int32_t *)ivar;
    while (!(result =
                 shmem_int32_test_all(arr, count, NULL, cmp, (int32_t)value))) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_all_test timed out after %d iterations", iterations);
        return false;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "int64") == 0) {
    int64_t *arr = (int64_t *)ivar;
    while (!(result =
                 shmem_int64_test_all(arr, count, NULL, cmp, (int64_t)value))) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_all_test timed out after %d iterations", iterations);
        return false;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "uint32") == 0) {
    uint32_t *arr = (uint32_t *)ivar;
    while (!(result = shmem_uint32_test_all(arr, count, NULL, cmp,
                                            (uint32_t)value))) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_all_test timed out after %d iterations", iterations);
        return false;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "uint64") == 0) {
    uint64_t *arr = (uint64_t *)ivar;
    while (!(result = shmem_uint64_test_all(arr, count, NULL, cmp,
                                            (uint64_t)value))) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_all_test timed out after %d iterations", iterations);
        return false;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "size") == 0) {
    size_t *arr = (size_t *)ivar;
    while (
        !(result = shmem_size_test_all(arr, count, NULL, cmp, (size_t)value))) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_all_test timed out after %d iterations", iterations);
        return false;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "ptrdiff") == 0) {
    ptrdiff_t *arr = (ptrdiff_t *)ivar;
    while (!(result = shmem_ptrdiff_test_all(arr, count, NULL, cmp,
                                             (ptrdiff_t)value))) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_all_test timed out after %d iterations", iterations);
        return false;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  }

  return result;
}

#define TEST_C_SHMEM_TEST_ALL(TYPE, TYPENAME)                                  \
  ({                                                                           \
    log_routine("shmem_test_all(" #TYPE ")");                                  \
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
               "shmem_test_all",                                               \
               mype);                                                          \
      /* Wait for all messages to arrive using shmem_test_all */               \
      bool all_received =                                                      \
          wait_all_test(wait_vars, npes, SHMEM_CMP_NE, 0, #TYPENAME);          \
      if (!all_received) {                                                     \
        log_fail("PE %d: wait_all_test timed out", mype);                      \
        success = false;                                                       \
      } else {                                                                 \
        log_info("PE %d: shmem_test_all completed successfully", mype);        \
                                                                               \
        int errors = 0;                                                        \
        /* Validate all received messages */                                   \
        for (int who = 0; who < npes; who++) {                                 \
          TYPE expected = (TYPE)(who + 1);                                     \
          if (wait_vars[who] != expected) {                                    \
            log_fail("PE %d: wait_vars[%d] = %d, expected %d", mype, who,      \
                     (int)wait_vars[who], (int)expected);                      \
            errors++;                                                          \
          } else {                                                             \
            log_info("PE %d: Received correct value %d from PE %d", mype,      \
                     (int)wait_vars[who], who);                                \
          }                                                                    \
        }                                                                      \
        log_info("PE %d: Validated %d messages with %d errors", mype, npes,    \
                 errors);                                                      \
        if (errors > 0)                                                        \
          success = false;                                                     \
      }                                                                        \
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

  // result &= TEST_C_SHMEM_TEST_ALL(short, short);
  result &= TEST_C_SHMEM_TEST_ALL(int, int);
  result &= TEST_C_SHMEM_TEST_ALL(long, long);
  result &= TEST_C_SHMEM_TEST_ALL(long long, longlong);
  // result &= TEST_C_SHMEM_TEST_ALL(unsigned short, ushort);
  result &= TEST_C_SHMEM_TEST_ALL(unsigned int, uint);
  result &= TEST_C_SHMEM_TEST_ALL(unsigned long, ulong);
  result &= TEST_C_SHMEM_TEST_ALL(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_TEST_ALL(int32_t, int32);
  result &= TEST_C_SHMEM_TEST_ALL(int64_t, int64);
  result &= TEST_C_SHMEM_TEST_ALL(uint32_t, uint32);
  result &= TEST_C_SHMEM_TEST_ALL(uint64_t, uint64);
  result &= TEST_C_SHMEM_TEST_ALL(size_t, size);
  result &= TEST_C_SHMEM_TEST_ALL(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_test_all", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
