/**
 * @file c_shmem_test.c
 * @brief Unit test shmem_test() routine.
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "log.h"
#include "shmemvv.h"

#define TIMEOUT 2

#define TEST_C_SHMEM_TEST(TYPE, TYPENAME)                                      \
  ({                                                                           \
    log_routine("shmem_test(" #TYPE ")");                                      \
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
        while (!shmem_##TYPENAME##_test(flag, SHMEM_CMP_EQ, 1)) {              \
          if (time(NULL) - start_time > TIMEOUT) {                             \
            log_fail("PE %d: Test timed out after %d iterations", mype,        \
                     iterations);                                              \
            break;                                                             \
          }                                                                    \
          usleep(1000);                                                        \
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

  result &= TEST_C_SHMEM_TEST(short, short);
  result &= TEST_C_SHMEM_TEST(int, int);
  result &= TEST_C_SHMEM_TEST(long, long);
  result &= TEST_C_SHMEM_TEST(long long, longlong);
  result &= TEST_C_SHMEM_TEST(unsigned short, ushort);
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
    display_test_result("C shmem_test()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
