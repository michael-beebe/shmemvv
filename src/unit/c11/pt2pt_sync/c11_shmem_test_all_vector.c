/**
 * @file c11_shmem_test_all_vector.c
 * @brief Tests the shmem_test_all_vector() routine.
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

/* Wait for all entries in the given ivar array to match the wait criteria using
 * vector comparison */
static bool wait_all_vector_test_c11(void *ivar, int count, int cmp,
                                     void *cmp_values, const char *typename) {
  int iterations = 0;
  time_t start_time = time(NULL);
  bool result = false;

  if (strcmp(typename, "int") == 0) {
    int *arr = (int *)ivar;
    int *values = (int *)cmp_values;
    while (!(result = shmem_test_all_vector(arr, count, NULL, cmp, values))) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_all_vector_test timed out after %d iterations",
                 iterations);
        return false;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "long") == 0) {
    long *arr = (long *)ivar;
    long *values = (long *)cmp_values;
    while (!(result = shmem_test_all_vector(arr, count, NULL, cmp, values))) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_all_vector_test timed out after %d iterations",
                 iterations);
        return false;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "longlong") == 0) {
    long long *arr = (long long *)ivar;
    long long *values = (long long *)cmp_values;
    while (!(result = shmem_test_all_vector(arr, count, NULL, cmp, values))) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_all_vector_test timed out after %d iterations",
                 iterations);
        return false;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "uint") == 0) {
    unsigned int *arr = (unsigned int *)ivar;
    unsigned int *values = (unsigned int *)cmp_values;
    while (!(result = shmem_test_all_vector(arr, count, NULL, cmp, values))) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_all_vector_test timed out after %d iterations",
                 iterations);
        return false;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "ulong") == 0) {
    unsigned long *arr = (unsigned long *)ivar;
    unsigned long *values = (unsigned long *)cmp_values;
    while (!(result = shmem_test_all_vector(arr, count, NULL, cmp, values))) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_all_vector_test timed out after %d iterations",
                 iterations);
        return false;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "ulonglong") == 0) {
    unsigned long long *arr = (unsigned long long *)ivar;
    unsigned long long *values = (unsigned long long *)cmp_values;
    while (!(result = shmem_test_all_vector(arr, count, NULL, cmp, values))) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_all_vector_test timed out after %d iterations",
                 iterations);
        return false;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "int32") == 0) {
    int32_t *arr = (int32_t *)ivar;
    int32_t *values = (int32_t *)cmp_values;
    while (!(result = shmem_test_all_vector(arr, count, NULL, cmp, values))) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_all_vector_test timed out after %d iterations",
                 iterations);
        return false;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "int64") == 0) {
    int64_t *arr = (int64_t *)ivar;
    int64_t *values = (int64_t *)cmp_values;
    while (!(result = shmem_test_all_vector(arr, count, NULL, cmp, values))) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_all_vector_test timed out after %d iterations",
                 iterations);
        return false;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "uint32") == 0) {
    uint32_t *arr = (uint32_t *)ivar;
    uint32_t *values = (uint32_t *)cmp_values;
    while (!(result = shmem_test_all_vector(arr, count, NULL, cmp, values))) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_all_vector_test timed out after %d iterations",
                 iterations);
        return false;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "uint64") == 0) {
    uint64_t *arr = (uint64_t *)ivar;
    uint64_t *values = (uint64_t *)cmp_values;
    while (!(result = shmem_test_all_vector(arr, count, NULL, cmp, values))) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_all_vector_test timed out after %d iterations",
                 iterations);
        return false;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "size") == 0) {
    size_t *arr = (size_t *)ivar;
    size_t *values = (size_t *)cmp_values;
    while (!(result = shmem_test_all_vector(arr, count, NULL, cmp, values))) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_all_vector_test timed out after %d iterations",
                 iterations);
        return false;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  } else if (strcmp(typename, "ptrdiff") == 0) {
    ptrdiff_t *arr = (ptrdiff_t *)ivar;
    ptrdiff_t *values = (ptrdiff_t *)cmp_values;
    while (!(result = shmem_test_all_vector(arr, count, NULL, cmp, values))) {
      if (time(NULL) - start_time > TIMEOUT) {
        log_fail("wait_all_vector_test timed out after %d iterations",
                 iterations);
        return false;
      }
      iterations++;
      if (iterations % 1000 == 0)
        usleep(100);
    }
  }

  return result;
}

