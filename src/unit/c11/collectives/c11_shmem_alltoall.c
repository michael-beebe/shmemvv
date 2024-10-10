/**
 * @file c11_shmem_alltoall.c
 *
 * @brief Unit test for shmem_alltoall().
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <shmem.h>

#include "shmemvv.h"

#define TEST_C11_SHMEM_ALLTOALL(TYPE) \
  ({                                                                           \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(npes * sizeof(TYPE));                     \
    TYPE *dest = (TYPE *)shmem_malloc(npes * sizeof(TYPE));                    \
                                                                               \
    for (int i = 0; i < npes; ++i) {                                           \
      src[i] = mype + i;                                                       \
    }                                                                          \
                                                                               \
    shmem_alltoall(SHMEM_TEAM_WORLD, dest, src, 1);               \
                                                                               \
    bool success = true;                                                       \
    for (int i = 0; i < npes; ++i) {                                           \
      if (dest[i] != mype + i) {                                               \
        success = false;                                                       \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
                                                                               \
    shmem_free(src);                                                           \
    shmem_free(dest);                                                          \
                                                                               \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();

  int result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C11_SHMEM_ALLTOALL(float);
  result &= TEST_C11_SHMEM_ALLTOALL(double);
  result &= TEST_C11_SHMEM_ALLTOALL(long double);
  result &= TEST_C11_SHMEM_ALLTOALL(char);
  result &= TEST_C11_SHMEM_ALLTOALL(signed char);
  result &= TEST_C11_SHMEM_ALLTOALL(short);
  result &= TEST_C11_SHMEM_ALLTOALL(int);
  result &= TEST_C11_SHMEM_ALLTOALL(long);
  result &= TEST_C11_SHMEM_ALLTOALL(long long);
  result &= TEST_C11_SHMEM_ALLTOALL(unsigned char);
  result &= TEST_C11_SHMEM_ALLTOALL(unsigned short);
  result &= TEST_C11_SHMEM_ALLTOALL(unsigned int);
  result &= TEST_C11_SHMEM_ALLTOALL(unsigned long);
  result &= TEST_C11_SHMEM_ALLTOALL(unsigned long long);
  result &= TEST_C11_SHMEM_ALLTOALL(int8_t);
  result &= TEST_C11_SHMEM_ALLTOALL(int16_t);
  result &= TEST_C11_SHMEM_ALLTOALL(int32_t);
  result &= TEST_C11_SHMEM_ALLTOALL(int64_t);
  result &= TEST_C11_SHMEM_ALLTOALL(uint8_t);
  result &= TEST_C11_SHMEM_ALLTOALL(uint16_t);
  result &= TEST_C11_SHMEM_ALLTOALL(uint32_t);
  result &= TEST_C11_SHMEM_ALLTOALL(uint64_t);
  result &= TEST_C11_SHMEM_ALLTOALL(size_t);
  result &= TEST_C11_SHMEM_ALLTOALL(ptrdiff_t);

  shmem_barrier_all();

  if (result) {
    if (shmem_my_pe() == 0) {
      display_test_result("C11 shmem_alltoall()", result, false);
    }
  } else {
    if (shmem_my_pe() == 0) {
      display_test_result("C11 shmem_alltoall()", result, false);
      rc = EXIT_FAILURE;
    }
  }

  shmem_finalize();
  return rc;
}
