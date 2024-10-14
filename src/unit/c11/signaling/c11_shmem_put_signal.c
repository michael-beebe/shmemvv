/**
 * @file c_shmem_put_signal.c
 * @brief Unit test for the shmem_put_signal() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"

#define TEST_C11_SHMEM_PUT_SIGNAL(TYPE)                                        \
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
        shmem_put_signal(&dest, &value, 1, &signal, 1, target_pe,              \
                         SHMEM_SIGNAL_SET);                                    \
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

  result &= TEST_C11_SHMEM_PUT_SIGNAL(float);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(double);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(long double);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(char);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(signed char);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(short);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(int);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(long);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(long long);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(unsigned char);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(unsigned short);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(unsigned int);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(unsigned long);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(unsigned long long);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(int8_t);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(int16_t);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(int32_t);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(int64_t);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(uint8_t);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(uint16_t);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(uint32_t);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(uint64_t);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(size_t);
  result &= TEST_C11_SHMEM_PUT_SIGNAL(ptrdiff_t);

  if (!result) {
    rc = EXIT_FAILURE;
  }

  if (mype == 0) {
    display_test_result("C11 shmem_put_signal()", result, false);
  }

  shmem_finalize();
  return rc;
}