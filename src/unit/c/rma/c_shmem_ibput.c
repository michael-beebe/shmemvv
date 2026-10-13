/**
 * @file c_shmem_ibput.c
 * @brief Typed and sized interleaved block transfers, default and explicit
 * contexts. Also built with API_USE_GENERICS and API_USE_PSHMEM.
 */
#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "shmemvv.h"
#include "type_tables.h"

#ifdef API_USE_GENERICS
#define TRANSFER(TYPE, NAME, D, S, DS, SS, B, N, PE)                           \
  shmem_ibput(D, S, DS, SS, B, N, PE)
#define CTX_TRANSFER(TYPE, NAME, CTX, D, S, DS, SS, B, N, PE)                  \
  shmem_ibput(CTX, D, S, DS, SS, B, N, PE)
#elif defined(API_USE_PSHMEM)
#include <pshmem.h>
#define TRANSFER(TYPE, NAME, D, S, DS, SS, B, N, PE)                           \
  pshmem_##NAME##_ibput(D, S, DS, SS, B, N, PE)
#define CTX_TRANSFER(TYPE, NAME, CTX, D, S, DS, SS, B, N, PE)                  \
  pshmem_ctx_##NAME##_ibput(CTX, D, S, DS, SS, B, N, PE)
#else
#define TRANSFER(TYPE, NAME, D, S, DS, SS, B, N, PE)                           \
  shmem_##NAME##_ibput(D, S, DS, SS, B, N, PE)
#define CTX_TRANSFER(TYPE, NAME, CTX, D, S, DS, SS, B, N, PE)                  \
  shmem_ctx_##NAME##_ibput(CTX, D, S, DS, SS, B, N, PE)
#endif
#ifdef API_USE_PSHMEM
#define SIZE_TRANSFER(SIZE, D, S, DS, SS, B, N, PE)                            \
  pshmem_ibput##SIZE(D, S, DS, SS, B, N, PE)
#define CTX_SIZE_TRANSFER(SIZE, CTX, D, S, DS, SS, B, N, PE)                   \
  pshmem_ctx_ibput##SIZE(CTX, D, S, DS, SS, B, N, PE)
#else
#define SIZE_TRANSFER(SIZE, D, S, DS, SS, B, N, PE)                            \
  shmem_ibput##SIZE(D, S, DS, SS, B, N, PE)
#define CTX_SIZE_TRANSFER(SIZE, CTX, D, S, DS, SS, B, N, PE)                   \
  shmem_ctx_ibput##SIZE(CTX, D, S, DS, SS, B, N, PE)
#endif
#define IB_IS_GET 0
#include "ib_rma_test.h"

int main(void) {
  shmem_init();
  log_init(__FILE__);
  if (shmem_n_pes() < 2) {
    display_not_enough_pes("RMA");
    log_close(EXIT_FAILURE);
    shmem_finalize();
    return EXIT_FAILURE;
  }
  shmem_ctx_t ctx;
  if (shmem_ctx_create(0, &ctx) != 0) {
    log_fail("Context creation failed");
    shmem_global_exit(EXIT_FAILURE);
  }
  static bool result;
  int rc = EXIT_SUCCESS;
  for (int explicit_ctx = 0; explicit_ctx < 2; ++explicit_ctx) {
    result = true;
#define TEST_TYPE(TYPE, NAME)                                                  \
  result &= TEST_C_SHMEM_IB_TRANSFER(TYPE, NAME, ctx, explicit_ctx);
    SHMEM_STANDARD_RMA_TYPE_TABLE(TEST_TYPE)
#undef TEST_TYPE
    shmem_barrier_all();
    reduce_test_result(explicit_ctx ? "shmem_ctx_ibput typed"
                                    : "shmem_ibput typed",
                       &result, false);
    shmem_barrier_all();
    if (!result)
      rc = EXIT_FAILURE;
#ifndef API_USE_GENERICS
    result = true;
#define TEST_SIZE(SIZE) result &= TEST_C_SHMEM_IB_SIZE(SIZE, ctx, explicit_ctx)
    TEST_SIZE(8);
    TEST_SIZE(16);
    TEST_SIZE(32);
    TEST_SIZE(64);
    TEST_SIZE(128);
#undef TEST_SIZE
    shmem_barrier_all();
    reduce_test_result(explicit_ctx ? "shmem_ctx_ibput<size>"
                                    : "shmem_ibput<size>",
                       &result, false);
    shmem_barrier_all();
    if (!result)
      rc = EXIT_FAILURE;
#endif
  }
  shmem_ctx_destroy(ctx);
  log_close(rc);
  shmem_finalize();
  return rc;
}
