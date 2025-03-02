/**
 * @file cxx_shmem_test.cpp
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

#define TEST_CXX_SHMEM_TEST(TYPE, TYPENAME)                                    \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_test()");                                 \
    bool success = true;                                                       \
    TYPE *flag = (TYPE *)shmem_malloc(sizeof(TYPE));                           \
    log_info("Allocated flag (%zu bytes) at address %p", sizeof(TYPE),         \
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
        *flag = 1;                                                             \
        shmem_quiet();                                                         \
      }                                                                        \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype != 0) {                                                         \
        time_t start_time = time(NULL);                                        \
        while (!shmem_##TYPENAME##_test(flag, SHMEM_CMP_EQ, 1)) {              \
          if (time(NULL) - start_time > TIMEOUT) {                             \
            log_warn("Test timed out after %d seconds", TIMEOUT);              \
            break;                                                             \
          }                                                                    \
          usleep(1000);                                                        \
        }                                                                      \
        if (*flag != 1) {                                                      \
          log_fail("Flag value %d does not match expected value 1",            \
                   (int)*flag);                                                \
          success = false;                                                     \
        }                                                                      \
      }                                                                        \
      shmem_free(flag);                                                        \
    }                                                                          \
    success;                                                                   \
  })

int main(int argc, char **argv) {
  shmem_init();
  log_init(__FILE__);

  int result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_CXX_SHMEM_TEST(short, short);
  result &= TEST_CXX_SHMEM_TEST(int, int);
  result &= TEST_CXX_SHMEM_TEST(long, long);
  result &= TEST_CXX_SHMEM_TEST(long long, longlong);
  result &= TEST_CXX_SHMEM_TEST(unsigned short, ushort);
  result &= TEST_CXX_SHMEM_TEST(unsigned int, uint);
  result &= TEST_CXX_SHMEM_TEST(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_TEST(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_TEST(int32_t, int32);
  result &= TEST_CXX_SHMEM_TEST(int64_t, int64);
  result &= TEST_CXX_SHMEM_TEST(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_TEST(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_TEST(size_t, size);
  result &= TEST_CXX_SHMEM_TEST(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_test()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
