/**
 * @file cxx_shmem_wait_until.cpp
 * @brief Unit test for shmem_wait_until
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "log.h"
#include "shmemvv.h"

#define TIMEOUT 2
#define TEST_CXX_SHMEM_WAIT_UNTIL(TYPE, TYPENAME)                              \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_wait_until()");                           \
    bool success = true;                                                       \
    TYPE *flag = (TYPE *)shmem_malloc(sizeof(TYPE));                           \
    log_info("Allocated flag (%zu bytes) at address %p", sizeof(TYPE),         \
             (void *)flag);                                                    \
    if (flag == NULL) {                                                        \
      log_fail("Memory allocation failed - shmem_malloc returned NULL");       \
      success = false;                                                         \
    } else {                                                                   \
      *flag = 0;                                                               \
      int mype = shmem_my_pe();                                                \
      int npes = shmem_n_pes();                                                \
      log_info("PE %d: Initialized flag to 0", mype);                          \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 0) {                                                         \
        for (int pe = 1; pe < npes; ++pe) {                                    \
          log_info("PE 0: Setting flag to 1 on PE %d", pe);                    \
          shmem_##TYPENAME##_p(flag, 1, pe);                                   \
        }                                                                      \
        shmem_quiet();                                                         \
        log_info("PE 0: Called shmem_quiet() after setting flags");            \
      }                                                                        \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype != 0) {                                                         \
        log_info("PE %d: Waiting for flag to become 1", mype);                 \
        shmem_##TYPENAME##_wait_until(flag, SHMEM_CMP_EQ, 1);                  \
        log_info("PE %d: wait_until completed, flag value is %d", mype,        \
                 (int)*flag);                                                  \
        if (*flag != 1) {                                                      \
          log_fail("PE %d: Test failed - flag value mismatch after wait. "     \
                   "Expected 1 but got %d",                                    \
                   mype, (int)*flag);                                          \
          success = false;                                                     \
        } else {                                                               \
          log_info("PE %d: Successfully received expected flag value", mype);  \
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

  result &= TEST_CXX_SHMEM_WAIT_UNTIL(int, int);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL(long, long);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL(long long, longlong);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL(unsigned int, uint);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL(int32_t, int32);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL(int64_t, int64);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL(size_t, size);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_wait_until()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
