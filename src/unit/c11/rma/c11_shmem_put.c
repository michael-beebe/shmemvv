/**
 * @file c11_shmem_put.c
 * @brief Unit test for the shmem_put() routine.
 */

#include <stdio.h>
#include <stdlib.h>
#include <shmem.h>

#include "shmemvv.h"

#define TEST_C11_SHMEM_PUT(TYPE) \
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
      shmem_put(dest, src, 10, 1);                                \
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

  result &= TEST_C11_SHMEM_PUT(long);
  result &= TEST_C11_SHMEM_PUT(double);
  result &= TEST_C11_SHMEM_PUT(long double);
  result &= TEST_C11_SHMEM_PUT(char);
  result &= TEST_C11_SHMEM_PUT(signed char);
  result &= TEST_C11_SHMEM_PUT(short);
  result &= TEST_C11_SHMEM_PUT(int);
  result &= TEST_C11_SHMEM_PUT(long);
  result &= TEST_C11_SHMEM_PUT(long long);
  result &= TEST_C11_SHMEM_PUT(unsigned char);
  result &= TEST_C11_SHMEM_PUT(unsigned short);
  result &= TEST_C11_SHMEM_PUT(unsigned int);
  result &= TEST_C11_SHMEM_PUT(unsigned long);
  result &= TEST_C11_SHMEM_PUT(unsigned long long);
  result &= TEST_C11_SHMEM_PUT(int8_t);
  result &= TEST_C11_SHMEM_PUT(int16_t);
  result &= TEST_C11_SHMEM_PUT(int32_t);
  result &= TEST_C11_SHMEM_PUT(int64_t);
  result &= TEST_C11_SHMEM_PUT(uint8_t);
  result &= TEST_C11_SHMEM_PUT(uint16_t);
  result &= TEST_C11_SHMEM_PUT(uint32_t);
  result &= TEST_C11_SHMEM_PUT(uint64_t);
  result &= TEST_C11_SHMEM_PUT(size_t);
  result &= TEST_C11_SHMEM_PUT(ptrdiff_t);

  shmem_barrier_all();

  if (result) {
    if (shmem_my_pe() == 0) {
      display_test_result("C11 shmem_put()", result, false);
    }
  } else {
    if (shmem_my_pe() == 0) {
      display_test_result("C11 shmem_put()", result, false);
      rc = EXIT_FAILURE;
    }
  }

  shmem_finalize();
  return rc;
}
