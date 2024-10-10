/**
 * @file c11_shmem_min_reduce.c
 * @brief Unit test for shmem_min_reduce().
 */

#include "shmemvv.h"

#define TEST_C11_SHMEM_MIN_REDUCE(TYPE) \
  ({                                                                           \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(sizeof(TYPE));                            \
    TYPE *dest = (TYPE *)shmem_malloc(sizeof(TYPE));                           \
                                                                               \
    *src = mype;                                                               \
                                                                               \
    shmem_min_reduce(SHMEM_TEAM_WORLD, dest, src, 1);             \
                                                                               \
    bool success = (*dest == 0);                                               \
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

  result &= TEST_C11_SHMEM_MIN_REDUCE(float);
  result &= TEST_C11_SHMEM_MIN_REDUCE(double);
  result &= TEST_C11_SHMEM_MIN_REDUCE(long double);
  result &= TEST_C11_SHMEM_MIN_REDUCE(char);
  result &= TEST_C11_SHMEM_MIN_REDUCE(signed char);
  result &= TEST_C11_SHMEM_MIN_REDUCE(short);
  result &= TEST_C11_SHMEM_MIN_REDUCE(int);
  result &= TEST_C11_SHMEM_MIN_REDUCE(long);
  result &= TEST_C11_SHMEM_MIN_REDUCE(long long);
  result &= TEST_C11_SHMEM_MIN_REDUCE(unsigned char);
  result &= TEST_C11_SHMEM_MIN_REDUCE(unsigned short);
  result &= TEST_C11_SHMEM_MIN_REDUCE(unsigned int);
  result &= TEST_C11_SHMEM_MIN_REDUCE(unsigned long);
  result &= TEST_C11_SHMEM_MIN_REDUCE(unsigned long long);
  result &= TEST_C11_SHMEM_MIN_REDUCE(int8_t);
  result &= TEST_C11_SHMEM_MIN_REDUCE(int16_t);
  result &= TEST_C11_SHMEM_MIN_REDUCE(int32_t);
  result &= TEST_C11_SHMEM_MIN_REDUCE(int64_t);
  result &= TEST_C11_SHMEM_MIN_REDUCE(uint8_t);
  result &= TEST_C11_SHMEM_MIN_REDUCE(uint16_t);
  result &= TEST_C11_SHMEM_MIN_REDUCE(uint32_t);
  result &= TEST_C11_SHMEM_MIN_REDUCE(uint64_t);
  result &= TEST_C11_SHMEM_MIN_REDUCE(size_t);
  result &= TEST_C11_SHMEM_MIN_REDUCE(ptrdiff_t);

  shmem_barrier_all();

  if (result) {
    if (shmem_my_pe() == 0) {
      display_test_result("C11 shmem_min_reduce()", result, false);
    }
  } else {
    if (shmem_my_pe() == 0) {
      display_test_result("C11 shmem_min_reduce()", result, false);
      rc = EXIT_FAILURE;
    }
  }

  shmem_finalize();
  return rc;
} 
