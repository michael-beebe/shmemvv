/**
 * @file c11_shmem_atomic_and.c
 * @brief Unit test for shmem_atomic_and
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"
#include "type_tables.h"

#define TEST_C11_SHMEM_ATOMIC_AND(TYPE)                                        \
  ({                                                                           \
    log_routine("shmem_atomic_and(" #TYPE ")");                                \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    TYPE value = 0xFF;                                                         \
    TYPE and_value = shmem_my_pe();                                            \
    *dest = value;                                                             \
    log_info("initialized dest at %p to %d", (void *)dest, (int)value);        \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    log_info("executing atomic and: dest = %p, value = %d", (void *)dest,      \
             (int)and_value);                                                  \
    shmem_atomic_and(dest, and_value, (mype + 1) % npes);                      \
    shmem_barrier_all();                                                       \
    TYPE prev_pe = (mype + npes - 1) % npes; /*find prev pe number*/           \
    success = (*dest == (value & prev_pe));                                    \
    if (!success)                                                              \
      log_fail("atomic and on %s did not produce expected value %d, "          \
               "got instead %d",                                               \
               #TYPE, (int)(value & prev_pe), (int)*dest);                     \
    else                                                                       \
      log_info("atomic and on a %s at %p produced expected result "            \
               "(%d & %d = %d)",                                               \
               #TYPE, (void *)dest, (int)value, (int)prev_pe, (int)*dest);     \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

#define TEST_C11_CTX_SHMEM_ATOMIC_AND(TYPE)                                    \
  ({                                                                           \
    log_routine("shmem_atomic_and(ctx, " #TYPE ")");                           \
    bool success = true;                                                       \
    static TYPE *dest;                                                         \
    dest = (TYPE *)shmem_malloc(sizeof(TYPE));                                 \
    log_info("shmem_malloc'd %d bytes at %p", sizeof(TYPE), (void *)dest);     \
    TYPE value = 0xFF;                                                         \
    TYPE and_value = shmem_my_pe();                                            \
    *dest = value;                                                             \
    log_info("initialized dest at %p to %d", (void *)dest, (int)value);        \
                                                                               \
    shmem_ctx_t ctx;                                                           \
    int ctx_create_status = shmem_ctx_create(0, &ctx);                         \
    if (ctx_create_status != 0) {                                              \
      log_fail("Failed to create context");                                    \
      shmem_free(dest);                                                        \
      return false;                                                            \
    }                                                                          \
    log_info("Successfully created context");                                  \
                                                                               \
    shmem_barrier_all();                                                       \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    log_info("executing atomic and with context: dest = %p, value = %d",       \
             (void *)dest, (int)and_value);                                    \
    shmem_atomic_and(ctx, dest, and_value, (mype + 1) % npes);                 \
    shmem_ctx_quiet(ctx);                                                      \
    shmem_barrier_all();                                                       \
    TYPE prev_pe = (mype + npes - 1) % npes; /*find prev pe number*/           \
    success = (*dest == (value & prev_pe));                                    \
    if (!success)                                                              \
      log_fail("atomic and with context on %s did not produce expected "       \
               "value %d, got instead %d",                                     \
               #TYPE, (int)(value & prev_pe), (int)*dest);                     \
    else                                                                       \
      log_info("atomic and with context on a %s at %p produced expected "      \
               "result (%d & %d = %d)",                                        \
               #TYPE, (void *)dest, (int)value, (int)prev_pe, (int)*dest);     \
                                                                               \
    shmem_ctx_destroy(ctx);                                                    \
    log_info("Context destroyed");                                             \
    shmem_free(dest);                                                          \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  if (!(shmem_n_pes() >= 2)) {
    log_warn("Not enough PEs to run test (requires 2 PEs, have %d PEs)",
             shmem_n_pes());
    if (shmem_my_pe() == 0) {
      display_not_enough_pes("atomic");
    }
    shmem_finalize();
    return EXIT_SUCCESS;
  }

  static int result = true;
  static int result_ctx = true;

  /* Test standard atomic and operations */
  #define X(type, shmem_types) result &= TEST_C11_SHMEM_ATOMIC_AND(type);
    SHMEM_BITWISE_AMO_TYPE_TABLE(X)
  #undef X

  shmem_barrier_all();

  reduce_test_result("C11 shmem_atomic_and", &result, false);


  /* Test context-specific atomic and operations */
  #define X(type, shmem_types) result_ctx &= TEST_C11_CTX_SHMEM_ATOMIC_AND(type);
    SHMEM_BITWISE_AMO_TYPE_TABLE(X)
  #undef X

  shmem_barrier_all();

  reduce_test_result("C11 shmem_atomic_and with ctx", &result_ctx, false);

  bool rc = result & result_ctx ? EXIT_SUCCESS : EXIT_FAILURE;
  log_close(rc);
  shmem_finalize();
  return rc;
}
