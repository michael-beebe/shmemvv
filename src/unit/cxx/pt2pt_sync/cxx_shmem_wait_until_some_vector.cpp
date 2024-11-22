/**
 * @file cxx_shmem_wait_until_some_vector.cpp
 * @brief Unit test shmem_wait_until_some_vector() routine.
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "log.h"
#include "shmemvv.h"

#define TIMEOUT 2

#define TEST_CXX_SHMEM_WAIT_UNTIL_SOME_VECTOR(TYPE, TYPENAME)                  \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_wait_until_some_vector()");                \
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
        log_info("PE 0: Setting flags[1] and flags[3] to 1 on PE 1");         \
        shmem_##TYPENAME##_p(&flags[1], 1, 1);                                 \
        shmem_##TYPENAME##_p(&flags[3], 1, 1);                                 \
        log_info("PE 0: Called shmem_quiet() after setting flags");            \
        shmem_quiet();                                                         \
      }                                                                        \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype != 0) {                                                         \
        int status[4] = {SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ,             \
                         SHMEM_CMP_EQ};                                        \
        TYPE cmp_values[4] = {1, 1, 1, 1};                                     \
        size_t indices[4];                                                     \
        log_info("PE %d: Calling wait_until_some_vector", mype);              \
        size_t count = shmem_##TYPENAME##_wait_until_some_vector(              \
            flags, 4, indices, status, SHMEM_CMP_EQ, cmp_values);              \
        log_info("PE %d: wait_until_some_vector returned count=%zu", mype,     \
                count);                                                        \
        if (count < 2) {                                                       \
          log_fail("Expected count >= 2 but got %zu", count);                 \
          success = false;                                                     \
        } else {                                                               \
          for (size_t i = 0; i < count; ++i) {                                 \
            if (flags[indices[i]] != 1) {                                      \
              log_fail("flags[%zu] = %d, expected 1", indices[i],             \
                      (int)flags[indices[i]]);                                 \
              success = false;                                                 \
              break;                                                           \
            }                                                                  \
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

  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME_VECTOR(short, short);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME_VECTOR(int, int);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME_VECTOR(long, long);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME_VECTOR(long long, longlong);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME_VECTOR(unsigned short, ushort);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME_VECTOR(unsigned int, uint);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME_VECTOR(unsigned long, ulong);
  result &=
      TEST_CXX_SHMEM_WAIT_UNTIL_SOME_VECTOR(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME_VECTOR(int32_t, int32);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME_VECTOR(int64_t, int64);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME_VECTOR(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME_VECTOR(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME_VECTOR(size_t, size);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_SOME_VECTOR(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_wait_until_some_vector()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
