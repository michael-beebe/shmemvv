/**
 * @file c11_shmem_collect.c
 * @brief Unit test for shmem_collect().
 */

#include "log.h"
#include "shmemvv.h"
#include "type_tables.h"

#define TEST_C11_SHMEM_COLLECT(TYPE, NELEMS)                                   \
  ({                                                                           \
    log_routine("shmem_collect(" #TYPE ")");                                   \
    log_info("shmem_collect() parameter nelems set to %d.", NELEMS);           \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(sizeof(TYPE) * NELEMS);                   \
    TYPE *dest = (TYPE *)shmem_malloc(npes * sizeof(TYPE) * NELEMS);           \
    log_info("shmem_malloc'd %d bytes @ &src = %p, %d bytes @ &dest = %p",     \
             sizeof(TYPE) * NELEMS, (void *)src, npes * sizeof(TYPE) * NELEMS, \
             (void *)dest);                                                    \
                                                                               \
    /*create unique pattern for each element*/                                 \
    for (int elem = 0; elem < NELEMS; elem ++){                                \
      /* Cast handles overflow/wraparound */                                   \
      src[elem] = (TYPE)((mype << 4) ^ (elem));                                \
    }                                                                          \
    log_info("set %p...%p to src[elem] = (%d << 4) ^ elem.", (void *)src,      \
            (void*) (src + NELEMS - 1), (int)mype);                            \
                                                                               \
    shmem_barrier_all(); /* Ensure all PEs are ready */                        \
    log_info("executing shmem_collect: dest = %p, src = %p", (void *)dest,     \
             (void *)src);                                                     \
    shmem_collect(SHMEM_TEAM_WORLD, dest, src, NELEMS);                        \
                                                                               \
    log_info("validating result...");                                          \
    bool success = true;                                                       \
    for (int i = 0; i < npes; ++i) {                                           \
      for (int elem = 0; elem < NELEMS; elem ++){                              \
        TYPE expected = ((i << 4) ^ (elem));                                   \
        if (dest[i*NELEMS + elem] != expected) {                               \
          log_fail("index %d of dest (%p) failed. expected %d, got %d",        \
            i*NELEMS + elem, &dest[i], (int)expected, (int)dest[i]);           \
          success = false;                                                     \
          break;                                                               \
        }                                                                      \
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


/*Unit test to ensure that varing values of nelems on different PEs works*/
#define TEST_C11_SHMEM_COLLECT_VAR_NELEMS(TYPE)                                \
  ({                                                                           \
    log_routine("shmem_collect(" #TYPE ") variable nelems");                   \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
    int my_nelem = mype + 1; /*linearly increasing number of elements with PE*/\
    int total_nelem = (npes * (npes + 1)) / 2;                                 \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(sizeof(TYPE) * npes);                     \
    TYPE *dest = (TYPE *)shmem_malloc(sizeof(TYPE) * total_nelem);             \
    log_info("shmem_malloc'd %d bytes @ &src = %p, %d3 bytes @ &dest = %p",    \
             my_nelem, (void *)src, total_nelem, (void *)dest);                \
                                                                               \
    for (int i = 0; i < my_nelem; i++){                                        \
      src[i] = (TYPE)(mype * (mype + 1)) / 2 + i;                              \
    }                                                                          \
    log_info("set %p...%p to %d + i", (void *)src,                             \
            (void*) (src + my_nelem - 1), (int)((mype * (mype + 1)) / 2));     \
                                                                               \
    shmem_team_sync(SHMEM_TEAM_WORLD);                                         \
    log_info("executing shmem_collect: dest = %p, src = %p", (void *)dest,     \
             (void *)src);                                                     \
    shmem_collect(SHMEM_TEAM_WORLD, dest, src, my_nelem);                      \
                                                                               \
    log_info("validating result...");                                          \
    bool success = true;                                                       \
    for (int i = 0; i < total_nelem; ++i) {                                    \
      TYPE expected = i;                                                       \
      if (dest[i] != expected){                                                \
        log_fail("index %d of dest (%p) failed. expected %d, got %d",          \
               i, (void *)dest, (int) expected, (int) dest[i]);                \
        success = false;                                                       \
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

  if (!(shmem_n_pes() >= 2)) {
    log_warn("Not enough PEs to run test (requires 2 PEs, have %d PEs)",
             shmem_n_pes());
    if (shmem_my_pe() == 0) {
      display_not_enough_pes("RMA");
    }
    shmem_finalize();
    return EXIT_SUCCESS;
  }

  static int result = true;
  #define X(type, shmem_types) result &= TEST_C11_SHMEM_COLLECT(type, 4);
    SHMEM_STANDARD_RMA_TYPE_TABLE(X)
  #undef X

  shmem_barrier_all();

  reduce_test_result("C11 shmem_collect", &result, false);

  static int result_var_nelems = true;
  #define X(type, shmem_types) result_var_nelems &= TEST_C11_SHMEM_COLLECT_VAR_NELEMS(type);
    SHMEM_STANDARD_RMA_TYPE_TABLE(X)
  #undef X

  shmem_barrier_all();
  reduce_test_result("C11 shmem_collect (variable nelems)", &result_var_nelems, false);

  bool passed = result & result_var_nelems;
  log_close(!passed);
  shmem_finalize();
  return passed ? EXIT_SUCCESS : EXIT_FAILURE;
}
