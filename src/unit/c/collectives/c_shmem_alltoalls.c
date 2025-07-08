/**
 * @file c_shmem_alltoalls.c
 *
 * @brief Unit test for shmem_alltoalls().
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C_SHMEM_ALLTOALLS(TYPE, TYPENAME)                                 \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_alltoalls");                              \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(npes * sizeof(TYPE));                     \
    TYPE *dest = (TYPE *)shmem_malloc(npes * sizeof(TYPE));                    \
    log_info("shmem_malloc'd %d bytes @ &src = %p, %d bytes @ &dest = %p",     \
             npes * sizeof(TYPE), (void *)src, npes * sizeof(TYPE),            \
             (void *)dest);                                                    \
                                                                               \
    /* Initialize source array - each PE puts its PE number in all elements */ \
    for (int i = 0; i < npes; ++i) {                                           \
      src[i] = (TYPE)mype;                                                     \
    }                                                                          \
    log_info("set %p..%p to %d", (void *)src, (void *)&src[npes - 1], mype);   \
                                                                               \
    /* Initialize destination array to a known bad value */                    \
    for (int i = 0; i < npes; ++i) {                                           \
      dest[i] = (TYPE) - 1;                                                    \
    }                                                                          \
                                                                               \
    /* Ensure all PEs are ready before starting alltoalls */                   \
    shmem_barrier_all();                                                       \
                                                                               \
    log_info("executing shmem_alltoalls: dest = %p, src = %p", (void *)dest,   \
             (void *)src);                                                     \
    shmem_##TYPENAME##_alltoalls(SHMEM_TEAM_WORLD, dest, src, 1, 1, 1);        \
                                                                               \
    /* Ensure all PEs complete the alltoalls before validation */              \
    shmem_barrier_all();                                                       \
                                                                               \
    log_info("validating result...");                                          \
    bool success = true;                                                       \
    for (int i = 0; i < npes; ++i) {                                           \
      /* After alltoalls, dest[i] should contain the value that PE i */        \
      /* had in src[mype], which is just i (since each PE puts its PE# */      \
      /* in all source elements) */                                            \
      TYPE expected = (TYPE)i;                                                 \
      if (dest[i] != expected) {                                               \
        log_info("index %d of dest (%p) failed. expected %d, got %d", i,       \
                 &dest[i], (int)expected, (int)dest[i]);                       \
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

  result &= TEST_C_SHMEM_ALLTOALLS(float, float);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_ALLTOALLS(double, double);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_ALLTOALLS(long double, longdouble);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_ALLTOALLS(char, char);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_ALLTOALLS(signed char, schar);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_ALLTOALLS(short, short);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_ALLTOALLS(int, int);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_ALLTOALLS(long, long);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_ALLTOALLS(long long, longlong);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_ALLTOALLS(unsigned char, uchar);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_ALLTOALLS(unsigned short, ushort);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_ALLTOALLS(unsigned int, uint);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_ALLTOALLS(unsigned long, ulong);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_ALLTOALLS(unsigned long long, ulonglong);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_ALLTOALLS(int8_t, int8);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_ALLTOALLS(int16_t, int16);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_ALLTOALLS(int32_t, int32);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_ALLTOALLS(int64_t, int64);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_ALLTOALLS(uint8_t, uint8);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_ALLTOALLS(uint16_t, uint16);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_ALLTOALLS(uint32_t, uint32);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_ALLTOALLS(uint64_t, uint64);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_ALLTOALLS(size_t, size);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_ALLTOALLS(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_alltoalls", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();

  return rc;
}
