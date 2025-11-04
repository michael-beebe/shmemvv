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
#include "type_tables.h"

#define TEST_C11_SHMEM_ALLTOALLS(TYPE, DST_STRIDE, SST_STRIDE, NELEMS)       \
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
        src[offset] = (TYPE)((mype << 4) ^ (pe << 2) ^ i);                                              \
      }                                                                        \
    }                                                                          \
    log_info("set source elements at stride %d positions to %d", SST_STRIDE,   \
             mype);                                                            \
                                                                               \
    /* Initialize destination array to a known bad value */                    \
    for (size_t i = 0; i < dst_size / sizeof(TYPE); i++) {                     \
      dest[i] = (TYPE) - 1;                                                    \
    }                                                                          \
    log_info("set dest elements to a known bad value, -1");                    \
                                                                               \
                                                                               \
    /* Ensure all PEs are ready before starting alltoalls */                   \
    shmem_barrier_all();                                                       \
                                                                               \
    log_info("executing shmem_alltoalls: dest = %p, src = %p", (void *)dest,   \
             (void *)src);                                                     \
    shmem_alltoalls(SHMEM_TEAM_WORLD, dest, src, DST_STRIDE,                   \
                    SST_STRIDE, NELEMS);                                       \
                                                                               \
    /* Ensure all PEs complete the alltoalls before validation */              \
    shmem_barrier_all();                                                       \
                                                                               \
    log_info("validating result...");                                          \
    bool success = true;                                                       \
    for (int pe = 0; pe < npes; pe++) {                                        \
      for (int i = 0; i < NELEMS; i++) {                                       \
        size_t offset = (pe * NELEMS + i) * DST_STRIDE;                        \
        TYPE expected = (TYPE)((pe << 4) ^ (mype << 2) ^ i);                   \
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

  /* Test all data types */
  static int result = true;
  #define X(type, shmem_types) result &= TEST_C11_SHMEM_ALLTOALLS(type, 1, 1, 1);
    SHMEM_STANDARD_RMA_TYPE_TABLE(X)
  #undef X

  /* Test comprehensive functionality with int type using different strides and elements */
  result &= TEST_C11_SHMEM_ALLTOALLS(int, 2, 1, 1);
  result &= TEST_C11_SHMEM_ALLTOALLS(int, 1, 2, 1);
  result &= TEST_C11_SHMEM_ALLTOALLS(int, 2, 2, 1);
  result &= TEST_C11_SHMEM_ALLTOALLS(int, 1, 1, 2);
  result &= TEST_C11_SHMEM_ALLTOALLS(int, 2, 1, 2);
  result &= TEST_C11_SHMEM_ALLTOALLS(int, 2, 2, 2);
  result &= TEST_C11_SHMEM_ALLTOALLS(int, 3, 3, 3);

  /* Test with 64-bit types using different strides */
  result &= TEST_C11_SHMEM_ALLTOALLS(int64_t, 2, 2, 2);

  /* Test with floating point types using different strides */
  result &= TEST_C11_SHMEM_ALLTOALLS(double, 2, 1, 2);

  shmem_barrier_all();
  reduce_test_result("C11 shmem_alltoalls", &result, false);

  bool passed = result;
  log_close(!passed);
  shmem_finalize();
  return passed ? EXIT_SUCCESS : EXIT_FAILURE;
}
