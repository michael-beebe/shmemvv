/**
 * @file c11_shmem_wait_until_some.c
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

#define TEST_C_SHMEM_WAIT_UNTIL_SOME(TYPE, TYPENAME)                           \
  ({                                                                           \
    log_routine("c11_shmem_wait_until_some(" #TYPE ")");                       \
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
        log_info("PE 0: Setting flags[1] and flags[3] to 1 on PE 1 "           \
                 "(addresses: %p, %p)", (void *)&flags[1], (void *)&flags[3]); \
        shmem_##TYPENAME##_p(&flags[1], 1, 1);                                 \
        shmem_##TYPENAME##_p(&flags[3], 1, 1);                                 \
        shmem_quiet();                                                         \
        log_info("PE 0: Called shmem_quiet() after setting flags");            \
      }                                                                        \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype != 0) {                                                         \
        size_t indices[4];                                                     \
        int status[4] = {SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ,             \
                         SHMEM_CMP_EQ};                                        \
        log_info("PE %d: Starting wait_until_some (flags=%p, n=4, "            \
                 "indices=%p, status=[SHMEM_CMP_EQ x4], target=1)",            \
                 mype, (void *)flags, (void *)indices);                        \
        size_t count = shmem_##TYPENAME##_wait_until_some(                     \
            flags, 4, indices, status, SHMEM_CMP_EQ, 1);                       \
        log_info("PE %d: wait_until_some completed with count=%zu",            \
                 mype, count);                                                 \
        if (count < 2) {                                                       \
          log_fail("PE %d: wait_until_some failed - expected count=2, "        \
                   "got count=%zu", mype, count);                              \
          success = false;                                                     \
        } else {                                                               \
          for (size_t i = 0; i < count; ++i) {                                 \
            if (flags[indices[i]] != 1) {                                      \
              log_fail("PE %d: Validation failed - flags[%zu]=%d, expected 1", \
                       mype, indices[i], (int)flags[indices[i]]);              \
              success = false;                                                 \
              break;                                                           \
            } else {                                                           \
              log_info("PE %d: Successfully validated flags[%zu]=1",           \
                       mype, indices[i]);                                      \
            }                                                                  \
          }                                                                    \
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

  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME(int, int);
  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME(long, long);
  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME(long long, longlong);
  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME(unsigned int, uint);
  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME(unsigned long, ulong);
  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME(int32_t, int32);
  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME(int64_t, int64);
  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME(uint32_t, uint32);
  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME(uint64_t, uint64);
  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME(size_t, size);
  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_wait_until_some()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
