/**
 * @file c11_shmem_test_any.c
 * @brief Tests the shmem_test_any() routine.
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "shmemvv.h"

#define TIMEOUT 2

#define TEST_C_SHMEM_TEST_ANY(TYPE, TYPENAME)                                  \
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
          shmem_##TYPENAME##_p(&flags[2], 1, pe);                              \
        }                                                                      \
        shmem_quiet();                                                         \
      }                                                                        \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype != 0) {                                                         \
        TYPE cmp_value = 1;                                                    \
        time_t start_time = time(NULL);                                        \
        while (!shmem_##TYPENAME##_test_any(flags, 4, NULL, SHMEM_CMP_EQ,      \
                                            cmp_value)) {                      \
          if (time(NULL) - start_time > TIMEOUT) {                             \
            break;                                                             \
          }                                                                    \
          usleep(1000);                                                        \
        }                                                                      \
        if (flags[2] != 1) {                                                   \
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

  result &= TEST_C_SHMEM_TEST_ANY(short, short);
  result &= TEST_C_SHMEM_TEST_ANY(int, int);
  result &= TEST_C_SHMEM_TEST_ANY(long, long);
  result &= TEST_C_SHMEM_TEST_ANY(long long, longlong);
  result &= TEST_C_SHMEM_TEST_ANY(unsigned short, ushort);
  result &= TEST_C_SHMEM_TEST_ANY(unsigned int, uint);
  result &= TEST_C_SHMEM_TEST_ANY(unsigned long, ulong);
  result &= TEST_C_SHMEM_TEST_ANY(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_TEST_ANY(int32_t, int32);
  result &= TEST_C_SHMEM_TEST_ANY(int64_t, int64);
  result &= TEST_C_SHMEM_TEST_ANY(uint32_t, uint32);
  result &= TEST_C_SHMEM_TEST_ANY(uint64_t, uint64);
  result &= TEST_C_SHMEM_TEST_ANY(size_t, size);
  result &= TEST_C_SHMEM_TEST_ANY(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_test_any()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return rc;
}
