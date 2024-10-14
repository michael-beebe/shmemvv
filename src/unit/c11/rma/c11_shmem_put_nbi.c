/**
 * @file c11_shmem_put_nbi.c
 * @brief Unit test for the shmem_put_nbi() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"

#define TEST_C11_SHMEM_PUT_NBI(TYPE)                                           \
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
      shmem_put_nbi(dest, src, 10, 1);                                         \
      shmem_quiet();                                                           \
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

  result &= TEST_C11_SHMEM_PUT_NBI(long);
  result &= TEST_C11_SHMEM_PUT_NBI(double);
  result &= TEST_C11_SHMEM_PUT_NBI(long double);
  result &= TEST_C11_SHMEM_PUT_NBI(char);
  result &= TEST_C11_SHMEM_PUT_NBI(signed char);
  result &= TEST_C11_SHMEM_PUT_NBI(short);
  result &= TEST_C11_SHMEM_PUT_NBI(int);
  result &= TEST_C11_SHMEM_PUT_NBI(long);
  result &= TEST_C11_SHMEM_PUT_NBI(long long);
  result &= TEST_C11_SHMEM_PUT_NBI(unsigned char);
  result &= TEST_C11_SHMEM_PUT_NBI(unsigned short);
  result &= TEST_C11_SHMEM_PUT_NBI(unsigned int);
  result &= TEST_C11_SHMEM_PUT_NBI(unsigned long);
  result &= TEST_C11_SHMEM_PUT_NBI(unsigned long long);
  result &= TEST_C11_SHMEM_PUT_NBI(int8_t);
  result &= TEST_C11_SHMEM_PUT_NBI(int16_t);
  result &= TEST_C11_SHMEM_PUT_NBI(int32_t);
  result &= TEST_C11_SHMEM_PUT_NBI(int64_t);
  result &= TEST_C11_SHMEM_PUT_NBI(uint8_t);
  result &= TEST_C11_SHMEM_PUT_NBI(uint16_t);
  result &= TEST_C11_SHMEM_PUT_NBI(uint32_t);
  result &= TEST_C11_SHMEM_PUT_NBI(uint64_t);
  result &= TEST_C11_SHMEM_PUT_NBI(size_t);
  result &= TEST_C11_SHMEM_PUT_NBI(ptrdiff_t);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_put_nbi()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return rc;
}
