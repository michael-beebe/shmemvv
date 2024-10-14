/**
 * @file c_shmem_get.cpp
 * @brief Unit test for the shmem_get() routine.
 */

#include <stdio.h>
#include <stdlib.h>
#include <shmem.h>

#include "shmemvv.h"

#define TEST_CXX_SHMEM_GET(TYPE, TYPENAME) \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE src[10], dest[10];                                             \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
                                                                               \
    if (mype == 0) {                                                           \
      for (int i = 0; i < 10; i++) {                                           \
        src[i] = i;                                                            \
      }                                                                        \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    if (mype == 1) {                                                           \
      shmem_##TYPENAME##_get(dest, src, 10, 0);                                \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    if (mype == 1) {                                                           \
      for (int i = 0; i < 10; i++) {                                           \
        if (dest[i] != i) {                                                    \
          success = false;                                                     \
          break;                                                               \
        }                                                                      \
      }                                                                        \
    }                                                                          \
                                                                               \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();

  if (!(shmem_n_pes() <= 2)) {
    if (shmem_my_pe() == 0) {
      display_not_enough_pes("RMA");
    }
    shmem_finalize();
    return EXIT_SUCCESS;
  }

  int result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_CXX_SHMEM_GET(long, long);
  result &= TEST_CXX_SHMEM_GET(double, double);
  result &= TEST_CXX_SHMEM_GET(long double, longdouble);
  result &= TEST_CXX_SHMEM_GET(char, char);
  result &= TEST_CXX_SHMEM_GET(signed char, schar);
  result &= TEST_CXX_SHMEM_GET(short, short);
  result &= TEST_CXX_SHMEM_GET(int, int);
  result &= TEST_CXX_SHMEM_GET(long, long);
  result &= TEST_CXX_SHMEM_GET(long long, longlong);
  result &= TEST_CXX_SHMEM_GET(unsigned char, uchar);
  result &= TEST_CXX_SHMEM_GET(unsigned short, ushort);
  result &= TEST_CXX_SHMEM_GET(unsigned int, uint);
  result &= TEST_CXX_SHMEM_GET(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_GET(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_GET(int8_t, int8);
  result &= TEST_CXX_SHMEM_GET(int16_t, int16);
  result &= TEST_CXX_SHMEM_GET(int32_t, int32);
  result &= TEST_CXX_SHMEM_GET(int64_t, int64);
  result &= TEST_CXX_SHMEM_GET(uint8_t, uint8);
  result &= TEST_CXX_SHMEM_GET(uint16_t, uint16);
  result &= TEST_CXX_SHMEM_GET(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_GET(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_GET(size_t, size);
  result &= TEST_CXX_SHMEM_GET(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_get()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return rc;
}
