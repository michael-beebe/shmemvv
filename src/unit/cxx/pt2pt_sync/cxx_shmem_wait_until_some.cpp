/**
 * @file cxx_shmem_wait_until_some.cpp
 * @brief Unit test shmem_wait_until_some() routine.
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "log.h"
#include "shmemvv.h"

#define TIMEOUT 2

#define TEST_CXX_SHMEM_WAIT_UNTIL_SOME(TYPE, TYPENAME)                         \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_wait_until_some()");                      \
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
        log_info("PE 0: Setting flags[1] and flags[3] to 1 on PE 1");          \
        shmem_##TYPENAME##_p(&flags[1], 1, 1);                                 \
        shmem_##TYPENAME##_p(&flags[3], 1, 1);                                 \
        log_info("PE 0: Called shmem_quiet() after setting flags");            \
        shmem_quiet();                                                         \
      }                                                                        \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype != 0) {                                                         \
        size_t indices[4];                                                     \
        int status[4] = {SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ,             \
                         SHMEM_CMP_EQ};                                        \
        log_info("PE %d: Waiting for some flags to become 1", mype);           \
        size_t count = shmem_##TYPENAME##_wait_until_some(                     \
            flags, 4, indices, status, SHMEM_CMP_EQ, 1);                       \
        log_info("PE %d: Wait completed, got count=%zu", mype, count);         \
        if (count < 2) {                                                       \
          log_fail("PE %d: Expected count >= 2 but got %zu", mype, count);     \
          success = false;                                                     \
        } else {                                                               \
          log_info("PE %d: Verifying flags at returned indices", mype);        \
          for (size_t i = 0; i < count; ++i) {                                 \
            if (flags[indices[i]] != 1) {                                      \
              log_fail("PE %d: flags[%zu] = %d, expected 1", mype, indices[i], \
                       flags[indices[i]]);                                     \
              success = false;                                                 \
              break;                                                           \
            }                                                                  \
          }                                                                    \
          if (success) {                                                       \
            log_info("PE %d: All flags verified successfully", mype);          \
          }                                                                    \
        }                                                                      \
      }                                                                        \
      shmem_free(flags);                                                       \
    }                                                                          \
    success;                                                                   \
  })

int main(int argc, char **argv) {
  shmem_init();
  log_init(__FILE__);

  int result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME(int, int);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME(long, long);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME(long long, longlong);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME(unsigned int, uint);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME(int32_t, int32);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME(int64_t, int64);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME(size_t, size);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_wait_until_some()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
