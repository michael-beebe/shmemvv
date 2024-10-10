/**
 * @file c_shmem_sum_reduce.c
 * @brief Unit test for shmem_sum_reduce().
 */

#include "shmemvv.h"

#define TEST_C_SHMEM_SUM_REDUCE(TYPE, TYPENAME) \
  ({                                                                           \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(sizeof(TYPE));                            \
    TYPE *dest = (TYPE *)shmem_malloc(sizeof(TYPE));                           \
                                                                               \
    *src = mype;                                                               \
                                                                               \
    shmem_##TYPENAME##_sum_reduce(SHMEM_TEAM_WORLD, dest, src, 1);             \
                                                                               \
    TYPE expected_sum = npes * (npes - 1) / 2;                                 \
    bool success = (*dest == expected_sum);                                    \
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

  result &= TEST_C_SHMEM_SUM_REDUCE(float, float);
  result &= TEST_C_SHMEM_SUM_REDUCE(double, double);
  result &= TEST_C_SHMEM_SUM_REDUCE(long double, longdouble);
  result &= TEST_C_SHMEM_SUM_REDUCE(char, char);
  result &= TEST_C_SHMEM_SUM_REDUCE(signed char, schar);
  result &= TEST_C_SHMEM_SUM_REDUCE(short, short);
  result &= TEST_C_SHMEM_SUM_REDUCE(int, int);
  result &= TEST_C_SHMEM_SUM_REDUCE(long, long);
  result &= TEST_C_SHMEM_SUM_REDUCE(long long, longlong);
  result &= TEST_C_SHMEM_SUM_REDUCE(unsigned char, uchar);
  result &= TEST_C_SHMEM_SUM_REDUCE(unsigned short, ushort);
  result &= TEST_C_SHMEM_SUM_REDUCE(unsigned int, uint);
  result &= TEST_C_SHMEM_SUM_REDUCE(unsigned long, ulong);
  result &= TEST_C_SHMEM_SUM_REDUCE(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_SUM_REDUCE(int8_t, int8);
  result &= TEST_C_SHMEM_SUM_REDUCE(int16_t, int16);
  result &= TEST_C_SHMEM_SUM_REDUCE(int32_t, int32);
  result &= TEST_C_SHMEM_SUM_REDUCE(int64_t, int64);
  result &= TEST_C_SHMEM_SUM_REDUCE(uint8_t, uint8);
  result &= TEST_C_SHMEM_SUM_REDUCE(uint16_t, uint16);
  result &= TEST_C_SHMEM_SUM_REDUCE(uint32_t, uint32);
  result &= TEST_C_SHMEM_SUM_REDUCE(uint64_t, uint64);
  result &= TEST_C_SHMEM_SUM_REDUCE(size_t, size);
  result &= TEST_C_SHMEM_SUM_REDUCE(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_sum_reduce()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return rc;
} 
