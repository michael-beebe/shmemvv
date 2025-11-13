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
#include "type_tables.h"

#define TEST_C11_SHMEM_ALLTOALL(TYPE, NELEMS)                                  \
  ({                                                                           \
    log_routine("shmem_alltoall(" #TYPE ")");                                  \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(npes * sizeof(TYPE) * NELEMS);            \
    TYPE *dest = (TYPE *)shmem_malloc(npes * sizeof(TYPE) * NELEMS);           \
    log_info("shmem_malloc'd %d bytes @ &src = %p, %d bytes @ &dest = %p",     \
             npes * sizeof(TYPE), (void *)src, npes * sizeof(TYPE),            \
             (void *)dest);                                                    \
                                                                               \
    for (int pe = 0; pe < npes; ++pe) {                                        \
      for (int j = 0; j < NELEMS; ++j){                                        \
        /* create a recognizeable, 8 bit safe pattern*/                        \
        src[pe * NELEMS + j] = (TYPE)((mype << 4) ^ (pe << 2) ^ j);            \
      }                                                                        \
    }                                                                          \
    log_info("set %p..%p to (mype << 4) ^ (pe << 2) ^ j", (void *)src,         \
            (void *)&src[npes*NELEMS - 1]);                                    \
                                                                               \
    log_info("executing shmem_alltoall: dest = %p, src = %p", (void *)dest,    \
             (void *)src);                                                     \
    shmem_alltoall(SHMEM_TEAM_WORLD, dest, src, NELEMS);                       \
                                                                               \
    log_info("validating result...");                                          \
    bool success = true;                                                       \
    for (int pe = 0; pe < npes; ++pe) {                                        \
      for (int j = 0; j < NELEMS; ++j){                                        \
        /*reproduce pattern*/                                                  \
        TYPE expected = (TYPE)((pe << 4) ^ (mype << 2) ^ j);                   \
        if (dest[pe * NELEMS + j] != expected){                                \
          log_info("index %d of dest (%p) failed. expected %d, got %d",        \
            pe * NELEMS + j, &dest[pe * NELEMS + j], (int)expected,            \
            (int)dest[pe * NELEMS + j]);                                       \
          success = false;                                                     \
          break;                                                               \
        }                                                                      \
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


#define TEST_C11_SHMEM_ALLTOALLMEM(NELEMS)                                     \
  ({                                                                           \
    log_routine("shmem_alltoallmem()");                                        \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
    void *src = (void *)shmem_calloc(npes * NELEMS, 1);                        \
    void *dest = (void *)shmem_calloc(npes * NELEMS, 1);                       \
    log_info("shmem_calloc'd %d bytes @ &src = %p, %d bytes @ &dest = %p",     \
             npes * NELEMS, (void *)src, npes * NELEMS,                        \
             (void *)dest);                                                    \
                                                                               \
    unsigned char * src_ptr = (unsigned char *) src;                           \
    for (int pe = 0; pe < npes; ++pe) {                                        \
      for (int j = 0; j < NELEMS; ++j){                                        \
        /* create a recognizeable, 8 bit safe pattern*/                        \
        unsigned char val = (((mype << 4) ^ (pe << 2) ^ j) % 256);             \
        memset(&src_ptr[pe * NELEMS + j], val, 1);                             \
      }                                                                        \
    }                                                                          \
    log_info("set %p..%p to (mype << 4) ^ (pe << 2) ^ j", (void *)src_ptr,     \
            (void *)(src_ptr + npes*NELEMS - 1));                              \
                                                                               \
    log_info("executing shmem_alltoallmem: dest = %p, src = %p", (void *)dest, \
             (void *)src);                                                     \
    shmem_alltoallmem(SHMEM_TEAM_WORLD, dest, src, NELEMS);                    \
                                                                               \
    unsigned char * dest_ptr = (unsigned char *) dest;                         \
    bool success = true;                                                       \
    log_info("validating result...");                                          \
    for (int pe = 0; pe < npes; ++pe) {                                        \
      for (int j = 0; j < NELEMS; ++j){                                        \
        /*reproduce pattern*/                                                  \
        unsigned char expected = (((pe << 4) ^ (mype << 2) ^ j) % 256);        \
        if (dest_ptr[pe * NELEMS + j] != expected){                            \
          log_info("index %d of dest (%p) failed. expected %d, got %d",        \
            pe * NELEMS + j, &dest_ptr[pe * NELEMS + j], (int)expected,        \
            (int)dest_ptr[pe * NELEMS + j]);                                   \
          success = false;                                                     \
          break;                                                               \
        }                                                                      \
      }                                                                        \
    }                                                                          \
                                                                               \
    if (success)                                                               \
      log_info("shmem_alltoallmem produced expected result.");                 \
    else                                                                       \
      log_fail(                                                                \
          "at least one value was unexpected in result of shmem_alltoallmem"); \
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
  #define X(type, shmem_types) result &= TEST_C11_SHMEM_ALLTOALL(type, 4);
    SHMEM_STANDARD_RMA_TYPE_TABLE(X)
  #undef X

  shmem_barrier_all();
  reduce_test_result("C11 shmem_alltoall", &result, false);

  static bool result_mem = true;
  result_mem &= TEST_C11_SHMEM_ALLTOALLMEM(4);

  shmem_barrier_all();  
  reduce_test_result("C11 shmem_alltoallmem", &result_mem, false);

  bool passed = result & result_mem;
  log_close(!passed);
  shmem_finalize();
  return passed ? EXIT_SUCCESS : EXIT_FAILURE;
}
