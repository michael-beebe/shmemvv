/**
 * @file c_shmem_p.c
 * @brief Unit test for the shmem_p() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"

#define TEST_C_SHMEM_P(TYPE, TYPENAME)                                         \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE src, dest;                                                     \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
                                                                               \
    src = mype;                                                                \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    if (mype == 0) {                                                           \
      shmem_##TYPENAME##_p(&dest, src, 1);                                     \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    if (mype == 1) {                                                           \
      if (dest != 0) {                                                         \
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

  result &= TEST_C_SHMEM_P(long, long);
  result &= TEST_C_SHMEM_P(double, double);
  result &= TEST_C_SHMEM_P(long double, longdouble);
  result &= TEST_C_SHMEM_P(char, char);
  result &= TEST_C_SHMEM_P(signed char, schar);
  result &= TEST_C_SHMEM_P(short, short);
  result &= TEST_C_SHMEM_P(int, int);
  result &= TEST_C_SHMEM_P(long, long);
  result &= TEST_C_SHMEM_P(long long, longlong);
  result &= TEST_C_SHMEM_P(unsigned char, uchar);
  result &= TEST_C_SHMEM_P(unsigned short, ushort);
  result &= TEST_C_SHMEM_P(unsigned int, uint);
  result &= TEST_C_SHMEM_P(unsigned long, ulong);
  result &= TEST_C_SHMEM_P(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_P(int8_t, int8);
  result &= TEST_C_SHMEM_P(int16_t, int16);
  result &= TEST_C_SHMEM_P(int32_t, int32);
  result &= TEST_C_SHMEM_P(int64_t, int64);
  result &= TEST_C_SHMEM_P(uint8_t, uint8);
  result &= TEST_C_SHMEM_P(uint16_t, uint16);
  result &= TEST_C_SHMEM_P(uint32_t, uint32);
  result &= TEST_C_SHMEM_P(uint64_t, uint64);
  result &= TEST_C_SHMEM_P(size_t, size);
  result &= TEST_C_SHMEM_P(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_p()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return rc;
}
