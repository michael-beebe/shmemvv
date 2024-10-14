/**
 * @file c11_shmem_alltoalls.c
 *
 * @brief Unit test for shmem_alltoalls().
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"

#define TEST_C11_SHMEM_ALLTOALLS(TYPE)                                         \
  ({                                                                           \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(npes * npes * sizeof(TYPE));              \
    TYPE *dest = (TYPE *)shmem_malloc(npes * npes * sizeof(TYPE));             \
                                                                               \
    for (int i = 0; i < npes; ++i) {                                           \
      src[i] = mype + i * npes;                                                \
    }                                                                          \
                                                                               \
    shmem_alltoalls(SHMEM_TEAM_WORLD, dest, src, 1, 1, npes);                  \
                                                                               \
    bool success = true;                                                       \
    for (int i = 0; i < npes; ++i) {                                           \
      if (dest[i] != i * npes + mype) {                                        \
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

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C11_SHMEM_ALLTOALLS(float);
  result &= TEST_C11_SHMEM_ALLTOALLS(double);
  result &= TEST_C11_SHMEM_ALLTOALLS(long double);
  result &= TEST_C11_SHMEM_ALLTOALLS(char);
  result &= TEST_C11_SHMEM_ALLTOALLS(signed char);
  result &= TEST_C11_SHMEM_ALLTOALLS(short);
  result &= TEST_C11_SHMEM_ALLTOALLS(int);
  result &= TEST_C11_SHMEM_ALLTOALLS(long);
  result &= TEST_C11_SHMEM_ALLTOALLS(long long);
  result &= TEST_C11_SHMEM_ALLTOALLS(unsigned char);
  result &= TEST_C11_SHMEM_ALLTOALLS(unsigned short);
  result &= TEST_C11_SHMEM_ALLTOALLS(unsigned int);
  result &= TEST_C11_SHMEM_ALLTOALLS(unsigned long);
  result &= TEST_C11_SHMEM_ALLTOALLS(unsigned long long);
  result &= TEST_C11_SHMEM_ALLTOALLS(int8_t);
  result &= TEST_C11_SHMEM_ALLTOALLS(int16_t);
  result &= TEST_C11_SHMEM_ALLTOALLS(int32_t);
  result &= TEST_C11_SHMEM_ALLTOALLS(int64_t);
  result &= TEST_C11_SHMEM_ALLTOALLS(uint8_t);
  result &= TEST_C11_SHMEM_ALLTOALLS(uint16_t);
  result &= TEST_C11_SHMEM_ALLTOALLS(uint32_t);
  result &= TEST_C11_SHMEM_ALLTOALLS(uint64_t);
  result &= TEST_C11_SHMEM_ALLTOALLS(size_t);
  result &= TEST_C11_SHMEM_ALLTOALLS(ptrdiff_t);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_alltoalls()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();

  return rc;
}
