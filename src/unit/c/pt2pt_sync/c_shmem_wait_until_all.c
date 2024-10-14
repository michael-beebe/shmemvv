/**
 * @file c_shmem_wait_until_all.c
 * @brief Unit test shmem_wait_until_all() routine.
 */

#include "shmemvv.h"
#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define TIMEOUT 2

#define TEST_C_SHMEM_WAIT_UNTIL_ALL(TYPE, TYPENAME)                            \
  ({                                                                           \
    bool success = true;                                                       \
    TYPE *flags = (TYPE *)shmem_malloc(2 * sizeof(TYPE));                      \
    if (flags == NULL) {                                                       \
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
          success = false;                                                     \
        }                                                                      \
      }                                                                        \
      shmem_free(flags);                                                       \
    }                                                                          \
    success;                                                                   \
  })

int main(int argc, char **argv) {
  shmem_init();

  int result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL(short, short);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL(int, int);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL(long, long);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL(long long, longlong);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL(unsigned short, ushort);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL(unsigned int, uint);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL(unsigned long, ulong);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL(int32_t, int32);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL(int64_t, int64);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL(uint32_t, uint32);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL(uint64_t, uint64);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL(size_t, size);
  result &= TEST_C_SHMEM_WAIT_UNTIL_ALL(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_wait_until_all()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return rc;
}
