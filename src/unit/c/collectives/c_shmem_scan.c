/**
 * @file c_shmem_scan.c
 *
 * @brief Unit test for shmem_scan().
 */
#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"
#include "type_tables.h"

#define TEST_C_SHMEM_SUM_INSCAN(TYPE, TYPENAME, NELEMS)                        \
  ({                                                                           \
    log_routine("shmem_scan(" #TYPE ")");                                      \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(sizeof(TYPE) * NELEMS);                   \
    TYPE *dest = (TYPE *)shmem_malloc(sizeof(TYPE) * NELEMS);                  \
    log_info("shmem_malloc'd %d bytes @ &src = %p, %d bytes @ &dest = %p",     \
             npes * sizeof(TYPE), (void *)src, npes * sizeof(TYPE),            \
             (void *)dest);                                                    \
                                                                               \
    for (int i = 0; i < NELEMS; i ++){                                         \
      src[i] = i + mype;                                                       \
    }                                                                          \
    log_info("set %p..%p to i + mype", (void *)src,                            \
            (void *)&src[npes*NELEMS - 1]);                                    \
                                                                               \
    log_info("executing shmem_sum_inscan: dest = %p, src = %p", (void *)dest,  \
             (void *)src);                                                     \
    shmem_##TYPENAME##_sum_inscan(SHMEM_TEAM_WORLD, dest, src, NELEMS);        \
    log_info("validating result...");                                          \
    bool success = true;                                                       \
    for (int i = 0; i < NELEMS; i++) {                                         \
      /* reproduce expected result */                                          \
      TYPE expected = (mype * (mype + 1 ) / 2) + i * (mype + 1);               \
      if (dest[i] != expected){                                                \
        success = false;                                                       \
        log_fail("Expected %d at index %d, got %d instead",(int) expected,     \
            i, (int) dest[i]);                                                 \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
                                                                               \
    if (success)                                                               \
      log_info("shmem_sum_inscan on " #TYPE " produced expected result.");     \
    else                                                                       \
      log_fail(                                                                \
          "at least one value was unexpected in result of shmem_sum_inscan");  \
    shmem_barrier_all();                                                       \
    shmem_free(src);                                                           \
    shmem_free(dest);                                                          \
                                                                               \
    success;                                                                   \
  })

#define TEST_C_SHMEM_SUM_EXSCAN(TYPE, TYPENAME, NELEMS)                        \
  ({                                                                           \
    log_routine("shmem_scan(" #TYPE ")");                                      \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(sizeof(TYPE) * NELEMS);                   \
    TYPE *dest = (TYPE *)shmem_malloc(sizeof(TYPE) * NELEMS);                  \
    log_info("shmem_malloc'd %d bytes @ &src = %p, %d bytes @ &dest = %p",     \
             npes * sizeof(TYPE), (void *)src, npes * sizeof(TYPE),            \
             (void *)dest);                                                    \
                                                                               \
    for (int i = 0; i < NELEMS; i ++){                                         \
      src[i] = i + mype;                                                       \
    }                                                                          \
    log_info("set %p..%p to i + mype", (void *)src,                            \
            (void *)&src[npes*NELEMS - 1]);                                    \
                                                                               \
    log_info("executing shmem_sum_inscan: dest = %p, src = %p", (void *)dest,  \
             (void *)src);                                                     \
    shmem_##TYPENAME##_sum_exscan(SHMEM_TEAM_WORLD, dest, src, NELEMS);        \
    log_info("validating result...");                                          \
    bool success = true;                                                       \
    for (int i = 0; i < NELEMS; i++) {                                         \
      /* reproduce expected result */                                          \
      if (mype != 0)                                                           \
        TYPE expected = (mype * (mype + 1 ) / 2) + i * (mype + 1);             \
      else                                                                     \
        TYPE expected = 0;                                                     \
      if (dest[i] != expected){                                                \
        success = false;                                                       \
        log_fail("Expected %d at index %d, got %d instead",(int) expected,     \
            i, (int) dest[i]);                                                 \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
                                                                               \
    if (success)                                                               \
      log_info("shmem_sum_exscan on " #TYPE " produced expected result.");     \
    else                                                                       \
      log_fail(                                                                \
          "at least one value was unexpected in result of shmem_sum_exscan");  \
    shmem_barrier_all();                                                       \
    shmem_free(src);                                                           \
    shmem_free(dest);                                                          \
                                                                               \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  if (!(shmem_n_pes() >= 2)) {
    log_warn("Not enough PEs to run test (requires 2 PEs, have %d PEs)", 
        shmem_n_pes()); 
    if (shmem_my_pe() == 0) { 
      display_not_enough_pes("collectives");
    }
    shmem_finalize();
    return EXIT_SUCCESS;
  }

  static bool result = true;
  #define X(type, shmem_types)                           \
    result &= TEST_C_SHMEM_SUM_INSCAN(type, shmem_types, 4);
    SHMEM_REDUCE_ARITH_TYPE_TABLE(X)
  #undef X

  shmem_barrier_all();
  reduce_test_result("C shmem_scan", &result, false);


  bool passed = result;
  log_close(!passed);
  shmem_finalize();
  return passed ? EXIT_SUCCESS : EXIT_FAILURE;
}
