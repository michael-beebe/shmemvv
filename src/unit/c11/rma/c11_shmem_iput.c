/**
 * @file c11_shmem_iput.c
 * @brief Unit test for the shmem_iput() routine.
 */

#include <stdio.h>
#include <stdlib.h>
#include <shmem.h>

#include "shmemvv.h"

#define TEST_C11_SHMEM_IPUT(TYPE) \
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
      shmem_iput(dest, src, 2, 2, 5, 1);                          \
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

  result &= TEST_C11_SHMEM_IPUT(long);
  result &= TEST_C11_SHMEM_IPUT(double);
  result &= TEST_C11_SHMEM_IPUT(long double);
  result &= TEST_C11_SHMEM_IPUT(char);
  result &= TEST_C11_SHMEM_IPUT(signed char);
  result &= TEST_C11_SHMEM_IPUT(short);
  result &= TEST_C11_SHMEM_IPUT(int);
  result &= TEST_C11_SHMEM_IPUT(long);
  result &= TEST_C11_SHMEM_IPUT(long long);
  result &= TEST_C11_SHMEM_IPUT(unsigned char);
  result &= TEST_C11_SHMEM_IPUT(unsigned short);
  result &= TEST_C11_SHMEM_IPUT(unsigned int);
  result &= TEST_C11_SHMEM_IPUT(unsigned long);
  result &= TEST_C11_SHMEM_IPUT(unsigned long long);
  result &= TEST_C11_SHMEM_IPUT(int8_t);
  result &= TEST_C11_SHMEM_IPUT(int16_t);
  result &= TEST_C11_SHMEM_IPUT(int32_t);
  result &= TEST_C11_SHMEM_IPUT(int64_t);
  result &= TEST_C11_SHMEM_IPUT(uint8_t);
  result &= TEST_C11_SHMEM_IPUT(uint16_t);
  result &= TEST_C11_SHMEM_IPUT(uint32_t);
  result &= TEST_C11_SHMEM_IPUT(uint64_t);
  result &= TEST_C11_SHMEM_IPUT(size_t);
  result &= TEST_C11_SHMEM_IPUT(ptrdiff_t);

  shmem_barrier_all();

  if (result) {
    if (shmem_my_pe() == 0) {
      display_test_result("C11 shmem_iput()", result, false);
    }
  } else {
    if (shmem_my_pe() == 0) {
      display_test_result("C11 shmem_iput()", result, false);
      rc = EXIT_FAILURE;
    }
  }

  shmem_finalize();
  return rc;
}
