/**
 * @file c11_shmem_test.c
 * @brief Unit test shmem_test() routine.
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "log.h"
#include "shmemvv.h"

// Reduce timeout to speed up tests
#define TIMEOUT 1
// Reduce sleep time between iterations
#define SLEEP_USEC 100

#define TEST_C11_SHMEM_TEST(TYPE)                                              \
  ({                                                                           \
    log_routine("c11_shmem_test(" #TYPE ")");                                  \
    bool success = true;                                                       \
    TYPE *flag = (TYPE *)shmem_malloc(sizeof(TYPE));                           \
    log_info("Allocated flag array (%zu bytes) at address %p", sizeof(TYPE),   \
             (void *)flag);                                                    \
    if (flag == NULL) {                                                        \
      log_fail("Memory allocation failed - shmem_malloc returned NULL");       \
      success = false;                                                         \
    } else {                                                                   \
      *flag = 0;                                                               \
      log_info("Initialized flag to 0");                                       \
      int mype = shmem_my_pe();                                                \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 0) {                                                         \
        log_info("PE 0: Setting flag to 1");                                   \
        *flag = 1;                                                             \
        shmem_quiet();                                                         \
        log_info("PE 0: Called shmem_quiet() after setting flag");             \
      }                                                                        \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype != 0) {                                                         \
        time_t start_time = time(NULL);                                        \
        int iterations = 0;                                                    \
        log_info(                                                              \
            "PE %d: Starting test loop (flag=%p, condition=SHMEM_CMP_EQ, "     \
            "target=1)",                                                       \
            mype, (void *)flag);                                               \
        int aggressive_polling = 0;                                            \
        while (!shmem_test(flag, SHMEM_CMP_EQ, 1)) {                           \
          if (time(NULL) - start_time > TIMEOUT) {                             \
            log_fail("PE %d: Test timed out after %d iterations", mype,        \
                     iterations);                                              \
            break;                                                             \
          }                                                                    \
          /* More aggressive polling initially, then backoff */                \
          if (aggressive_polling < 1000) {                                     \
            aggressive_polling++;                                              \
          } else {                                                             \
            usleep(SLEEP_USEC);                                                \
          }                                                                    \
          iterations++;                                                        \
        }                                                                      \
        log_info("PE %d: Test loop completed after %d iterations", mype,       \
                 iterations);                                                  \
        if (*flag != 1) {                                                      \
          log_fail("PE %d: Validation failed - flag=%d, expected 1", mype,     \
                   (int)*flag);                                                \
          success = false;                                                     \
        } else {                                                               \
          log_info("PE %d: Successfully validated flag=1", mype);              \
        }                                                                      \
      }                                                                        \
      log_info("Freeing allocated memory at %p", (void *)flag);                \
      shmem_free(flag);                                                        \
    }                                                                          \
    success;                                                                   \
  })

int main(int argc, char **argv) {
  shmem_init();
  log_init(__FILE__);

  int result = true;
  int rc = EXIT_SUCCESS;

  // result &= TEST_C11_SHMEM_TEST(short);
  result &= TEST_C11_SHMEM_TEST(int);
  result &= TEST_C11_SHMEM_TEST(long);
  result &= TEST_C11_SHMEM_TEST(long long);
  // result &= TEST_C11_SHMEM_TEST(unsigned short);
  result &= TEST_C11_SHMEM_TEST(unsigned int);
  result &= TEST_C11_SHMEM_TEST(unsigned long);
  result &= TEST_C11_SHMEM_TEST(unsigned long long);
  result &= TEST_C11_SHMEM_TEST(int32_t);
  result &= TEST_C11_SHMEM_TEST(int64_t);
  result &= TEST_C11_SHMEM_TEST(uint32_t);
  result &= TEST_C11_SHMEM_TEST(uint64_t);
  result &= TEST_C11_SHMEM_TEST(size_t);
  result &= TEST_C11_SHMEM_TEST(ptrdiff_t);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_test", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
