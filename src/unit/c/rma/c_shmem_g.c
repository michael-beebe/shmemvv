/**
 * @file c_shmem_g.c
 * @brief Unit test for the shmem_g() routine.
 */

#include <stdio.h>
#include <stdlib.h>
#include <shmem.h>

#include "shmemvv.h"

#define TEST_C_SHMEM_G(TYPE, TYPENAME) \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE src, dest;                                                     \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
                                                                               \
    if (mype == 0) {                                                           \
      src = 10;                                                                \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    if (mype == 1) {                                                           \
      dest = shmem_##TYPENAME##_g(&src, 0);                                    \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    if (mype == 1) {                                                           \
      if (dest != 10) {                                                        \
        success = false;                                                       \
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

  result &= TEST_C_SHMEM_G(long, long);
  result &= TEST_C_SHMEM_G(double, double);
  result &= TEST_C_SHMEM_G(long double, longdouble);
  result &= TEST_C_SHMEM_G(char, char);
  result &= TEST_C_SHMEM_G(signed char, schar);
  result &= TEST_C_SHMEM_G(short, short);
  result &= TEST_C_SHMEM_G(int, int);
  result &= TEST_C_SHMEM_G(long, long);
  result &= TEST_C_SHMEM_G(long long, longlong);
  result &= TEST_C_SHMEM_G(unsigned char, uchar);
  result &= TEST_C_SHMEM_G(unsigned short, ushort);
  result &= TEST_C_SHMEM_G(unsigned int, uint);
  result &= TEST_C_SHMEM_G(unsigned long, ulong);
  result &= TEST_C_SHMEM_G(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_G(int8_t, int8);
  result &= TEST_C_SHMEM_G(int16_t, int16);
  result &= TEST_C_SHMEM_G(int32_t, int32);
  result &= TEST_C_SHMEM_G(int64_t, int64);
  result &= TEST_C_SHMEM_G(uint8_t, uint8);
  result &= TEST_C_SHMEM_G(uint16_t, uint16);
  result &= TEST_C_SHMEM_G(uint32_t, uint32);
  result &= TEST_C_SHMEM_G(uint64_t, uint64);
  result &= TEST_C_SHMEM_G(size_t, size);
  result &= TEST_C_SHMEM_G(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_g()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return rc;
}

