/**
 * @file cxx_shmem_test_any.cpp
 * @brief Tests the shmem_test_any() routine.
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "log.h"
#include "shmemvv.h"

#define TIMEOUT 2

#define TEST_CXX_SHMEM_TEST_ANY(TYPE, TYPENAME)                                \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_test_any()");                             \
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
      int mype = shmem_my_pe();                                                \
      int npes = shmem_n_pes();                                                \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 0) {                                                         \
        for (int pe = 1; pe < npes; ++pe) {                                    \
          log_info("PE 0: Setting flag[2] to 1 on remote PE %d", pe);          \
          shmem_##TYPENAME##_p(&flags[2], 1, pe);                              \
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
        log_info("PE %d: Starting test_any loop (flags=%p, "                   \
                 "condition=SHMEM_CMP_EQ, target=1)",                          \
                 mype, (void *)flags);                                         \
        while (!shmem_##TYPENAME##_test_any(flags, 4, NULL, SHMEM_CMP_EQ,      \
                                            cmp_value)) {                      \
          if (time(NULL) - start_time > TIMEOUT) {                             \
            break;                                                             \
          }                                                                    \
          usleep(1000);                                                        \
        }                                                                      \
        log_info("PE %d: test_any completed, flag[2] value is %d",             \
                 mype, (int)flags[2]);                                         \
        if (flags[2] != 1) {                                                   \
          log_fail("PE %d: Test failed - flag[2] value mismatch. "             \
                   "Expected 1 but got %d",                                    \
                   mype, (int)flags[2]);                                       \
          success = false;                                                     \
        } else {                                                               \
          log_info("PE %d: Successfully received expected flag value", mype);  \
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

  result &= TEST_CXX_SHMEM_TEST_ANY(short, short);
  result &= TEST_CXX_SHMEM_TEST_ANY(int, int);
  result &= TEST_CXX_SHMEM_TEST_ANY(long, long);
  result &= TEST_CXX_SHMEM_TEST_ANY(long long, longlong);
  result &= TEST_CXX_SHMEM_TEST_ANY(unsigned short, ushort);
  result &= TEST_CXX_SHMEM_TEST_ANY(unsigned int, uint);
  result &= TEST_CXX_SHMEM_TEST_ANY(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_TEST_ANY(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_TEST_ANY(int32_t, int32);
  result &= TEST_CXX_SHMEM_TEST_ANY(int64_t, int64);
  result &= TEST_CXX_SHMEM_TEST_ANY(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_TEST_ANY(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_TEST_ANY(size_t, size);
  result &= TEST_CXX_SHMEM_TEST_ANY(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_test_any()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
