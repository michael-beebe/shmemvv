/**
 * @file c_shmem_test_all.cpp
 * @brief Tests the shmem_test_all() routine.
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "shmemvv.h"

#define TIMEOUT 2

#define TEST_CXX_SHMEM_TEST_ALL(TYPE, TYPENAME)                                \
  ({                                                                           \
    bool success = true;                                                       \
    TYPE *flags = (TYPE *)shmem_malloc(4 * sizeof(TYPE));                      \
    if (flags == NULL) {                                                       \
      success = false;                                                         \
    } else {                                                                   \
      for (int i = 0; i < 4; ++i) {                                            \
        flags[i] = 0;                                                          \
      }                                                                        \
      int mype = shmem_my_pe();                                                \
      int npes = shmem_n_pes();                                                \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 0) {                                                         \
        for (int pe = 1; pe < npes; ++pe) {                                    \
          for (int i = 0; i < 4; ++i) {                                        \
            shmem_##TYPENAME##_p(&flags[i], 1, pe);                            \
          }                                                                    \
        }                                                                      \
        shmem_quiet();                                                         \
      }                                                                        \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype != 0) {                                                         \
        TYPE cmp_value = 1;                                                    \
        time_t start_time = time(NULL);                                        \
        while (!shmem_##TYPENAME##_test_all(flags, 4, NULL, SHMEM_CMP_EQ,      \
                                            cmp_value)) {                      \
          if (time(NULL) - start_time > TIMEOUT) {                             \
            break;                                                             \
          }                                                                    \
          usleep(500);                                                         \
        }                                                                      \
        for (int i = 0; i < 4; ++i) {                                          \
          if (flags[i] != 1) {                                                 \
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

  int result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_CXX_SHMEM_TEST_ALL(short, short);
  result &= TEST_CXX_SHMEM_TEST_ALL(int, int);
  result &= TEST_CXX_SHMEM_TEST_ALL(long, long);
  result &= TEST_CXX_SHMEM_TEST_ALL(long long, longlong);
  result &= TEST_CXX_SHMEM_TEST_ALL(unsigned short, ushort);
  result &= TEST_CXX_SHMEM_TEST_ALL(unsigned int, uint);
  result &= TEST_CXX_SHMEM_TEST_ALL(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_TEST_ALL(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_TEST_ALL(int32_t, int32);
  result &= TEST_CXX_SHMEM_TEST_ALL(int64_t, int64);
  result &= TEST_CXX_SHMEM_TEST_ALL(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_TEST_ALL(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_TEST_ALL(size_t, size);
  result &= TEST_CXX_SHMEM_TEST_ALL(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_test_all()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return rc;
}