#define TEST_C11_SHMEM_TEST_ALL_VECTOR(TYPE)                                   \
  ({                                                                           \
    log_routine("c11_shmem_test_all_vector(" #TYPE ")");                       \
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
      TYPE cmp_values[npes];                                                   \
      /* Set up comparison values - wait for each PE's expected value */       \
      for (int i = 0; i < npes; i++) {                                         \
        cmp_values[i] = (TYPE)(i + 1);                                         \
      }                                                                        \
      log_info("PE %d: Waiting for all messages to arrive using "              \
               "shmem_test_all_vector",                                        \
               mype);                                                          \
      /* Wait for all messages to arrive using shmem_test_all_vector */        \
      /* Map TYPE to typename string for wait_all_vector_test_c11 */           \
      const char *typename;                                                    \
      if (sizeof(TYPE) == sizeof(int) &&                                       \
          _Generic((TYPE)0, int: 1, default: 0))                               \
        typename = "int";                                                      \
      else if (sizeof(TYPE) == sizeof(long) &&                                 \
               _Generic((TYPE)0, long: 1, default: 0))                         \
        typename = "long";                                                     \
      else if (sizeof(TYPE) == sizeof(long long) &&                            \
               _Generic((TYPE)0, long long: 1, default: 0))                    \
        typename = "longlong";                                                 \
      else if (sizeof(TYPE) == sizeof(unsigned int) &&                         \
               _Generic((TYPE)0, unsigned int: 1, default: 0))                 \
        typename = "uint";                                                     \
      else if (sizeof(TYPE) == sizeof(unsigned long) &&                        \
               _Generic((TYPE)0, unsigned long: 1, default: 0))                \
        typename = "ulong";                                                    \
      else if (sizeof(TYPE) == sizeof(unsigned long long) &&                   \
               _Generic((TYPE)0, unsigned long long: 1, default: 0))           \
        typename = "ulonglong";                                                \
      else if (sizeof(TYPE) == sizeof(int32_t) &&                              \
               _Generic((TYPE)0, int32_t: 1, default: 0))                      \
        typename = "int32";                                                    \
      else if (sizeof(TYPE) == sizeof(int64_t) &&                              \
               _Generic((TYPE)0, int64_t: 1, default: 0))                      \
        typename = "int64";                                                    \
      else if (sizeof(TYPE) == sizeof(uint32_t) &&                             \
               _Generic((TYPE)0, uint32_t: 1, default: 0))                     \
        typename = "uint32";                                                   \
      else if (sizeof(TYPE) == sizeof(uint64_t) &&                             \
               _Generic((TYPE)0, uint64_t: 1, default: 0))                     \
        typename = "uint64";                                                   \
      else if (sizeof(TYPE) == sizeof(size_t) &&                               \
               _Generic((TYPE)0, size_t: 1, default: 0))                       \
        typename = "size";                                                     \
      else if (sizeof(TYPE) == sizeof(ptrdiff_t) &&                            \
               _Generic((TYPE)0, ptrdiff_t: 1, default: 0))                    \
        typename = "ptrdiff";                                                  \
      else                                                                     \
        typename = "unknown";                                                  \
                                                                               \
      bool all_received = wait_all_vector_test_c11(                            \
          wait_vars, npes, SHMEM_CMP_EQ, cmp_values, typename);                \
      if (!all_received) {                                                     \
        log_fail("PE %d: wait_all_vector_test_c11 timed out", mype);           \
        success = false;                                                       \
      } else {                                                                 \
        log_info("PE %d: shmem_test_all_vector completed successfully", mype); \
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

  // result &= TEST_C11_SHMEM_TEST_ALL_VECTOR(short);
  result &= TEST_C11_SHMEM_TEST_ALL_VECTOR(int);
  result &= TEST_C11_SHMEM_TEST_ALL_VECTOR(long);
  result &= TEST_C11_SHMEM_TEST_ALL_VECTOR(long long);
  // result &= TEST_C11_SHMEM_TEST_ALL_VECTOR(unsigned short);
  result &= TEST_C11_SHMEM_TEST_ALL_VECTOR(unsigned int);
  result &= TEST_C11_SHMEM_TEST_ALL_VECTOR(unsigned long);
  result &= TEST_C11_SHMEM_TEST_ALL_VECTOR(unsigned long long);
  result &= TEST_C11_SHMEM_TEST_ALL_VECTOR(int32_t);
  result &= TEST_C11_SHMEM_TEST_ALL_VECTOR(int64_t);
  result &= TEST_C11_SHMEM_TEST_ALL_VECTOR(uint32_t);
  result &= TEST_C11_SHMEM_TEST_ALL_VECTOR(uint64_t);
  result &= TEST_C11_SHMEM_TEST_ALL_VECTOR(size_t);
  result &= TEST_C11_SHMEM_TEST_ALL_VECTOR(ptrdiff_t);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_test_all_vector", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
