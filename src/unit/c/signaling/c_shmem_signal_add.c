/**
 * @file c_shmem_signal_add.c
 * @brief Concurrent signal additions and unsigned wraparound.
 */
#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

static uint64_t signal_word;
static bool result;

static bool test_shmem_signal_add(shmem_ctx_t ctx, bool explicit_ctx) {
  log_routine("shmem_signal_add / shmem_ctx_signal_add");
  const uint64_t initial[] = {0, UINT64_MAX - 5};
  bool passed = true;
  for (size_t i = 0; i < sizeof(initial) / sizeof(initial[0]); ++i) {
    signal_word = initial[i];
    shmem_barrier_all();
    /* Every origin concurrently adds to PE 0. Lost updates fail the count. */
    for (int n = 0; n < 17; ++n) {
      if (explicit_ctx) {
#ifdef API_USE_GENERICS
        shmem_signal_add(ctx, &signal_word, 3, 0);
#else
        shmem_ctx_signal_add(ctx, &signal_word, 3, 0);
#endif
      } else {
        shmem_signal_add(&signal_word, 3, 0);
      }
    }
    if (explicit_ctx)
      shmem_ctx_quiet(ctx);
    else
      shmem_quiet();
    shmem_sync_all();
    uint64_t expected = initial[i];
    if (shmem_my_pe() == 0)
      expected += UINT64_C(51) * (uint64_t)shmem_n_pes();
    uint64_t actual = shmem_signal_fetch(&signal_word);
    if (actual != expected) {
      log_fail("Signal sum: expected %llu, got %llu",
               (unsigned long long)expected, (unsigned long long)actual);
      passed = false;
    }
    shmem_barrier_all();
  }
  return passed;
}

int main(void) {
  shmem_init();
  log_init(__FILE__);
  shmem_ctx_t ctx;
  if (shmem_ctx_create(0, &ctx) != 0) {
    log_fail("Context creation failed");
    shmem_global_exit(EXIT_FAILURE);
  }
  int rc = EXIT_SUCCESS;
  for (int explicit_ctx = 0; explicit_ctx < 2; ++explicit_ctx) {
    result = test_shmem_signal_add(ctx, explicit_ctx);
    shmem_barrier_all();
    reduce_test_result(explicit_ctx ? "shmem_ctx_signal_add"
                                    : "shmem_signal_add",
                       &result, false);
    shmem_barrier_all();
    if (!result)
      rc = EXIT_FAILURE;
  }
  shmem_ctx_destroy(ctx);
  log_close(rc);
  shmem_finalize();
  return rc;
}
