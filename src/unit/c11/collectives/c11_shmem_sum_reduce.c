/**
 * @file c11_shmem_sum_reduce.c
 * @brief Unit test for shmem_sum_reduce().
 */

#include "log.h"
#include "shmemvv.h"

#define TEST_C11_SHMEM_SUM_REDUCE(TYPE)                                        \
  ({                                                                           \
    log_routine("shmem_sum_reduce(" #TYPE ")");                                \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(sizeof(TYPE));                            \
    TYPE *dest = (TYPE *)shmem_malloc(sizeof(TYPE));                           \
    log_info("shmem_malloc'd %d bytes @ &src = %p, %d bytes @ &dest = %p",     \
             sizeof(TYPE), (void *)src, sizeof(TYPE), (void *)dest);           \
                                                                               \
    *src = mype;                                                               \
    log_info("set src (%p) to %d", (void *)src, mype);                         \
                                                                               \
    log_info("executing shmem_sum_reduce: dest = %p, src = %p", (void *)dest,  \
             (void *)src);                                                     \
    shmem_sum_reduce(SHMEM_TEAM_WORLD, dest, src, 1);                          \
                                                                               \
    /* this might fail on >16 pes. is this an issue? */                        \
    TYPE expected_sum = npes * (npes - 1) / 2;                                 \
    bool success = (*dest == expected_sum);                                    \
                                                                               \
    if (success)                                                               \
      log_info("shmem_sum_reduce on " #TYPE " produced expected result.");     \
    else                                                                       \
      log_fail("shmem_sum_reduce on " #TYPE                                    \
               " produced unexpected result: expected %d, found %d",           \
               (char)expected_sum, (char)*dest);                               \
    shmem_free(src);                                                           \
    shmem_free(dest);                                                          \
                                                                               \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C11_SHMEM_SUM_REDUCE(float);
  result &= TEST_C11_SHMEM_SUM_REDUCE(double);
  result &= TEST_C11_SHMEM_SUM_REDUCE(long double);
  result &= TEST_C11_SHMEM_SUM_REDUCE(char);
  result &= TEST_C11_SHMEM_SUM_REDUCE(signed char);
  result &= TEST_C11_SHMEM_SUM_REDUCE(short);
  result &= TEST_C11_SHMEM_SUM_REDUCE(int);
  result &= TEST_C11_SHMEM_SUM_REDUCE(long);
  result &= TEST_C11_SHMEM_SUM_REDUCE(long long);
  result &= TEST_C11_SHMEM_SUM_REDUCE(unsigned char);
  result &= TEST_C11_SHMEM_SUM_REDUCE(unsigned short);
  result &= TEST_C11_SHMEM_SUM_REDUCE(unsigned int);
  result &= TEST_C11_SHMEM_SUM_REDUCE(unsigned long);
  result &= TEST_C11_SHMEM_SUM_REDUCE(unsigned long long);
  result &= TEST_C11_SHMEM_SUM_REDUCE(int8_t);
  result &= TEST_C11_SHMEM_SUM_REDUCE(int16_t);
  result &= TEST_C11_SHMEM_SUM_REDUCE(int32_t);
  result &= TEST_C11_SHMEM_SUM_REDUCE(int64_t);
  result &= TEST_C11_SHMEM_SUM_REDUCE(uint8_t);
  result &= TEST_C11_SHMEM_SUM_REDUCE(uint16_t);
  result &= TEST_C11_SHMEM_SUM_REDUCE(uint32_t);
  result &= TEST_C11_SHMEM_SUM_REDUCE(uint64_t);
  result &= TEST_C11_SHMEM_SUM_REDUCE(size_t);
  result &= TEST_C11_SHMEM_SUM_REDUCE(ptrdiff_t);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_sum_reduce", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
