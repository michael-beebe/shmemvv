/**
 * @file cxx_shmem_test_all_vector.cpp
 * @brief Tests the shmem_test_all_vector() routine.
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "log.h"
#include "shmemvv.h"

#define TIMEOUT 2
#define TEST_CXX_SHMEM_TEST_ALL_VECTOR(TYPE, TYPENAME)                         \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_test_all_vector()");                      \
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
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 0) {                                                         \
        for (int i = 0; i < 4; ++i) {                                          \
          flags[i] = 1;                                                        \
        }                                                                      \
        log_info("PE 0: Set all flags to 1");                                  \
        shmem_quiet();                                                         \
        log_info("PE 0: Called shmem_quiet() after setting flags");            \
      }                                                                        \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype != 0) {                                                         \
        TYPE cmp_values[4] = {1, 1, 1, 1};                                     \
        time_t start_time = time(NULL);                                        \
        log_info("PE %d: Starting test_all_vector (flags=%p, n=4, "            \
                 "condition=SHMEM_CMP_EQ, target=1)",                          \
                 mype, (void *)flags);                                         \
        while (!shmem_##TYPENAME##_test_all_vector(                            \
            flags, 4, NULL, SHMEM_CMP_EQ, cmp_values)) {                       \
          if (time(NULL) - start_time > TIMEOUT) {                             \
            log_fail("PE %d: Test timed out after %d seconds", mype, TIMEOUT); \
            break;                                                             \
          }                                                                    \
          usleep(1000);                                                        \
        }                                                                      \
        log_info("PE %d: test_all_vector completed", mype);                    \
        for (int i = 0; i < 4; ++i) {                                          \
          if (flags[i] != 1) {                                                 \
            log_fail("PE %d: Test failed - flags[%d] = %d, expected 1", mype,  \
                     i, (int)flags[i]);                                        \
            success = false;                                                   \
            break;                                                             \
          }                                                                    \
        }                                                                      \
        if (success) {                                                         \
          log_info("PE %d: Successfully received all expected flag values",    \
                   mype);                                                      \
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

  result &= TEST_CXX_SHMEM_TEST_ALL_VECTOR(short, short);
  result &= TEST_CXX_SHMEM_TEST_ALL_VECTOR(int, int);
  result &= TEST_CXX_SHMEM_TEST_ALL_VECTOR(long, long);
  result &= TEST_CXX_SHMEM_TEST_ALL_VECTOR(long long, longlong);
  result &= TEST_CXX_SHMEM_TEST_ALL_VECTOR(unsigned short, ushort);
  result &= TEST_CXX_SHMEM_TEST_ALL_VECTOR(unsigned int, uint);
  result &= TEST_CXX_SHMEM_TEST_ALL_VECTOR(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_TEST_ALL_VECTOR(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_TEST_ALL_VECTOR(int32_t, int32);
  result &= TEST_CXX_SHMEM_TEST_ALL_VECTOR(int64_t, int64);
  result &= TEST_CXX_SHMEM_TEST_ALL_VECTOR(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_TEST_ALL_VECTOR(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_TEST_ALL_VECTOR(size_t, size);
  result &= TEST_CXX_SHMEM_TEST_ALL_VECTOR(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_test_all_vector()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
