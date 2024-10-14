/**
 * @file c_shmem_put_signal.cpp
 * @brief Unit test for the shmem_put_signal() routine.
 */

#include <stdio.h>
#include <stdlib.h>
#include <shmem.h>

#include "shmemvv.h"

#define TEST_CXX_SHMEM_PUT_SIGNAL(TYPE, TYPENAME) \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE dest = 0;                                                      \
    static TYPE value = (TYPE)123;                                             \
    static uint64_t signal = 0;                                                \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
                                                                               \
    if (npes < 2) {                                                            \
      success = false;                                                         \
    } else {                                                                   \
      int target_pe = (mype + 1) % npes;                                       \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 0) {                                                         \
        shmem_##TYPENAME##_put_signal(&dest, &value, 1, &signal, 1, target_pe, \
                                      SHMEM_SIGNAL_SET);                       \
      }                                                                        \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 1) {                                                         \
        if (dest != 123 || signal != 1) {                                      \
          success = false;                                                     \
        }                                                                      \
      }                                                                        \
    }                                                                          \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();

  int npes = shmem_n_pes();
  int mype = shmem_my_pe();

  if (!(npes <= 2)) {
    if (mype == 0) {
      display_not_enough_pes("SIGNALING");
    }
    shmem_finalize();
    return EXIT_SUCCESS;
  }

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_CXX_SHMEM_PUT_SIGNAL(float, float);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(double, double);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(long double, longdouble);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(char, char);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(signed char, schar);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(short, short);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(int, int);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(long, long);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(long long, longlong);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(unsigned char, uchar);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(unsigned short, ushort);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(unsigned int, uint);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(int8_t, int8);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(int16_t, int16);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(int32_t, int32);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(int64_t, int64);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(uint8_t, uint8);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(uint16_t, uint16);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(size_t, size);
  result &= TEST_CXX_SHMEM_PUT_SIGNAL(ptrdiff_t, ptrdiff);

  if (!result) {
    rc = EXIT_FAILURE;
  }

  if (mype == 0) {
    display_test_result("CXX shmem_put_signal()", result, false);
  }

  shmem_finalize();
  return rc;
}