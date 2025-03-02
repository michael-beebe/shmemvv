/**
 * @file cxx_shmem_wait_until_any.cpp
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
#define TEST_CXX_SHMEM_WAIT_UNTIL_ANY(TYPE, TYPENAME)                          \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_wait_until_any()");                       \
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
        shmem_##TYPENAME##_p(&flags[2], 1, 1);                                 \
        shmem_quiet();                                                         \
        log_info("PE 0: Set flags[2] to 1 on PE 1");                           \
      }                                                                        \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype != 0) {                                                         \
        int status[3] = {SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ};            \
        size_t index = shmem_##TYPENAME##_wait_until_any(flags, 3, status,     \
                                                         SHMEM_CMP_EQ, 1);     \
        if (index == SIZE_MAX) {                                               \
          log_fail("wait_until_any returned SIZE_MAX");                        \
          success = false;                                                     \
        } else if (flags[index] != 1) {                                        \
          log_fail("flags[%zu] = %d, expected 1", index, flags[index]);        \
          success = false;                                                     \
        } else {                                                               \
          log_info("wait_until_any returned index %zu with value %d", index,   \
                   flags[index]);                                              \
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

  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY(short, short);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY(int, int);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY(long, long);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY(long long, longlong);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY(unsigned short, ushort);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY(unsigned int, uint);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY(int32_t, int32);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY(int64_t, int64);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY(size_t, size);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ANY(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_wait_until_any()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
