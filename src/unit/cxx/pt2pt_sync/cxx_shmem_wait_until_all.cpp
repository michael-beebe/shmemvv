/**
 * @file cxx_shmem_wait_until_all.cpp
 * @brief Unit test shmem_wait_until_all() routine.
 */

#include "shmemvv.h"
#include "log.h"
#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define TIMEOUT 2

#define TEST_CXX_SHMEM_WAIT_UNTIL_ALL(TYPE, TYPENAME)                          \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_wait_until_all()");                       \
    bool success = true;                                                       \
    TYPE *flags = (TYPE *)shmem_malloc(2 * sizeof(TYPE));                      \
    log_info("Allocated flags array (%zu bytes) at address %p",                \
             2 * sizeof(TYPE), (void *)flags);                                 \
    if (flags == NULL) {                                                       \
      log_fail("Memory allocation failed - shmem_malloc returned NULL");       \
      success = false;                                                         \
    } else {                                                                   \
      flags[0] = 0;                                                            \
      flags[1] = 0;                                                            \
      int mype = shmem_my_pe();                                                \
      int npes = shmem_n_pes();                                                \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 0) {                                                         \
        for (int pe = 1; pe < npes; ++pe) {                                    \
          shmem_##TYPENAME##_p(&flags[0], 1, pe);                              \
          shmem_##TYPENAME##_p(&flags[1], 1, pe);                              \
        }                                                                      \
        shmem_quiet();                                                         \
      }                                                                        \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype != 0) {                                                         \
        shmem_##TYPENAME##_wait_until_all(flags, 2, NULL, SHMEM_CMP_EQ, 1);    \
        if (flags[0] != 1 || flags[1] != 1) {                                  \
          log_fail("flags[0] = %d, flags[1] = %d, expected both to be 1",      \
                   (int)flags[0], (int)flags[1]);                              \
          success = false;                                                     \
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

  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL(short, short);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL(int, int);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL(long, long);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL(long long, longlong);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL(unsigned short, ushort);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL(unsigned int, uint);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL(int32_t, int32);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL(int64_t, int64);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL(size_t, size);
  result &= TEST_CXX_SHMEM_WAIT_UNTIL_ALL(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_wait_until_all()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
