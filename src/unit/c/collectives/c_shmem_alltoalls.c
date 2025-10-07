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

#define TEST_C_SHMEM_ALLTOALLS(TYPE, TYPENAME, DST_STRIDE, SST_STRIDE, NELEMS) \
  ({                                                                           \
    log_routine("shmem_alltoalls(" #TYPE ", dst=" #DST_STRIDE                  \
                ", sst=" #SST_STRIDE ", nelems=" #NELEMS ")");                 \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    /* Calculate buffer sizes according to OpenSHMEM spec */                   \
    size_t src_size = SST_STRIDE * NELEMS * npes * sizeof(TYPE);               \
    size_t dst_size = DST_STRIDE * NELEMS * npes * sizeof(TYPE);               \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(src_size);                                \
    TYPE *dest = (TYPE *)shmem_malloc(dst_size);                               \
    log_info("shmem_malloc'd %zu bytes @ &src = %p, %zu bytes @ &dest = %p",   \
             src_size, (void *)src, dst_size, (void *)dest);                   \
                                                                               \
    /* Initialize source array - each PE puts its PE number in specific        \
     * positions */                                                            \
    for (int pe = 0; pe < npes; pe++) {                                        \
      for (int i = 0; i < NELEMS; i++) {                                       \
        size_t offset = (pe * NELEMS + i) * SST_STRIDE;                        \
        src[offset] = (TYPE)mype;                                              \
      }                                                                        \
    }                                                                          \
                                                                               \
    /* Initialize destination array to a known bad value */                    \
    for (size_t i = 0; i < dst_size / sizeof(TYPE); i++) {                     \
      dest[i] = (TYPE) - 1;                                                    \
    }                                                                          \
                                                                               \
    log_info("set source elements at stride %d positions to %d", SST_STRIDE,   \
             mype);                                                            \
                                                                               \
    /* Ensure all PEs are ready before starting alltoalls */                   \
    shmem_barrier_all();                                                       \
                                                                               \
    log_info("executing shmem_alltoalls: dest = %p, src = %p", (void *)dest,   \
             (void *)src);                                                     \
    shmem_##TYPENAME##_alltoalls(SHMEM_TEAM_WORLD, dest, src, DST_STRIDE,      \
                                 SST_STRIDE, NELEMS);                          \
                                                                               \
    /* Ensure all PEs complete the alltoalls before validation */              \
    shmem_barrier_all();                                                       \
                                                                               \
    log_info("validating result...");                                          \
    bool success = true;                                                       \
    for (int pe = 0; pe < npes; pe++) {                                        \
      for (int i = 0; i < NELEMS; i++) {                                       \
        size_t offset = (pe * NELEMS + i) * DST_STRIDE;                        \
        TYPE expected = (TYPE)pe;                                              \
        if (dest[offset] != expected) {                                        \
          log_info("dest[%zu] failed. expected %d, got %d", offset,            \
                   (int)expected, (int)dest[offset]);                          \
          success = false;                                                     \
          break;                                                               \
        }                                                                      \
      }                                                                        \
      if (!success)                                                            \
        break;                                                                 \
    }                                                                          \
                                                                               \
    if (success)                                                               \
      log_info("shmem_alltoalls on " #TYPE " with dst=" #DST_STRIDE            \
               ", sst=" #SST_STRIDE ", nelems=" #NELEMS                        \
               " produced expected result.");                                  \
    else                                                                       \
      log_fail(                                                                \
          "at least one value was unexpected in result of shmem_alltoalls");   \
                                                                               \
    /* Print sample results for first few PEs */                               \
    if (mype < 2) {                                                            \
      log_info("PE %d: Sample results:", mype);                                \
      for (int pe = 0; pe < npes && pe < 3; pe++) {                            \
        for (int i = 0; i < NELEMS && i < 2; i++) {                            \
          size_t offset = (pe * NELEMS + i) * DST_STRIDE;                      \
          log_info("  dest[%zu] = %d", offset, (int)dest[offset]);             \
        }                                                                      \
      }                                                                        \
    }                                                                          \
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

  result &= TEST_C_SHMEM_ALLTOALLS(int, int, 1, 1, 1);
  result &= TEST_C_SHMEM_ALLTOALLS(int, int, 2, 1, 1);
  result &= TEST_C_SHMEM_ALLTOALLS(int, int, 1, 2, 1);
  result &= TEST_C_SHMEM_ALLTOALLS(int, int, 2, 2, 1);
  result &= TEST_C_SHMEM_ALLTOALLS(int, int, 1, 1, 2);
  result &= TEST_C_SHMEM_ALLTOALLS(int, int, 2, 1, 2);
  result &= TEST_C_SHMEM_ALLTOALLS(int, int, 2, 2, 2);
  result &= TEST_C_SHMEM_ALLTOALLS(int, int, 3, 3, 3);
  result &= TEST_C_SHMEM_ALLTOALLS(int64_t, int64, 2, 2, 2);
  result &= TEST_C_SHMEM_ALLTOALLS(double, double, 2, 1, 2);
  result &= TEST_C_SHMEM_ALLTOALLS(char, char, 1, 1, 1);
  result &= TEST_C_SHMEM_ALLTOALLS(signed char, schar, 1, 1, 1);
  result &= TEST_C_SHMEM_ALLTOALLS(short, short, 1, 1, 1);
  result &= TEST_C_SHMEM_ALLTOALLS(int, int, 1, 1, 1);
  result &= TEST_C_SHMEM_ALLTOALLS(long, long, 1, 1, 1);
  result &= TEST_C_SHMEM_ALLTOALLS(long long, longlong, 1, 1, 1);
  result &= TEST_C_SHMEM_ALLTOALLS(unsigned char, uchar, 1, 1, 1);
  result &= TEST_C_SHMEM_ALLTOALLS(unsigned short, ushort, 1, 1, 1);
  result &= TEST_C_SHMEM_ALLTOALLS(unsigned int, uint, 1, 1, 1);
  result &= TEST_C_SHMEM_ALLTOALLS(unsigned long, ulong, 1, 1, 1);
  result &= TEST_C_SHMEM_ALLTOALLS(unsigned long long, ulonglong, 1, 1, 1);
  result &= TEST_C_SHMEM_ALLTOALLS(int8_t, int8, 1, 1, 1);
  result &= TEST_C_SHMEM_ALLTOALLS(int16_t, int16, 1, 1, 1);
  result &= TEST_C_SHMEM_ALLTOALLS(int32_t, int32, 1, 1, 1);
  result &= TEST_C_SHMEM_ALLTOALLS(uint8_t, uint8, 1, 1, 1);
  result &= TEST_C_SHMEM_ALLTOALLS(uint16_t, uint16, 1, 1, 1);
  result &= TEST_C_SHMEM_ALLTOALLS(uint32_t, uint32, 1, 1, 1);
  result &= TEST_C_SHMEM_ALLTOALLS(uint64_t, uint64, 1, 1, 1);
  result &= TEST_C_SHMEM_ALLTOALLS(size_t, size, 1, 1, 1);
  result &= TEST_C_SHMEM_ALLTOALLS(ptrdiff_t, ptrdiff, 1, 1, 1);
  result &= TEST_C_SHMEM_ALLTOALLS(float, float, 1, 1, 1);
  result &= TEST_C_SHMEM_ALLTOALLS(long double, longdouble, 1, 1, 1);

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
