/**
 * @file c11_shmem_wait_until_all_vector.c
 * @brief Unit test shmem_wait_until_all_vector() routine.
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "log.h"
#include "shmemvv.h"

#define TIMEOUT 2
#define TEST_C_SHMEM_WAIT_UNTIL_ALL_VECTOR(TYPE, TYPENAME)                     \
  ({                                                                           \
    log_routine("c11_shmem_wait_until_all_vector(" #TYPE ")");                 \
    bool success = true;                                                       \
    TYPE *flags = (TYPE *)shmem_malloc(4 * sizeof(TYPE));                      \
    log_info("shmem_malloc'd flags (%d bytes, 4x " #TYPE ") at %p",            \
             4 * sizeof(TYPE), (void *)flags);                                 \
    if (flags == NULL) {                                                       \
      log_fail("shmem_malloc failed!");                                        \
      success = false;                                                         \
    } else {                                                                   \
      for (int i = 0; i < 4; ++i) {                                            \
        flags[i] = 0;                                                          \
      }                                                                        \
      log_info("set flags to 0");                                              \
      int mype = shmem_my_pe();                                                \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 0) {                                                         \
        for (int i = 0; i < 4; ++i) {                                          \
          shmem_##TYPENAME##_p(&flags[i], 1, 1);                               \
        }                                                                      \
        log_info("set flags on pe 1 to [1; 4]");                               \
        shmem_quiet();                                                         \
      }                                                                        \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype != 0) {                                                         \
        int status[4] = {SHMEM_CMP_EQ, SHMEM_CMP_EQ, SHMEM_CMP_EQ,             \
                         SHMEM_CMP_EQ};                                        \
        TYPE cmp_values[4] = {1, 1, 1, 1};                                     \
        log_info("executing wait_until_all(flags = %p, n = 4, status = "       \
                 "[SHMEM_CMP_EQ; 4], cmp_values = [1; 4])",                    \
                 (void *)flags);                                                \
        shmem_##TYPENAME##_wait_until_all_vector(flags, 4, status,             \
                                                 SHMEM_CMP_EQ, cmp_values);    \
        log_info("wait until returned");                                       \
        for (int i = 0; i < 4; ++i) {                                          \
          if (flags[i] != 1) {                                                 \
            log_fail("wait until returned but flags didn't match test!");      \
            log_fail("expected flags[%d] (%p) to equal 1, got %d", i,          \
                     (void *)&flags[i], flags[i]);                             \
            success = false;                                                   \
            break;                                                             \
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

  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL_VECTOR(short, short);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL_VECTOR(int, int);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL_VECTOR(long, long);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL_VECTOR(long long, longlong);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL_VECTOR(unsigned short, ushort);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL_VECTOR(unsigned int, uint);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL_VECTOR(unsigned long, ulong);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL_VECTOR(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL_VECTOR(int32_t, int32);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL_VECTOR(int64_t, int64);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL_VECTOR(uint32_t, uint32);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL_VECTOR(uint64_t, uint64);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL_VECTOR(size_t, size);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL_VECTOR(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_wait_until_all_vector()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
