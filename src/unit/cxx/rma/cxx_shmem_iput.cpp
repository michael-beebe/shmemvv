/**
 * @file c_shmem_iput.cpp
 * @brief Unit test for the shmem_iput() routine.
 */

#include <stdio.h>
#include <stdlib.h>
#include <shmem.h>

#include "shmemvv.h"

#define TEST_CXX_SHMEM_IPUT(TYPE, TYPENAME) \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE src[10], dest[10];                                             \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
                                                                               \
    for (int i = 0; i < 10; i++) {                                             \
      src[i] = i + mype;                                                       \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    if (mype == 0) {                                                           \
      shmem_##TYPENAME##_iput(dest, src, 2, 2, 5, 1);                          \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    if (mype == 1) {                                                           \
      for (int i = 0; i < 10; i += 2) {                                        \
        if (dest[i] != i / 2) {                                                \
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

  result &= TEST_CXX_SHMEM_IPUT(long, long);
  result &= TEST_CXX_SHMEM_IPUT(double, double);
  result &= TEST_CXX_SHMEM_IPUT(long double, longdouble);
  result &= TEST_CXX_SHMEM_IPUT(char, char);
  result &= TEST_CXX_SHMEM_IPUT(signed char, schar);
  result &= TEST_CXX_SHMEM_IPUT(short, short);
  result &= TEST_CXX_SHMEM_IPUT(int, int);
  result &= TEST_CXX_SHMEM_IPUT(long, long);
  result &= TEST_CXX_SHMEM_IPUT(long long, longlong);
  result &= TEST_CXX_SHMEM_IPUT(unsigned char, uchar);
  result &= TEST_CXX_SHMEM_IPUT(unsigned short, ushort);
  result &= TEST_CXX_SHMEM_IPUT(unsigned int, uint);
  result &= TEST_CXX_SHMEM_IPUT(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_IPUT(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_IPUT(int8_t, int8);
  result &= TEST_CXX_SHMEM_IPUT(int16_t, int16);
  result &= TEST_CXX_SHMEM_IPUT(int32_t, int32);
  result &= TEST_CXX_SHMEM_IPUT(int64_t, int64);
  result &= TEST_CXX_SHMEM_IPUT(uint8_t, uint8);
  result &= TEST_CXX_SHMEM_IPUT(uint16_t, uint16);
  result &= TEST_CXX_SHMEM_IPUT(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_IPUT(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_IPUT(size_t, size);
  result &= TEST_CXX_SHMEM_IPUT(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_iput()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return rc;
}
