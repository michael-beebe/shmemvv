/**
 * @file c_shmem_pe_quiet.c
 * @brief PE quiet completes RMA, AMO and signaling on the selected context.
 */
#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

static int source, dest[2], atomic_word;
static uint64_t signals[2];
static bool result;

static bool test_shmem_pe_quiet(shmem_ctx_t ctx, bool explicit_ctx,
                                bool session, int list_case) {
  log_routine(explicit_ctx ? "shmem_ctx_pe_quiet" : "shmem_pe_quiet");
  int me = shmem_my_pe(), npes = shmem_n_pes();
  int next = (me + 1) % npes, previous = (me + npes - 1) % npes;
  const int targets[3] = {next, next, me};
  const size_t counts[3] = {1, 2, 3};
  log_info("Explicit context %d, session %d, list case %d", explicit_ctx,
           session, list_case);
  source = me + 11;
  dest[0] = dest[1] = atomic_word = 0;
  signals[0] = signals[1] = 0;
  shmem_barrier_all();
#ifdef SHMEMVV_HAVE_SESSIONS
  if (session) {
    const shmem_ctx_session_config_t config = {.total_ops = 2};
    shmem_ctx_session_start(ctx, SHMEM_CTX_SESSION_BATCH, &config,
                            SHMEM_CTX_SESSION_TOTAL_OPS);
  }
#endif
  int value = me + 101, signal_value = me + 102;
  int fetched = -1;
  if (explicit_ctx) {
    if (session)
      shmem_ctx_int_put(ctx, dest, &value, 1, next);
    else
      shmem_ctx_int_put_nbi(ctx, dest, &value, 1, next);
    shmem_ctx_int_get_nbi(ctx, &fetched, &source, 1, next);
    shmem_ctx_int_atomic_inc(ctx, &atomic_word, next);
    shmem_ctx_signal_set(ctx, &signals[0], 17, next);
    shmem_ctx_int_put_signal_nbi(ctx, &dest[1], &signal_value, 1, &signals[1],
                                 19, SHMEM_SIGNAL_SET, next);
    /* NULL is ignored for an empty list even with outstanding operations. */
    shmem_ctx_pe_quiet(ctx, NULL, 0);
    shmem_ctx_pe_quiet(ctx, targets, 0);
    shmem_ctx_pe_quiet(ctx, targets, counts[list_case]);
  } else {
    if (session)
      shmem_int_put(dest, &value, 1, next);
    else
      shmem_int_put_nbi(dest, &value, 1, next);
    shmem_int_get_nbi(&fetched, &source, 1, next);
    shmem_int_atomic_inc(&atomic_word, next);
    shmem_signal_set(&signals[0], 17, next);
    shmem_int_put_signal_nbi(&dest[1], &signal_value, 1, &signals[1], 19,
                             SHMEM_SIGNAL_SET, next);
    shmem_pe_quiet(NULL, 0);
    shmem_pe_quiet(targets, 0);
    shmem_pe_quiet(targets, counts[list_case]);
  }
  /* Check GET completion and reuse PUT sources before any collective. */
  bool passed = fetched == next + 11;
  if (!passed)
    log_fail("Nonblocking GET incomplete: expected %d, got %d", next + 11,
             fetched);
  value = signal_value = -1;
  /* sync_all synchronizes PEs without performing default-context quiet. */
  shmem_sync_all();
  if (dest[0] != previous + 101 || dest[1] != previous + 102 ||
      atomic_word != 1 || shmem_signal_fetch(&signals[0]) != 17 ||
      shmem_signal_fetch(&signals[1]) != 19) {
    log_fail("Selected-PE completion failed: destinations %d/%d, atomic %d, "
             "signals %llu/%llu",
             dest[0], dest[1], atomic_word,
             (unsigned long long)shmem_signal_fetch(&signals[0]),
             (unsigned long long)shmem_signal_fetch(&signals[1]));
    passed = false;
  }
#ifdef SHMEMVV_HAVE_SESSIONS
  if (session)
    shmem_ctx_session_stop(ctx);
#endif
  shmem_barrier_all();
  return passed;
}

int main(void) {
  shmem_init();
  log_init(__FILE__);
  if (shmem_n_pes() < 2) {
    display_not_enough_pes("MEMORY");
    log_close(EXIT_FAILURE);
    shmem_finalize();
    return EXIT_FAILURE;
  }
  int rc = EXIT_SUCCESS;
  for (int mode = 0; mode < 3; ++mode) {
    shmem_ctx_t ctx = SHMEM_CTX_DEFAULT;
    bool explicit_ctx = mode != 0;
    if (explicit_ctx &&
        shmem_ctx_create(mode == 2 ? SHMEM_CTX_NOSTORE : 0, &ctx) != 0) {
      log_fail("Context creation failed");
      shmem_global_exit(EXIT_FAILURE);
    }
    result = true;
    for (int list_case = 0; list_case < 3; ++list_case)
      result &= test_shmem_pe_quiet(ctx, explicit_ctx, false, list_case);
#ifdef SHMEMVV_HAVE_SESSIONS
    result &= test_shmem_pe_quiet(ctx, explicit_ctx, true, 0);
#endif
    shmem_barrier_all();
    const char *name = mode == 0   ? "C shmem_pe_quiet"
                       : mode == 1 ? "C shmem_ctx_pe_quiet"
                                   : "C shmem_ctx_pe_quiet NOSTORE";
    reduce_test_result(name, &result, false);
    shmem_barrier_all();
    if (!result)
      rc = EXIT_FAILURE;
    if (explicit_ctx)
      shmem_ctx_destroy(ctx);
  }
  log_close(rc);
  shmem_finalize();
  return rc;
}
