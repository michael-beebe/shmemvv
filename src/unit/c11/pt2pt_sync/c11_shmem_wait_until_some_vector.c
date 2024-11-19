/**
 * @file c11_shmem_wait_until_some_vector.c
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

#define TEST_C_SHMEM_WAIT_UNTIL_SOME_VECTOR(TYPE, TYPENAME)                    \
  ({                                                                           \
    log_routine("c11_shmem_wait_until_some_vector(" #TYPE ")");                \
    bool success = true;                                                       \
    TYPE *flags = (TYPE *)shmem_malloc(4 * sizeof(TYPE));                      \
    log_info("shmem_malloc'd 4 flags (%d bytes) at %p", 4 * sizeof(TYPE),      \
             (void *)flags);                                                   \
    if (flags == NULL) {                                                       \
      log_fail("shmem_malloc failed!");                                        \
      success = false;                                                         \
    } else {                                                                   \
      for (int i = 0; i < 4; ++i) {                                            \
        flags[i] = 0;                                                          \
      }                                                                        \
      log_info("set flags to [0; 4]");                                         \
      int mype = shmem_my_pe();                                                \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 0) {                                                         \
        log_info("setting flags[1, 3] ([%p, %p]) on pe 1 to 1",                \
                 (void *)&flags[1], (void *)&flags[3]);                        \
        shmem_##TYPENAME##_p(&flags[1], 1, 1);                                 \
        shmem_##TYPENAME##_p(&flags[3], 1, 1);                                 \
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
        log_info("executing wait_until_some_vector(flags = %p, n = 4, "        \
                 "indices = %p, status = "                                     \
                 "[SHMEM_CMP_EQ; 4], to = 1)",                                 \
                 (void *)flags, (void *)indices);                              \
        size_t count = shmem_##TYPENAME##_wait_until_some_vector(              \
            flags, 4, indices, status, SHMEM_CMP_EQ, cmp_values);              \
        log_info("wait_until_some returned");                                  \
        if (count < 2) {                                                       \
          log_info("wait_until_any didn't find all matches! expected count = " \
                   "2, got %d",                                                \
                   (char)count);                                               \
          success = false;                                                     \
        } else {                                                               \
          for (size_t i = 0; i < count; ++i) {                                 \
            if (flags[indices[i]] != 1) {                                      \
              log_fail(                                                        \
                  "indices says flags[%d] should be 1, but was really %d!",    \
                  indices[i], (char)flags[indices[1]]);                        \
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

  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME_VECTOR(short, short);
  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME_VECTOR(int, int);
  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME_VECTOR(long, long);
  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME_VECTOR(long long, longlong);
  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME_VECTOR(unsigned short, ushort);
  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME_VECTOR(unsigned int, uint);
  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME_VECTOR(unsigned long, ulong);
  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME_VECTOR(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME_VECTOR(int32_t, int32);
  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME_VECTOR(int64_t, int64);
  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME_VECTOR(uint32_t, uint32);
  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME_VECTOR(uint64_t, uint64);
  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME_VECTOR(size_t, size);
  result &= TEST_C_SHMEM_WAIT_UNTIL_SOME_VECTOR(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_wait_until_some_vector()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
