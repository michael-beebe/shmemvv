/**
 * @file c11_shmem_test_all.c
 * @brief Tests the shmem_test_all() routine.
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "log.h"
#include "shmemvv.h"

#define TIMEOUT 2

#define TEST_C11_SHMEM_TEST_ALL(TYPE)                                          \
  ({                                                                           \
    log_routine("c11_shmem_test_all(" #TYPE ")");                              \
    bool success = true;                                                       \
    TYPE *flags = (TYPE *)shmem_malloc(4 * sizeof(TYPE));                      \
    log_info("Allocated flags array (%zu bytes) at address %p",                \
             4 * sizeof(TYPE), (void *)flags);                                 \
    if (flags == NULL) {                                                       \
      log_fail("Memory allocation failed - shmem_malloc returned NULL");       \
      success = false;                                                         \
    } else {                                                                   \
      for (int i = 0; i < 4; ++i) {                                            \
        flags[i] = 0;                                                          \
      }                                                                        \
      log_info("Initialized all flags to 0");                                  \
      int mype = shmem_my_pe();                                                \
      int npes = shmem_n_pes();                                                \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 0) {                                                         \
        for (int pe = 1; pe < npes; ++pe) {                                    \
          log_info("PE 0: Setting flags to 1 on remote PE %d", pe);            \
          for (int i = 0; i < 4; ++i) {                                        \
            shmem_p(&flags[i], 1, pe);                                         \
          }                                                                    \
        }                                                                      \
        log_info("PE 0: Completed setting flags, calling shmem_quiet()");      \
        shmem_quiet();                                                         \
      }                                                                        \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype != 0) {                                                         \
        TYPE cmp_value = 1;                                                    \
        time_t start_time = time(NULL);                                        \
        int iterations = 0;                                                    \
        log_info("PE %d: Starting test_all loop (flags=%p, condition="         \
                 "SHMEM_CMP_EQ, target=1)",                                    \
                 mype, (void *)flags);                                         \
        while (!shmem_test_all(flags, 4, NULL, SHMEM_CMP_EQ, cmp_value)) {     \
          if (time(NULL) - start_time > TIMEOUT) {                             \
            log_fail("PE %d: Test timed out after %d iterations", mype,        \
                     iterations);                                              \
            break;                                                             \
          }                                                                    \
          usleep(500);                                                         \
          iterations++;                                                        \
        }                                                                      \
        log_info("PE %d: test_all loop completed after %d iterations", mype,   \
                 iterations);                                                  \
                                                                               \
        for (int i = 0; i < 4; ++i) {                                          \
          if (flags[i] != 1) {                                                 \
            log_fail("PE %d: Validation failed - flags[%d] = %d, expected 1",  \
                     mype, i, (int)flags[i]);                                  \
            success = false;                                                   \
            break;                                                             \
          }                                                                    \
        }                                                                      \
        if (success) {                                                         \
          log_info("PE %d: All flags successfully validated", mype);           \
        }                                                                      \
      }                                                                        \
      log_info("Freeing allocated memory at %p", (void *)flags);               \
      shmem_free(flags);                                                       \
    }                                                                          \
    success;                                                                   \
  })

int main(int argc, char **argv) {
  shmem_init();
  log_init(__FILE__);

  int result = true;
  int rc = EXIT_SUCCESS;

  // result &= TEST_C11_SHMEM_TEST_ALL(short);
  result &= TEST_C11_SHMEM_TEST_ALL(int);
  result &= TEST_C11_SHMEM_TEST_ALL(long);
  result &= TEST_C11_SHMEM_TEST_ALL(long long);
  // result &= TEST_C11_SHMEM_TEST_ALL(unsigned short);
  result &= TEST_C11_SHMEM_TEST_ALL(unsigned int);
  result &= TEST_C11_SHMEM_TEST_ALL(unsigned long);
  result &= TEST_C11_SHMEM_TEST_ALL(unsigned long long);
  result &= TEST_C11_SHMEM_TEST_ALL(int32_t);
  result &= TEST_C11_SHMEM_TEST_ALL(int64_t);
  result &= TEST_C11_SHMEM_TEST_ALL(uint32_t);
  result &= TEST_C11_SHMEM_TEST_ALL(uint64_t);
  result &= TEST_C11_SHMEM_TEST_ALL(size_t);
  result &= TEST_C11_SHMEM_TEST_ALL(ptrdiff_t);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_test_all", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
