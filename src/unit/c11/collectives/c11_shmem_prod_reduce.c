/**
 * @file c11_shmem_prod_reduce.c
 * @brief Unit test for shmem_prod_reduce().
 */

#include "shmemvv.h"

#define TEST_C11_SHMEM_PROD_REDUCE(TYPE) \
  ({                                                                           \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(sizeof(TYPE));                            \
    TYPE *dest = (TYPE *)shmem_malloc(sizeof(TYPE));                           \
                                                                               \
    *src = mype + 1;                                                           \
                                                                               \
    shmem_prod_reduce(SHMEM_TEAM_WORLD, dest, src, 1);            \
                                                                               \
    TYPE expected_prod = 1;                                                    \
    for (int i = 1; i <= npes; i++) {                                          \
      expected_prod *= i;                                                      \
    }                                                                          \
                                                                               \
    bool success = (*dest == expected_prod);                                   \
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

  result &= TEST_C11_SHMEM_PROD_REDUCE(float);
  result &= TEST_C11_SHMEM_PROD_REDUCE(double);
  result &= TEST_C11_SHMEM_PROD_REDUCE(long double);
  result &= TEST_C11_SHMEM_PROD_REDUCE(char);
  result &= TEST_C11_SHMEM_PROD_REDUCE(signed char);
  result &= TEST_C11_SHMEM_PROD_REDUCE(short);
  result &= TEST_C11_SHMEM_PROD_REDUCE(int);
  result &= TEST_C11_SHMEM_PROD_REDUCE(long);
  result &= TEST_C11_SHMEM_PROD_REDUCE(long long);
  result &= TEST_C11_SHMEM_PROD_REDUCE(unsigned char);
  result &= TEST_C11_SHMEM_PROD_REDUCE(unsigned short);
  result &= TEST_C11_SHMEM_PROD_REDUCE(unsigned int);
  result &= TEST_C11_SHMEM_PROD_REDUCE(unsigned long);
  result &= TEST_C11_SHMEM_PROD_REDUCE(unsigned long long);
  result &= TEST_C11_SHMEM_PROD_REDUCE(int8_t);
  result &= TEST_C11_SHMEM_PROD_REDUCE(int16_t);
  result &= TEST_C11_SHMEM_PROD_REDUCE(int32_t);
  result &= TEST_C11_SHMEM_PROD_REDUCE(int64_t);
  result &= TEST_C11_SHMEM_PROD_REDUCE(uint8_t);
  result &= TEST_C11_SHMEM_PROD_REDUCE(uint16_t);
  result &= TEST_C11_SHMEM_PROD_REDUCE(uint32_t);
  result &= TEST_C11_SHMEM_PROD_REDUCE(uint64_t);
  result &= TEST_C11_SHMEM_PROD_REDUCE(size_t);
  result &= TEST_C11_SHMEM_PROD_REDUCE(ptrdiff_t);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_prod_reduce()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return rc;
} 
