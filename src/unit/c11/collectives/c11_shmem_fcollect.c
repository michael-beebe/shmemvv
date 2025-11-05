/**
 * @file c11_shmem_fcollect.c
 * @brief Unit test for shmem_fcollect().
 */

#include "log.h"
#include "shmemvv.h"
#include "type_tables.h"

#define TEST_C11_SHMEM_FCOLLECT(TYPE, NELEMS)                                  \
  ({                                                                           \
    log_routine("shmem_fcollect(" #TYPE ")");                                  \
    log_info("shmem_collect() parameter nelems set to %d.", NELEMS);           \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_calloc(NELEMS, sizeof(TYPE));                    \
    TYPE *dest = (TYPE *)shmem_calloc(npes * NELEMS, sizeof(TYPE));            \
    log_info("shmem_malloc'd %d bytes @ &src = %p, %d bytes @ &dest = %p",     \
             sizeof(TYPE) * NELEMS, (void *)src, npes * sizeof(TYPE) * NELEMS, \
             (void *)dest);                                                    \
                                                                               \
    if (!src || !dest) {                                                       \
      log_fail("Failed to allocate symmetric memory");                         \
      if (src)                                                                 \
        shmem_free(src);                                                       \
      if (dest)                                                                \
        shmem_free(dest);                                                      \
      return false;                                                            \
    }                                                                          \
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
                                                                               \
    log_info("executing shmem_fcollect: dest = %p, src = %p", (void *)dest,    \
             (void *)src);                                                     \
    shmem_fcollect(SHMEM_TEAM_WORLD, dest, src, NELEMS);                       \
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
      log_info("shmem_fcollect on " #TYPE " produced expected result.");       \
    else                                                                       \
      log_fail(                                                                \
          "at least one value was unexpected in result of shmem_fcollect");    \
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

  static bool result = true;
  /*test multiple nelem values*/
  #define X(type, shmem_types) result &= TEST_C11_SHMEM_FCOLLECT(type, 1); \
                               result &= TEST_C11_SHMEM_FCOLLECT(type, 4); \
                               result &= TEST_C11_SHMEM_FCOLLECT(type, 7);
    SHMEM_STANDARD_RMA_TYPE_TABLE(X)
  #undef X

  shmem_barrier_all();

  reduce_test_result("C11 shmem_fcollect", &result, false);

  bool passed = result;
  log_close(!passed);
  shmem_finalize();
  return passed ? EXIT_SUCCESS : EXIT_FAILURE;
}
