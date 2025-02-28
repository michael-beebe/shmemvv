/**
 * @file c_shmem_min_reduce.c
 * @brief Unit test for shmem_min_reduce().
 */

#include "log.h"
#include "shmemvv.h"

#define TEST_C_SHMEM_MIN_REDUCE(TYPE, TYPENAME)                                \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_min_reduce");                             \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(sizeof(TYPE));                            \
    TYPE *dest = (TYPE *)shmem_malloc(sizeof(TYPE));                           \
    log_info("shmem_malloc'd %d bytes @ &src = %p, %d bytes @ &dest = %p",     \
             sizeof(TYPE), (void *)src, sizeof(TYPE), (void *)dest);           \
                                                                               \
    *src = mype;                                                               \
    log_info("set %p (src) to %d", (void *)src, mype);                         \
                                                                               \
    log_info("executing shmem_min_reduce: dest = %p, src = %p", (void *)dest,  \
             (void *)src);                                                     \
    shmem_##TYPENAME##_min_reduce(SHMEM_TEAM_WORLD, dest, src, 1);             \
                                                                               \
    log_info("validating result...");                                          \
    bool success = (*dest == 0);                                               \
                                                                               \
    if (success)                                                               \
      log_info("shmem_" #TYPENAME "_min_reduce produced expected result.");    \
    else                                                                       \
      log_fail("unexpected result from shmem_min_reduce");                     \
                                                                               \
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

  result &= TEST_C_SHMEM_MIN_REDUCE(float, float);
  result &= TEST_C_SHMEM_MIN_REDUCE(double, double);
  result &= TEST_C_SHMEM_MIN_REDUCE(long double, longdouble);
  result &= TEST_C_SHMEM_MIN_REDUCE(char, char);
  result &= TEST_C_SHMEM_MIN_REDUCE(signed char, schar);
  result &= TEST_C_SHMEM_MIN_REDUCE(short, short);
  result &= TEST_C_SHMEM_MIN_REDUCE(int, int);
  result &= TEST_C_SHMEM_MIN_REDUCE(long, long);
  result &= TEST_C_SHMEM_MIN_REDUCE(long long, longlong);
  result &= TEST_C_SHMEM_MIN_REDUCE(unsigned char, uchar);
  result &= TEST_C_SHMEM_MIN_REDUCE(unsigned short, ushort);
  result &= TEST_C_SHMEM_MIN_REDUCE(unsigned int, uint);
  result &= TEST_C_SHMEM_MIN_REDUCE(unsigned long, ulong);
  result &= TEST_C_SHMEM_MIN_REDUCE(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_MIN_REDUCE(int8_t, int8);
  result &= TEST_C_SHMEM_MIN_REDUCE(int16_t, int16);
  result &= TEST_C_SHMEM_MIN_REDUCE(int32_t, int32);
  result &= TEST_C_SHMEM_MIN_REDUCE(int64_t, int64);
  result &= TEST_C_SHMEM_MIN_REDUCE(uint8_t, uint8);
  result &= TEST_C_SHMEM_MIN_REDUCE(uint16_t, uint16);
  result &= TEST_C_SHMEM_MIN_REDUCE(uint32_t, uint32);
  result &= TEST_C_SHMEM_MIN_REDUCE(uint64_t, uint64);
  result &= TEST_C_SHMEM_MIN_REDUCE(size_t, size);
  result &= TEST_C_SHMEM_MIN_REDUCE(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_min_reduce()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
