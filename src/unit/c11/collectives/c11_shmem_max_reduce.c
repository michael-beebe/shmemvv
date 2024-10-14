/**
 * @file c11_shmem_max_reduce.c
 * @brief Unit test for shmem_max_reduce().
 */

#include "shmemvv.h"

#define TEST_C11_SHMEM_MAX_REDUCE(TYPE)                                        \
  ({                                                                           \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(sizeof(TYPE));                            \
    TYPE *dest = (TYPE *)shmem_malloc(sizeof(TYPE));                           \
                                                                               \
    *src = mype;                                                               \
                                                                               \
    shmem_max_reduce(SHMEM_TEAM_WORLD, dest, src, 1);                          \
                                                                               \
    bool success = (*dest == npes - 1);                                        \
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

  result &= TEST_C11_SHMEM_MAX_REDUCE(float);
  result &= TEST_C11_SHMEM_MAX_REDUCE(double);
  result &= TEST_C11_SHMEM_MAX_REDUCE(long double);
  result &= TEST_C11_SHMEM_MAX_REDUCE(char);
  result &= TEST_C11_SHMEM_MAX_REDUCE(signed char);
  result &= TEST_C11_SHMEM_MAX_REDUCE(short);
  result &= TEST_C11_SHMEM_MAX_REDUCE(int);
  result &= TEST_C11_SHMEM_MAX_REDUCE(long);
  result &= TEST_C11_SHMEM_MAX_REDUCE(long long);
  result &= TEST_C11_SHMEM_MAX_REDUCE(unsigned char);
  result &= TEST_C11_SHMEM_MAX_REDUCE(unsigned short);
  result &= TEST_C11_SHMEM_MAX_REDUCE(unsigned int);
  result &= TEST_C11_SHMEM_MAX_REDUCE(unsigned long);
  result &= TEST_C11_SHMEM_MAX_REDUCE(unsigned long long);
  result &= TEST_C11_SHMEM_MAX_REDUCE(int8_t);
  result &= TEST_C11_SHMEM_MAX_REDUCE(int16_t);
  result &= TEST_C11_SHMEM_MAX_REDUCE(int32_t);
  result &= TEST_C11_SHMEM_MAX_REDUCE(int64_t);
  result &= TEST_C11_SHMEM_MAX_REDUCE(uint8_t);
  result &= TEST_C11_SHMEM_MAX_REDUCE(uint16_t);
  result &= TEST_C11_SHMEM_MAX_REDUCE(uint32_t);
  result &= TEST_C11_SHMEM_MAX_REDUCE(uint64_t);
  result &= TEST_C11_SHMEM_MAX_REDUCE(size_t);
  result &= TEST_C11_SHMEM_MAX_REDUCE(ptrdiff_t);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_max_reduce()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return rc;
}
