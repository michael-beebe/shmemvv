/**
 * @file cxx_shmem_test_all.cpp
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

#define TEST_CXX_SHMEM_TEST_ALL(TYPE, TYPENAME)                                \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_test_all()");                             \
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
          log_info("PE 0: Setting all flags to 1 on PE %d", pe);               \
          for (int i = 0; i < 4; ++i) {                                        \
            shmem_##TYPENAME##_p(&flags[i], 1, pe);                            \
          }                                                                    \
        }                                                                      \
        log_info("PE 0: Completed setting flags, calling shmem_quiet()");      \
        shmem_quiet();                                                         \
      } else {                                                                 \
        log_info("PE %d: Waiting for PE 0 to set local flags", mype);          \
      }                                                                        \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype != 0) {                                                         \
        TYPE cmp_value = 1;                                                    \
        time_t start_time = time(NULL);                                        \
        log_info("PE %d: Starting test_all loop (flags=%p, n=4, "              \
                 "condition=SHMEM_CMP_EQ, target=1)",                          \
                 mype, (void *)flags);                                         \
        while (!shmem_##TYPENAME##_test_all(flags, 4, NULL, SHMEM_CMP_EQ,      \
                                            cmp_value)) {                      \
          if (time(NULL) - start_time > TIMEOUT) {                             \
            log_fail("PE %d: Test timed out after %d seconds", mype, TIMEOUT); \
            break;                                                             \
          }                                                                    \
          usleep(500);                                                         \
        }                                                                      \
        bool all_set = true;                                                   \
        for (int i = 0; i < 4; ++i) {                                          \
          if (flags[i] != 1) {                                                 \
            log_fail("PE %d: Test failed - flag[%d] value mismatch. "          \
                     "Expected 1 but got %d",                                  \
                     mype, i, flags[i]);                                       \
            success = false;                                                   \
            all_set = false;                                                   \
            break;                                                             \
          }                                                                    \
        }                                                                      \
        if (all_set) {                                                         \
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

  result &= TEST_CXX_SHMEM_TEST_ALL(short, short);
  result &= TEST_CXX_SHMEM_TEST_ALL(int, int);
  result &= TEST_CXX_SHMEM_TEST_ALL(long, long);
  result &= TEST_CXX_SHMEM_TEST_ALL(long long, longlong);
  result &= TEST_CXX_SHMEM_TEST_ALL(unsigned short, ushort);
  result &= TEST_CXX_SHMEM_TEST_ALL(unsigned int, uint);
  result &= TEST_CXX_SHMEM_TEST_ALL(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_TEST_ALL(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_TEST_ALL(int32_t, int32);
  result &= TEST_CXX_SHMEM_TEST_ALL(int64_t, int64);
  result &= TEST_CXX_SHMEM_TEST_ALL(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_TEST_ALL(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_TEST_ALL(size_t, size);
  result &= TEST_CXX_SHMEM_TEST_ALL(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_test_all()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
