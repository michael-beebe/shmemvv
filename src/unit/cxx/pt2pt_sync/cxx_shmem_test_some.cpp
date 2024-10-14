/**
 * @file c_shmem_test_some.cpp
 * @brief Unit test shmem_test_some() routine.
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "shmemvv.h"

#define TIMEOUT 2
#define TEST_CXX_SHMEM_TEST_SOME(TYPE, TYPENAME)                               \
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
          shmem_##TYPENAME##_p(&flags[1], 1, pe);                              \
          shmem_##TYPENAME##_p(&flags[3], 1, pe);                              \
        }                                                                      \
        shmem_quiet();                                                         \
      }                                                                        \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype != 0) {                                                         \
        TYPE cmp_value = 1;                                                    \
        size_t indices[4];                                                     \
        size_t num_indices;                                                    \
        time_t start_time = time(NULL);                                        \
        while ((num_indices = shmem_##TYPENAME##_test_some(                    \
                    flags, 4, indices, NULL, SHMEM_CMP_EQ, cmp_value)) == 0) { \
          if (time(NULL) - start_time > TIMEOUT) {                             \
            break;                                                             \
          }                                                                    \
          usleep(1000);                                                        \
        }                                                                      \
        if (flags[1] != 1 || flags[3] != 1) {                                  \
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

  result &= TEST_CXX_SHMEM_TEST_SOME(short, short);
  result &= TEST_CXX_SHMEM_TEST_SOME(int, int);
  result &= TEST_CXX_SHMEM_TEST_SOME(long, long);
  result &= TEST_CXX_SHMEM_TEST_SOME(long long, longlong);
  result &= TEST_CXX_SHMEM_TEST_SOME(unsigned short, ushort);
  result &= TEST_CXX_SHMEM_TEST_SOME(unsigned int, uint);
  result &= TEST_CXX_SHMEM_TEST_SOME(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_TEST_SOME(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_TEST_SOME(int32_t, int32);
  result &= TEST_CXX_SHMEM_TEST_SOME(int64_t, int64);
  result &= TEST_CXX_SHMEM_TEST_SOME(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_TEST_SOME(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_TEST_SOME(size_t, size);
  result &= TEST_CXX_SHMEM_TEST_SOME(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_test_some()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return rc;
}
