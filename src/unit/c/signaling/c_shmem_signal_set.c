/**
 * @file c_shmem_signal_set.c
 * @brief Standalone signal replacement, including explicit-context dispatch.
 */
#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

static bool test_shmem_signal_set(shmem_ctx_t ctx, bool explicit_ctx,
                                  uint64_t *signals) {
  log_routine("shmem_signal_set / shmem_ctx_signal_set");
  int me = shmem_my_pe(), npes = shmem_n_pes();
  int next = (me + 1) % npes, previous = (me + npes - 1) % npes;
  const uint64_t values[] = {UINT64_MAX, 0, UINT64_C(0x8000000000000011)};
  bool passed = true;
  for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
    for (int pe = 0; pe < npes; ++pe)
      signals[pe] = 7;
    shmem_barrier_all();
    if (explicit_ctx) {
#ifdef API_USE_GENERICS
      shmem_signal_set(ctx, &signals[me], values[i], next);
#else
      shmem_ctx_signal_set(ctx, &signals[me], values[i], next);
#endif
      shmem_ctx_quiet(ctx);
    } else {
      shmem_signal_set(&signals[me], values[i], next);
      shmem_quiet();
    }
    shmem_sync_all();
    for (int pe = 0; pe < npes; ++pe) {
      uint64_t expected = pe == previous ? values[i] : 7;
      uint64_t actual = shmem_signal_fetch(&signals[pe]);
      if (actual != expected) {
        log_fail("Signal %d: expected %llu, got %llu", pe,
                 (unsigned long long)expected, (unsigned long long)actual);
        passed = false;
      }
    }
    shmem_barrier_all();
  }
  return passed;
}

int main(void) {
  shmem_init();
  log_init(__FILE__);
  uint64_t *signals = shmem_malloc(shmem_n_pes() * sizeof(*signals));
  shmem_ctx_t ctx;
  if (signals == NULL || shmem_ctx_create(0, &ctx) != 0) {
    log_fail("Allocation or context creation failed");
    shmem_global_exit(EXIT_FAILURE);
  }
  static bool result;
  int rc = EXIT_SUCCESS;
  for (int explicit_ctx = 0; explicit_ctx < 2; ++explicit_ctx) {
    result = test_shmem_signal_set(ctx, explicit_ctx, signals);
    shmem_barrier_all();
    reduce_test_result(explicit_ctx ? "shmem_ctx_signal_set"
                                    : "shmem_signal_set",
                       &result, false);
    shmem_barrier_all();
    if (!result)
      rc = EXIT_FAILURE;
  }
  shmem_ctx_destroy(ctx);
  shmem_free(signals);
  log_close(rc);
  shmem_finalize();
  return rc;
}
