/**
 * @file c11_shmem_alltoalls.c
 *
 * @brief Unit test for shmem_alltoalls().
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C11_SHMEM_ALLTOALLS(TYPE)                                         \
  ({                                                                           \
    log_routine("shmem_alltoalls(" #TYPE ")");                                   \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(npes * npes * sizeof(TYPE));              \
    TYPE *dest = (TYPE *)shmem_malloc(npes * npes * sizeof(TYPE));             \
    log_info("shmem_malloc'd %d bytes @ &src = %p, %d bytes @ &dest = %p",     \
             npes * sizeof(TYPE), (void *)src, npes * sizeof(TYPE), (void *)dest);    \
                                                                               \
    for (int i = 0; i < npes; ++i) {                                           \
      src[i] = mype + i * npes;                                                \
    }                                                                          \
    log_info("set %p..%p to %d + idx * %d", (void *)src, (void*)&src[npes - 1], mype, npes);                      \
                                                                               \
    log_info("executing shmem_alltoalls: dest = %p, src = %p", (void *)dest,     \
             (void *)src);                                                     \
    shmem_alltoalls(SHMEM_TEAM_WORLD, dest, src, 1, 1, npes);                  \
                                                                               \
    log_info("validating result...");                                          \
    bool success = true;                                                       \
    for (int i = 0; i < npes; ++i) {                                           \
      if (dest[i] != i * npes + mype) {                                        \
        log_info("index %d of dest (%p) failed. expected %d, got %d", i,       \
                 &dest[i], mype + i * npes, (char)dest[i]);                                  \
        success = false;                                                       \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
                                                                               \
    if (success)                                                               \
      log_info("shmem_alltoalls on " #TYPE " produced expected result.");        \
    else                                                                       \
      log_fail(                                                                \
          "at least one value was unexpected in result of shmem_alltoalls");     \
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

  result &= TEST_C11_SHMEM_ALLTOALLS(float);
  result &= TEST_C11_SHMEM_ALLTOALLS(double);
  result &= TEST_C11_SHMEM_ALLTOALLS(long double);
  result &= TEST_C11_SHMEM_ALLTOALLS(char);
  result &= TEST_C11_SHMEM_ALLTOALLS(signed char);
  result &= TEST_C11_SHMEM_ALLTOALLS(short);
  result &= TEST_C11_SHMEM_ALLTOALLS(int);
  result &= TEST_C11_SHMEM_ALLTOALLS(long);
  result &= TEST_C11_SHMEM_ALLTOALLS(long long);
  result &= TEST_C11_SHMEM_ALLTOALLS(unsigned char);
  result &= TEST_C11_SHMEM_ALLTOALLS(unsigned short);
  result &= TEST_C11_SHMEM_ALLTOALLS(unsigned int);
  result &= TEST_C11_SHMEM_ALLTOALLS(unsigned long);
  result &= TEST_C11_SHMEM_ALLTOALLS(unsigned long long);
  result &= TEST_C11_SHMEM_ALLTOALLS(int8_t);
  result &= TEST_C11_SHMEM_ALLTOALLS(int16_t);
  result &= TEST_C11_SHMEM_ALLTOALLS(int32_t);
  result &= TEST_C11_SHMEM_ALLTOALLS(int64_t);
  result &= TEST_C11_SHMEM_ALLTOALLS(uint8_t);
  result &= TEST_C11_SHMEM_ALLTOALLS(uint16_t);
  result &= TEST_C11_SHMEM_ALLTOALLS(uint32_t);
  result &= TEST_C11_SHMEM_ALLTOALLS(uint64_t);
  result &= TEST_C11_SHMEM_ALLTOALLS(size_t);
  result &= TEST_C11_SHMEM_ALLTOALLS(ptrdiff_t);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_alltoalls()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();

  return rc;
}
