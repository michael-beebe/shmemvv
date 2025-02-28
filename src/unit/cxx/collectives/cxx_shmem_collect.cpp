/**
 * @file c_shmem_collect.cpp
 * @brief Unit test for shmem_collect().
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_CXX_SHMEM_COLLECT(TYPE, TYPENAME)                                 \
  ({                                                                           \
    log_routine("shmem_collect(" #TYPE ")");                                   \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(sizeof(TYPE));                            \
    TYPE *dest = (TYPE *)shmem_malloc(npes * sizeof(TYPE));                    \
    log_info("shmem_malloc'd %d bytes @ &src = %p, %d bytes @ &dest = %p",     \
             sizeof(TYPE), (void *)src, npes * sizeof(TYPE), (void *)dest);    \
                                                                               \
    src[0] = mype;                                                             \
    log_info("set %p to %d", (void *)src, mype);                               \
                                                                               \
    log_info("executing shmem_collect: dest = %p, src = %p", (void *)dest,     \
             (void *)src);                                                     \
    shmem_##TYPENAME##_collect(SHMEM_TEAM_WORLD, dest, src, 1);                \
                                                                               \
    log_info("validating result...");                                          \
    bool success = true;                                                       \
    for (int i = 0; i < npes; ++i) {                                           \
      if (dest[i] != i) {                                                      \
        log_info("index %d of dest (%p) failed. expected %d, got %d", i,       \
                 &dest[i], i, (char)dest[i]);                                  \
        success = false;                                                       \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
                                                                               \
    if (success)                                                               \
      log_info("shmem_collect on " #TYPE " produced expected result.");        \
    else                                                                       \
      log_fail(                                                                \
          "at least one value was unexpected in result of shmem_collect");     \
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

  result &= TEST_CXX_SHMEM_COLLECT(float, float);
  result &= TEST_CXX_SHMEM_COLLECT(double, double);
  result &= TEST_CXX_SHMEM_COLLECT(long double, longdouble);
  result &= TEST_CXX_SHMEM_COLLECT(char, char);
  result &= TEST_CXX_SHMEM_COLLECT(signed char, schar);
  result &= TEST_CXX_SHMEM_COLLECT(short, short);
  result &= TEST_CXX_SHMEM_COLLECT(int, int);
  result &= TEST_CXX_SHMEM_COLLECT(long, long);
  result &= TEST_CXX_SHMEM_COLLECT(long long, longlong);
  result &= TEST_CXX_SHMEM_COLLECT(unsigned char, uchar);
  result &= TEST_CXX_SHMEM_COLLECT(unsigned short, ushort);
  result &= TEST_CXX_SHMEM_COLLECT(unsigned int, uint);
  result &= TEST_CXX_SHMEM_COLLECT(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_COLLECT(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_COLLECT(int8_t, int8);
  result &= TEST_CXX_SHMEM_COLLECT(int16_t, int16);
  result &= TEST_CXX_SHMEM_COLLECT(int32_t, int32);
  result &= TEST_CXX_SHMEM_COLLECT(int64_t, int64);
  result &= TEST_CXX_SHMEM_COLLECT(uint8_t, uint8);
  result &= TEST_CXX_SHMEM_COLLECT(uint16_t, uint16);
  result &= TEST_CXX_SHMEM_COLLECT(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_COLLECT(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_COLLECT(size_t, size);
  result &= TEST_CXX_SHMEM_COLLECT(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_collect()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
