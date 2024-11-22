/**
 * @file c_shmem_alltoalls.cpp
 *
 * @brief Unit test for shmem_alltoalls().
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_CXX_SHMEM_ALLTOALLS(TYPE, TYPENAME)                               \
  ({                                                                           \
    log_routine("shmem_alltoalls(" #TYPE ")");                                 \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(npes * npes * sizeof(TYPE));              \
    TYPE *dest = (TYPE *)shmem_malloc(npes * npes * sizeof(TYPE));             \
    log_info("shmem_malloc'd %d bytes @ &src = %p, %d bytes @ &dest = %p",     \
             npes * npes * sizeof(TYPE), (void *)src,                          \
             npes * npes * sizeof(TYPE), (void *)dest);                        \
                                                                               \
    for (int i = 0; i < npes; ++i) {                                           \
      src[i] = mype + i * npes;                                                \
    }                                                                          \
    log_info("set %p..%p to %d + idx * %d", (void *)src,                      \
             (void *)&src[npes - 1], mype, npes);                              \
                                                                               \
    log_info("executing shmem_alltoalls: dest = %p, src = %p", (void *)dest,   \
             (void *)src);                                                     \
    shmem_##TYPENAME##_alltoalls(SHMEM_TEAM_WORLD, dest, src, 1, 1, npes);     \
                                                                               \
    log_info("validating result...");                                          \
    bool success = true;                                                       \
    for (int i = 0; i < npes; ++i) {                                           \
      if (dest[i] != i * npes + mype) {                                        \
        log_info("index %d of dest (%p) failed. expected %d, got %d", i,       \
                 &dest[i], i * npes + mype, (char)dest[i]);                    \
        success = false;                                                       \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
                                                                               \
    if (success)                                                               \
      log_info("shmem_alltoalls on " #TYPE " produced expected result.");      \
    else                                                                       \
      log_fail(                                                                \
          "at least one value was unexpected in result of shmem_alltoalls");   \
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

  result &= TEST_CXX_SHMEM_ALLTOALLS(float, float);
  result &= TEST_CXX_SHMEM_ALLTOALLS(double, double);
  result &= TEST_CXX_SHMEM_ALLTOALLS(long double, longdouble);
  result &= TEST_CXX_SHMEM_ALLTOALLS(char, char);
  result &= TEST_CXX_SHMEM_ALLTOALLS(signed char, schar);
  result &= TEST_CXX_SHMEM_ALLTOALLS(short, short);
  result &= TEST_CXX_SHMEM_ALLTOALLS(int, int);
  result &= TEST_CXX_SHMEM_ALLTOALLS(long, long);
  result &= TEST_CXX_SHMEM_ALLTOALLS(long long, longlong);
  result &= TEST_CXX_SHMEM_ALLTOALLS(unsigned char, uchar);
  result &= TEST_CXX_SHMEM_ALLTOALLS(unsigned short, ushort);
  result &= TEST_CXX_SHMEM_ALLTOALLS(unsigned int, uint);
  result &= TEST_CXX_SHMEM_ALLTOALLS(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_ALLTOALLS(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_ALLTOALLS(int8_t, int8);
  result &= TEST_CXX_SHMEM_ALLTOALLS(int16_t, int16);
  result &= TEST_CXX_SHMEM_ALLTOALLS(int32_t, int32);
  result &= TEST_CXX_SHMEM_ALLTOALLS(int64_t, int64);
  result &= TEST_CXX_SHMEM_ALLTOALLS(uint8_t, uint8);
  result &= TEST_CXX_SHMEM_ALLTOALLS(uint16_t, uint16);
  result &= TEST_CXX_SHMEM_ALLTOALLS(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_ALLTOALLS(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_ALLTOALLS(size_t, size);
  result &= TEST_CXX_SHMEM_ALLTOALLS(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_alltoalls()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();

  return rc;
}
