/**
 * @file c11_shmem_g.c
 * @brief Unit test for the shmem_g() routine.
 */

#include <stdio.h>
#include <stdlib.h>
#include <shmem.h>

#include "shmemvv.h"

#define TEST_C11_SHMEM_G(TYPE) \
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
      dest = shmem_g(&src, 0);                                    \
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

  result &= TEST_C11_SHMEM_G(long);
  result &= TEST_C11_SHMEM_G(double);
  result &= TEST_C11_SHMEM_G(long double);
  result &= TEST_C11_SHMEM_G(char);
  result &= TEST_C11_SHMEM_G(signed char);
  result &= TEST_C11_SHMEM_G(short);
  result &= TEST_C11_SHMEM_G(int);
  result &= TEST_C11_SHMEM_G(long);
  result &= TEST_C11_SHMEM_G(long long);
  result &= TEST_C11_SHMEM_G(unsigned char);
  result &= TEST_C11_SHMEM_G(unsigned short);
  result &= TEST_C11_SHMEM_G(unsigned int);
  result &= TEST_C11_SHMEM_G(unsigned long);
  result &= TEST_C11_SHMEM_G(unsigned long long);
  result &= TEST_C11_SHMEM_G(int8_t);
  result &= TEST_C11_SHMEM_G(int16_t);
  result &= TEST_C11_SHMEM_G(int32_t);
  result &= TEST_C11_SHMEM_G(int64_t);
  result &= TEST_C11_SHMEM_G(uint8_t);
  result &= TEST_C11_SHMEM_G(uint16_t);
  result &= TEST_C11_SHMEM_G(uint32_t);
  result &= TEST_C11_SHMEM_G(uint64_t);
  result &= TEST_C11_SHMEM_G(size_t);
  result &= TEST_C11_SHMEM_G(ptrdiff_t);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_g()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return rc;
}

