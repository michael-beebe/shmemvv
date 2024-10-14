/**
 * @file c_shmem_alltoall.cpp
 *
 * @brief Unit test for shmem_alltoall().
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"

#define TEST_CXX_SHMEM_ALLTOALL(TYPE, TYPENAME)                                \
  ({                                                                           \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(npes * sizeof(TYPE));                     \
    TYPE *dest = (TYPE *)shmem_malloc(npes * sizeof(TYPE));                    \
                                                                               \
    for (int i = 0; i < npes; ++i) {                                           \
      src[i] = mype + i;                                                       \
    }                                                                          \
                                                                               \
    shmem_##TYPENAME##_alltoall(SHMEM_TEAM_WORLD, dest, src, 1);               \
                                                                               \
    bool success = true;                                                       \
    for (int i = 0; i < npes; ++i) {                                           \
      if (dest[i] != mype + i) {                                               \
        success = false;                                                       \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
                                                                               \
    shmem_free(src);                                                           \
    shmem_free(dest);                                                          \
                                                                               \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();

  int result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_CXX_SHMEM_ALLTOALL(float, float);
  result &= TEST_CXX_SHMEM_ALLTOALL(double, double);
  result &= TEST_CXX_SHMEM_ALLTOALL(long double, longdouble);
  result &= TEST_CXX_SHMEM_ALLTOALL(char, char);
  result &= TEST_CXX_SHMEM_ALLTOALL(signed char, schar);
  result &= TEST_CXX_SHMEM_ALLTOALL(short, short);
  result &= TEST_CXX_SHMEM_ALLTOALL(int, int);
  result &= TEST_CXX_SHMEM_ALLTOALL(long, long);
  result &= TEST_CXX_SHMEM_ALLTOALL(long long, longlong);
  result &= TEST_CXX_SHMEM_ALLTOALL(unsigned char, uchar);
  result &= TEST_CXX_SHMEM_ALLTOALL(unsigned short, ushort);
  result &= TEST_CXX_SHMEM_ALLTOALL(unsigned int, uint);
  result &= TEST_CXX_SHMEM_ALLTOALL(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_ALLTOALL(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_ALLTOALL(int8_t, int8);
  result &= TEST_CXX_SHMEM_ALLTOALL(int16_t, int16);
  result &= TEST_CXX_SHMEM_ALLTOALL(int32_t, int32);
  result &= TEST_CXX_SHMEM_ALLTOALL(int64_t, int64);
  result &= TEST_CXX_SHMEM_ALLTOALL(uint8_t, uint8);
  result &= TEST_CXX_SHMEM_ALLTOALL(uint16_t, uint16);
  result &= TEST_CXX_SHMEM_ALLTOALL(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_ALLTOALL(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_ALLTOALL(size_t, size);
  result &= TEST_CXX_SHMEM_ALLTOALL(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_alltoall()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return rc;
}
