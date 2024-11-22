/**
 * @file cxx_shmem_test_some.cpp
 * @brief Unit test shmem_test_some() routine.
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "log.h"
#include "shmemvv.h"

#define TIMEOUT 2
#define TEST_CXX_SHMEM_TEST_SOME(TYPE, TYPENAME)                               \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_test_some()");                            \
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
          shmem_##TYPENAME##_p(&flags[1], 1, pe);                              \
          shmem_##TYPENAME##_p(&flags[3], 1, pe);                              \
        }                                                                      \
        shmem_quiet();                                                         \
        log_info("PE 0: Set flags[1] and flags[3] to 1 on all other PEs");    \
      }                                                                        \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype != 0) {                                                         \
        TYPE cmp_value = 1;                                                    \
        size_t indices[4];                                                     \
        size_t num_indices;                                                    \
        time_t start_time = time(NULL);                                        \
        log_info("PE %d: Starting test_some (flags=%p, n=4, "                  \
                 "condition=SHMEM_CMP_EQ, target=1)",                          \
                 mype, (void *)flags);                                         \
        while ((num_indices = shmem_##TYPENAME##_test_some(                    \
                    flags, 4, indices, NULL, SHMEM_CMP_EQ, cmp_value)) == 0) { \
          if (time(NULL) - start_time > TIMEOUT) {                             \
            log_fail("PE %d: Test timed out after %d seconds", mype, TIMEOUT); \
            break;                                                             \
          }                                                                    \
          usleep(1000);                                                        \
        }                                                                      \
        log_info("PE %d: test_some completed", mype);                          \
        if (flags[1] != 1 || flags[3] != 1) {                                  \
          log_fail("PE %d: Test failed - flags[1]=%d flags[3]=%d, expected 1", \
                   mype, (int)flags[1], (int)flags[3]);                        \
          success = false;                                                     \
        } else {                                                               \
          log_info("PE %d: Successfully received expected flag values",         \
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

  result &= TEST_CXX_SHMEM_TEST_SOME(short, short);
  result &= TEST_CXX_SHMEM_TEST_SOME(int, int);
  result &= TEST_CXX_SHMEM_TEST_SOME(long, long);
  result &= TEST_CXX_SHMEM_TEST_SOME(long long, longlong);
  result &= TEST_CXX_SHMEM_TEST_SOME(unsigned short, ushort);
  result &= TEST_CXX_SHMEM_TEST_SOME(unsigned int, uint);
  result &= TEST_CXX_SHMEM_TEST_SOME(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_TEST_SOME(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_TEST_SOME(int32_t, int32);
  result &= TEST_CXX_SHMEM_TEST_SOME(int64_t, int64);
  result &= TEST_CXX_SHMEM_TEST_SOME(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_TEST_SOME(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_TEST_SOME(size_t, size);
  result &= TEST_CXX_SHMEM_TEST_SOME(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_test_some()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
