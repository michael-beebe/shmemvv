/**
 * @file c_shmem_max_reduce.cpp
 * @brief Unit test for shmem_max_reduce().
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_CXX_SHMEM_MAX_REDUCE(TYPE, TYPENAME)                              \
  ({                                                                           \
    log_routine("shmem_max_reduce(" #TYPE ")");                                \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(sizeof(TYPE));                            \
    TYPE *dest = (TYPE *)shmem_malloc(sizeof(TYPE));                           \
    log_info("shmem_malloc'd %d bytes @ &src = %p, %d bytes @ &dest = %p",     \
             sizeof(TYPE), (void *)src, sizeof(TYPE), (void *)dest);           \
                                                                               \
    *src = mype;                                                               \
    log_info("set %p to %d", (void *)src, mype);                               \
                                                                               \
    log_info("executing shmem_max_reduce: dest = %p, src = %p", (void *)dest,  \
             (void *)src);                                                     \
    shmem_##TYPENAME##_max_reduce(SHMEM_TEAM_WORLD, dest, src, 1);             \
                                                                               \
    bool success = (*dest == npes - 1);                                        \
                                                                               \
    if (success)                                                               \
      log_info("shmem_max_reduce on " #TYPE " produced expected result.");     \
    else                                                                       \
      log_fail("unexpected result from shmem_max_reduce");                     \
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

  result &= TEST_CXX_SHMEM_MAX_REDUCE(float, float);
  result &= TEST_CXX_SHMEM_MAX_REDUCE(double, double);
  result &= TEST_CXX_SHMEM_MAX_REDUCE(long double, longdouble);
  result &= TEST_CXX_SHMEM_MAX_REDUCE(char, char);
  result &= TEST_CXX_SHMEM_MAX_REDUCE(signed char, schar);
  result &= TEST_CXX_SHMEM_MAX_REDUCE(short, short);
  result &= TEST_CXX_SHMEM_MAX_REDUCE(int, int);
  result &= TEST_CXX_SHMEM_MAX_REDUCE(long, long);
  result &= TEST_CXX_SHMEM_MAX_REDUCE(long long, longlong);
  result &= TEST_CXX_SHMEM_MAX_REDUCE(unsigned char, uchar);
  result &= TEST_CXX_SHMEM_MAX_REDUCE(unsigned short, ushort);
  result &= TEST_CXX_SHMEM_MAX_REDUCE(unsigned int, uint);
  result &= TEST_CXX_SHMEM_MAX_REDUCE(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_MAX_REDUCE(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_MAX_REDUCE(int8_t, int8);
  result &= TEST_CXX_SHMEM_MAX_REDUCE(int16_t, int16);
  result &= TEST_CXX_SHMEM_MAX_REDUCE(int32_t, int32);
  result &= TEST_CXX_SHMEM_MAX_REDUCE(int64_t, int64);
  result &= TEST_CXX_SHMEM_MAX_REDUCE(uint8_t, uint8);
  result &= TEST_CXX_SHMEM_MAX_REDUCE(uint16_t, uint16);
  result &= TEST_CXX_SHMEM_MAX_REDUCE(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_MAX_REDUCE(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_MAX_REDUCE(size_t, size);
  result &= TEST_CXX_SHMEM_MAX_REDUCE(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_max_reduce()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
