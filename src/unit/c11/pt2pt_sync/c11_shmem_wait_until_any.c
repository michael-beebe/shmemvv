/**
 * @file c11_shmem_wait_until_any.c
 * @brief Unit test shmem_wait_until_any() routine.
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "log.h"
#include "shmemvv.h"

#define TIMEOUT 2
#define TEST_C11_SHMEM_WAIT_UNTIL_ANY(TYPE)                                    \
  ({                                                                           \
    log_routine("c11_shmem_wait_until_any(" #TYPE ")");                        \
    bool success = true;                                                       \
    TYPE *flags = (TYPE *)shmem_malloc(3 * sizeof(TYPE));                      \
    log_info("Allocated flags array (%zu bytes) at address %p",                \
             3 * sizeof(TYPE), (void *)flags);                                 \
    if (flags == NULL) {                                                       \
      log_fail("Memory allocation failed - shmem_malloc returned NULL");       \
      success = false;                                                         \
    } else {                                                                   \
      for (int i = 0; i < 3; i++) {                                            \
        flags[i] = 0;                                                          \
      }                                                                        \
      log_info("Initialized all flags to 0");                                  \
      int mype = shmem_my_pe();                                                \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 0) {                                                         \
        log_info("PE 0: Setting flags[2] to 1 on PE 1 (address: %p)",          \
                 (void *)&flags[2]);                                           \
        shmem_p(&flags[2], 1, 1);                                              \
        shmem_quiet();                                                         \
        log_info("PE 0: Called shmem_quiet() after setting flag");             \
      }                                                                        \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype != 0) {                                                         \
        int status[3] = {SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ};            \
        log_info("PE %d: Starting wait_until_any (flags=%p, n=3, "             \
                 "status=[SHMEM_CMP_EQ x3], target=1)",                        \
                 mype, (void *)flags);                                         \
        size_t index =                                                         \
            shmem_wait_until_any(flags, 3, status, SHMEM_CMP_EQ, 1);           \
        log_info("PE %d: wait_until_any completed with index=%zu", mype,       \
                 index);                                                       \
        if (index == SIZE_MAX) {                                               \
          log_fail("PE %d: wait_until_any failed - returned SIZE_MAX", mype);  \
          success = false;                                                     \
        } else if (flags[index] != 1) {                                        \
          log_fail("PE %d: Validation failed - flags[%zu]=%d, expected 1",     \
                   mype, index, (int)flags[index]);                            \
          success = false;                                                     \
        } else {                                                               \
          log_info("PE %d: Successfully validated flags[%zu]=1", mype, index); \
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

  // result &= TEST_C11_SHMEM_WAIT_UNTIL_ANY(short);
  result &= TEST_C11_SHMEM_WAIT_UNTIL_ANY(int);
  result &= TEST_C11_SHMEM_WAIT_UNTIL_ANY(long);
  result &= TEST_C11_SHMEM_WAIT_UNTIL_ANY(long long);
  // result &= TEST_C11_SHMEM_WAIT_UNTIL_ANY(unsigned short);
  result &= TEST_C11_SHMEM_WAIT_UNTIL_ANY(unsigned int);
  result &= TEST_C11_SHMEM_WAIT_UNTIL_ANY(unsigned long);
  result &= TEST_C11_SHMEM_WAIT_UNTIL_ANY(unsigned long long);
  result &= TEST_C11_SHMEM_WAIT_UNTIL_ANY(int32_t);
  result &= TEST_C11_SHMEM_WAIT_UNTIL_ANY(int64_t);
  result &= TEST_C11_SHMEM_WAIT_UNTIL_ANY(uint32_t);
  result &= TEST_C11_SHMEM_WAIT_UNTIL_ANY(uint64_t);
  result &= TEST_C11_SHMEM_WAIT_UNTIL_ANY(size_t);
  result &= TEST_C11_SHMEM_WAIT_UNTIL_ANY(ptrdiff_t);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_wait_until_any", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
