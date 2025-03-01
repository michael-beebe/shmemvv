/**
 * @file c11_shmem_signal_wait_until.c
 * @brief Unit test for shmem_signal_wait_until
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "log.h"
#include "shmemvv.h"

#define TIMEOUT 2
#define TEST_C_SHMEM_WAIT_UNTIL(TYPE, TYPENAME)                                \
  ({                                                                           \
    log_routine("c11_shmem_wait_until(" #TYPE ")");                            \
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
      int npes = shmem_n_pes();                                                \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 0) {                                                         \
        log_info("PE 0: Starting to set flags on other PEs");                  \
        for (int pe = 1; pe < npes; ++pe) {                                    \
          log_info("PE 0: Setting flag to 1 on PE %d (address: %p)", pe,       \
                   (void *)flag);                                              \
          shmem_##TYPENAME##_p(flag, 1, pe);                                   \
        }                                                                      \
        shmem_quiet();                                                         \
        log_info("PE 0: Called shmem_quiet() after setting flags");            \
      }                                                                        \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype != 0) {                                                         \
        log_info("PE %d: Starting wait_until (flag=%p, SHMEM_CMP_EQ, 1)",      \
                 mype, (void *)flag);                                          \
        shmem_##TYPENAME##_wait_until(flag, SHMEM_CMP_EQ, 1);                  \
        log_info("PE %d: wait_until completed with flag value=%d", mype,       \
                 (int)*flag);                                                  \
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

  result &= TEST_C_SHMEM_WAIT_UNTIL(int, int);
  result &= TEST_C_SHMEM_WAIT_UNTIL(long, long);
  result &= TEST_C_SHMEM_WAIT_UNTIL(long long, longlong);
  result &= TEST_C_SHMEM_WAIT_UNTIL(unsigned int, uint);
  result &= TEST_C_SHMEM_WAIT_UNTIL(unsigned long, ulong);
  result &= TEST_C_SHMEM_WAIT_UNTIL(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_WAIT_UNTIL(int32_t, int32);
  result &= TEST_C_SHMEM_WAIT_UNTIL(int64_t, int64);
  result &= TEST_C_SHMEM_WAIT_UNTIL(uint32_t, uint32);
  result &= TEST_C_SHMEM_WAIT_UNTIL(uint64_t, uint64);
  result &= TEST_C_SHMEM_WAIT_UNTIL(size_t, size);
  result &= TEST_C_SHMEM_WAIT_UNTIL(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_wait_until", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
