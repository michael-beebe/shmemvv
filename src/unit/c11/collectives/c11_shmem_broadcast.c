/**
 * @file c11_shmem_broadcast.c
 * @brief Unit test for shmem_broadcast().
 */

#include "log.h"
#include "shmemvv.h"
#include "type_tables.h"

#define TEST_C11_SHMEM_BROADCAST(TYPE, NELEMS)                                 \
  ({                                                                           \
    log_routine("shmem_broadcast(" #TYPE ")");                                 \
    log_info("shmem_collect() parameter nelems set to %d.", NELEMS);           \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_calloc(NELEMS, sizeof(TYPE));                    \
    TYPE *dest = (TYPE *)shmem_calloc(NELEMS, sizeof(TYPE));                   \
    log_info("shmem_malloc'd %d bytes @ &src = %p, %d bytes @ &dest = %p",     \
      NELEMS * sizeof(TYPE), (void *)src, NELEMS * sizeof(TYPE), (void *)dest);\
    if (!src || !dest) {                                                       \
      log_fail("Failed to allocate symmetric memory");                         \
      if (src)                                                                 \
        shmem_free(src);                                                       \
      if (dest)                                                                \
        shmem_free(dest);                                                      \
      return false;                                                            \
    }                                                                          \
                                                                               \
    /* Only root PE sets the actual source data */                             \
    if (mype == 0) {                                                           \
      for (int i = 0; i < NELEMS; ++i) {                                       \
        /* Typecast handles wraparound/overflow */                             \
        src[i] = (TYPE)(i + 1);                                                \
      }                                                                        \
      log_info("set %p..%p to idx + 1", (void *)src, (void *)(src + NELEMS));  \
    }                                                                          \
                                                                               \
    for (int i = 0; i < NELEMS; ++i) {                                         \
      dest[i] = -1;                                                            \
    }                                                                          \
    log_info("set %p..%p to -1", (void *)dest, (void *)(dest + NELEMS));       \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    log_info("executing shmem_broadcast: dest = %p, src = %p", (void *)dest,   \
             (void *)src);                                                     \
    shmem_broadcast(SHMEM_TEAM_WORLD, dest, src, NELEMS, 0);                   \
    /*dest data object guarenteed to be updated after collective routine */    \
                                                                               \
    log_info("validating result...");                                          \
    bool success = true;                                                       \
    for (int i = 0; i < NELEMS; ++i) {                                         \
      /* Typecast handles wraparound/overflow */                               \
      TYPE expected = (TYPE)(i + 1);                                           \
      if (dest[i] != expected) {                                               \
        log_info("index %d of dest (%p) failed. expected %d, got %d", i,       \
                 &dest[i], (int)expected, (int)dest[i]);                       \
        success = false;                                                       \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
                                                                               \
    if (success)                                                               \
      log_info("shmem_broadcast on " #TYPE " produced expected result.");      \
    else                                                                       \
      log_fail(                                                                \
          "at least one value was unexpected in result of shmem_broadcast");   \
    shmem_free(src);                                                           \
    shmem_free(dest);                                                          \
                                                                               \
    /* Ensure memory cleanup is complete across all PEs */                     \
    shmem_barrier_all();                                                       \
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
  #define X(type, shmem_types) result &= TEST_C11_SHMEM_BROADCAST(type, 4);
    SHMEM_STANDARD_RMA_TYPE_TABLE(X)
  #undef X

  /* Test various broadcast sizes with int */
  result &= TEST_C11_SHMEM_BROADCAST(int, 1);
  result &= TEST_C11_SHMEM_BROADCAST(int, 2);
  result &= TEST_C11_SHMEM_BROADCAST(int, 7);
  result &= TEST_C11_SHMEM_BROADCAST(int, 16);
  result &= TEST_C11_SHMEM_BROADCAST(int, 1024); 
  result &= TEST_C11_SHMEM_BROADCAST(int, 16384); 

  shmem_barrier_all();
  reduce_test_result("C11 shmem_broadcast", &result, false);

  bool passed = result;
  log_close(!passed);
  shmem_finalize();
  return passed ? EXIT_SUCCESS : EXIT_FAILURE;
}
