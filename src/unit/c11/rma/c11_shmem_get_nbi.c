/**
 * @file c11_shmem_get_nbi.c
 * @brief Unit test for the shmem_get_nbi() routine.
 */

#include <stdio.h>
#include <stdlib.h>
#include <shmem.h>

#include "shmemvv.h"

#define TEST_C11_SHMEM_GET_NBI(TYPE) \
  ({                                                                           \
    bool success = true;                                                       \
    static TYPE src[10], dest[10];                                             \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
                                                                               \
    if (mype == 0) {                                                           \
      for (int i = 0; i < 10; i++) {                                           \
        src[i] = i;                                                            \
      }                                                                        \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    if (mype == 1) {                                                           \
      shmem_get_nbi(dest, src, 10, 0);                            \
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

  result &= TEST_C11_SHMEM_GET_NBI(long);
  result &= TEST_C11_SHMEM_GET_NBI(double);
  result &= TEST_C11_SHMEM_GET_NBI(long double);
  result &= TEST_C11_SHMEM_GET_NBI(char);
  result &= TEST_C11_SHMEM_GET_NBI(signed char);
  result &= TEST_C11_SHMEM_GET_NBI(short);
  result &= TEST_C11_SHMEM_GET_NBI(int);
  result &= TEST_C11_SHMEM_GET_NBI(long);
  result &= TEST_C11_SHMEM_GET_NBI(long long);
  result &= TEST_C11_SHMEM_GET_NBI(unsigned char);
  result &= TEST_C11_SHMEM_GET_NBI(unsigned short);
  result &= TEST_C11_SHMEM_GET_NBI(unsigned int);
  result &= TEST_C11_SHMEM_GET_NBI(unsigned long);
  result &= TEST_C11_SHMEM_GET_NBI(unsigned long long);
  result &= TEST_C11_SHMEM_GET_NBI(int8_t);
  result &= TEST_C11_SHMEM_GET_NBI(int16_t);
  result &= TEST_C11_SHMEM_GET_NBI(int32_t);
  result &= TEST_C11_SHMEM_GET_NBI(int64_t);
  result &= TEST_C11_SHMEM_GET_NBI(uint8_t);
  result &= TEST_C11_SHMEM_GET_NBI(uint16_t);
  result &= TEST_C11_SHMEM_GET_NBI(uint32_t);
  result &= TEST_C11_SHMEM_GET_NBI(uint64_t);
  result &= TEST_C11_SHMEM_GET_NBI(size_t);
  result &= TEST_C11_SHMEM_GET_NBI(ptrdiff_t);


  shmem_barrier_all();

  if (result) {
    if (shmem_my_pe() == 0) {
      display_test_result("C11 shmem_get_nbi()", result, false);
    }
  } else {
    if (shmem_my_pe() == 0) {
      display_test_result("C11 shmem_get_nbi()", result, true);
      rc = EXIT_FAILURE;
    }
  }

  shmem_finalize();
  return rc;
}
