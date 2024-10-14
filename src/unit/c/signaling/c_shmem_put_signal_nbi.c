/**
 * @file c_shmem_put_signal_nbi.c
 * @brief Unit test for the shmem_put_signal_nbi() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"

#define TEST_C_SHMEM_PUT_SIGNAL_NBI(TYPE, TYPENAME)                            \
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
        shmem_##TYPENAME##_put_signal_nbi(&dest, &value, 1, &signal, 1,        \
                                          target_pe, SHMEM_SIGNAL_SET);        \
        shmem_quiet();                                                         \
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

  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(float, float);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(double, double);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(long double, longdouble);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(char, char);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(signed char, schar);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(short, short);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(int, int);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(long, long);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(long long, longlong);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(unsigned char, uchar);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(unsigned short, ushort);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(unsigned int, uint);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(unsigned long, ulong);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(int8_t, int8);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(int16_t, int16);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(int32_t, int32);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(int64_t, int64);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(uint8_t, uint8);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(uint16_t, uint16);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(uint32_t, uint32);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(uint64_t, uint64);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(size_t, size);
  result &= TEST_C_SHMEM_PUT_SIGNAL_NBI(ptrdiff_t, ptrdiff);

  if (!result) {
    rc = EXIT_FAILURE;
  }

  if (mype == 0) {
    display_test_result("C shmem_put_signal_nbi()", result, false);
  }

  shmem_finalize();
  return rc;
}
