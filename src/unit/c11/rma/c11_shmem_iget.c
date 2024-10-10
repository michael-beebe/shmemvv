/**
 * @file c11_shmem_iget.c
 * @brief Unit test for the shmem_iget() routine.
 */

#include <stdio.h>
#include <stdlib.h>
#include <shmem.h>

#include "shmemvv.h"

#define TEST_C11_SHMEM_IGET(TYPE) \
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
      shmem_iget(dest, src, 2, 2, 5, 0);                          \
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

  result &= TEST_C11_SHMEM_IGET(long);
  result &= TEST_C11_SHMEM_IGET(double);
  result &= TEST_C11_SHMEM_IGET(long double);
  result &= TEST_C11_SHMEM_IGET(char);
  result &= TEST_C11_SHMEM_IGET(signed char);
  result &= TEST_C11_SHMEM_IGET(short);
  result &= TEST_C11_SHMEM_IGET(int);
  result &= TEST_C11_SHMEM_IGET(long);
  result &= TEST_C11_SHMEM_IGET(long long);
  result &= TEST_C11_SHMEM_IGET(unsigned char);
  result &= TEST_C11_SHMEM_IGET(unsigned short);
  result &= TEST_C11_SHMEM_IGET(unsigned int);
  result &= TEST_C11_SHMEM_IGET(unsigned long);
  result &= TEST_C11_SHMEM_IGET(unsigned long long);
  result &= TEST_C11_SHMEM_IGET(int8_t);
  result &= TEST_C11_SHMEM_IGET(int16_t);
  result &= TEST_C11_SHMEM_IGET(int32_t);
  result &= TEST_C11_SHMEM_IGET(int64_t);
  result &= TEST_C11_SHMEM_IGET(uint8_t);
  result &= TEST_C11_SHMEM_IGET(uint16_t);
  result &= TEST_C11_SHMEM_IGET(uint32_t);
  result &= TEST_C11_SHMEM_IGET(uint64_t);
  result &= TEST_C11_SHMEM_IGET(size_t);
  result &= TEST_C11_SHMEM_IGET(ptrdiff_t);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_iget()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return rc;
}
