/**
 * @file c11_shmem_p.c
 * @brief Unit test for the shmem_p() routine.
 */

#include <stdio.h>
#include <stdlib.h>
#include <shmem.h>

#include "shmemvv.h"

#define TEST_C11_SHMEM_P(TYPE) \
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
      shmem_p(&dest, src, 1);                                     \
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

  result &= TEST_C11_SHMEM_P(long);
  result &= TEST_C11_SHMEM_P(double);
  result &= TEST_C11_SHMEM_P(long double);
  result &= TEST_C11_SHMEM_P(char);
  result &= TEST_C11_SHMEM_P(signed char);
  result &= TEST_C11_SHMEM_P(short);
  result &= TEST_C11_SHMEM_P(int);
  result &= TEST_C11_SHMEM_P(long);
  result &= TEST_C11_SHMEM_P(long long);
  result &= TEST_C11_SHMEM_P(unsigned char);
  result &= TEST_C11_SHMEM_P(unsigned short);
  result &= TEST_C11_SHMEM_P(unsigned int);
  result &= TEST_C11_SHMEM_P(unsigned long);
  result &= TEST_C11_SHMEM_P(unsigned long long);
  result &= TEST_C11_SHMEM_P(int8_t);
  result &= TEST_C11_SHMEM_P(int16_t);
  result &= TEST_C11_SHMEM_P(int32_t);
  result &= TEST_C11_SHMEM_P(int64_t);
  result &= TEST_C11_SHMEM_P(uint8_t);
  result &= TEST_C11_SHMEM_P(uint16_t);
  result &= TEST_C11_SHMEM_P(uint32_t);
  result &= TEST_C11_SHMEM_P(uint64_t);
  result &= TEST_C11_SHMEM_P(size_t);
  result &= TEST_C11_SHMEM_P(ptrdiff_t);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_p()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return rc;
}
