/**
 * @file c11_shmem_alltoall.c
 *
 * @brief Unit test for shmem_alltoall().
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C11_SHMEM_ALLTOALL(TYPE)                                          \
  ({                                                                           \
    log_routine("shmem_alltoall(" #TYPE ")");                                  \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(npes * sizeof(TYPE));                     \
    TYPE *dest = (TYPE *)shmem_malloc(npes * sizeof(TYPE));                    \
    log_info("shmem_malloc'd %d bytes @ &src = %p, %d bytes @ &dest = %p",     \
             npes * sizeof(TYPE), (void *)src, npes * sizeof(TYPE),            \
             (void *)dest);                                                    \
                                                                               \
    for (int i = 0; i < npes; ++i) {                                           \
      src[i] = mype + i;                                                       \
    }                                                                          \
    log_info("set %p..%p to %d + idx", (void *)src, (void *)&src[npes - 1],    \
             mype);                                                            \
                                                                               \
    log_info("executing shmem_alltoall: dest = %p, src = %p", (void *)dest,    \
             (void *)src);                                                     \
    shmem_alltoall(SHMEM_TEAM_WORLD, dest, src, 1);                            \
                                                                               \
    log_info("validating result...");                                          \
    bool success = true;                                                       \
    for (int i = 0; i < npes; ++i) {                                           \
      if (dest[i] != mype + i) {                                               \
        log_info("index %d of dest (%p) failed. expected %d, got %d", i,       \
                 &dest[i], mype + i, (char)dest[i]);                           \
        success = false;                                                       \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
                                                                               \
    if (success)                                                               \
      log_info("shmem_alltoall on " #TYPE " produced expected result.");       \
    else                                                                       \
      log_fail(                                                                \
          "at least one value was unexpected in result of shmem_alltoall");    \
    shmem_free(src);                                                           \
    shmem_free(dest);                                                          \
                                                                               \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  int result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C11_SHMEM_ALLTOALL(float);
  result &= TEST_C11_SHMEM_ALLTOALL(double);
  result &= TEST_C11_SHMEM_ALLTOALL(long double);
  result &= TEST_C11_SHMEM_ALLTOALL(char);
  result &= TEST_C11_SHMEM_ALLTOALL(signed char);
  result &= TEST_C11_SHMEM_ALLTOALL(short);
  result &= TEST_C11_SHMEM_ALLTOALL(int);
  result &= TEST_C11_SHMEM_ALLTOALL(long);
  result &= TEST_C11_SHMEM_ALLTOALL(long long);
  result &= TEST_C11_SHMEM_ALLTOALL(unsigned char);
  result &= TEST_C11_SHMEM_ALLTOALL(unsigned short);
  result &= TEST_C11_SHMEM_ALLTOALL(unsigned int);
  result &= TEST_C11_SHMEM_ALLTOALL(unsigned long);
  result &= TEST_C11_SHMEM_ALLTOALL(unsigned long long);
  result &= TEST_C11_SHMEM_ALLTOALL(int8_t);
  result &= TEST_C11_SHMEM_ALLTOALL(int16_t);
  result &= TEST_C11_SHMEM_ALLTOALL(int32_t);
  result &= TEST_C11_SHMEM_ALLTOALL(int64_t);
  result &= TEST_C11_SHMEM_ALLTOALL(uint8_t);
  result &= TEST_C11_SHMEM_ALLTOALL(uint16_t);
  result &= TEST_C11_SHMEM_ALLTOALL(uint32_t);
  result &= TEST_C11_SHMEM_ALLTOALL(uint64_t);
  result &= TEST_C11_SHMEM_ALLTOALL(size_t);
  result &= TEST_C11_SHMEM_ALLTOALL(ptrdiff_t);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_alltoall()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
