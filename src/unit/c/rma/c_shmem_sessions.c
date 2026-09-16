/**
 * @file c_shmem_sessions.c
 * @brief Session hints must preserve source lifetime, ordering and completion.
 *
 * Also built as C11 with SESSION_USE_GENERICS to exercise generic RMA dispatch.
 */
#include <shmem.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "shmemvv.h"

#ifdef SESSION_USE_GENERICS
#define PUT(ctx, dst, src, n, pe) shmem_put(ctx, dst, src, n, pe)
#define P(ctx, dst, value, pe) shmem_p(ctx, dst, value, pe)
#else
#define PUT(ctx, dst, src, n, pe) shmem_ctx_uint64_put(ctx, dst, src, n, pe)
#define P(ctx, dst, value, pe) shmem_ctx_uint64_p(ctx, dst, value, pe)
#endif

#define WORDS (1u << 18)
static uint64_t signal_word;
static int local_failure, global_failure;
static int any_failure;

static void report(const char *name, bool ok) {
  if (!ok)
    log_fail("%s on PE %d", name, shmem_my_pe());
  local_failure = !ok;
  shmem_int_max_reduce(SHMEM_TEAM_WORLD, &global_failure, &local_failure, 1);
  if (shmem_my_pe() == 0)
    display_test_result(name, global_failure == 0, false);
  any_failure |= global_failure;
}

static void reset(uint64_t *dst) {
  memset(dst, 0, WORDS * sizeof(*dst));
  signal_word = 0;
  shmem_barrier_all();
}

static void start(shmem_ctx_t ctx, size_t hint) {
  const shmem_ctx_session_config_t cfg = {.total_ops = hint};
  shmem_ctx_session_start(ctx, SHMEM_CTX_SESSION_BATCH, &cfg,
                          SHMEM_CTX_SESSION_TOTAL_OPS);
}

static void exercise(shmem_ctx_t ctx, uint64_t *dst, uint64_t *src,
                     size_t hint) {
  const int me = shmem_my_pe();
  const int next = (me + 1) % shmem_n_pes();
  const int previous = (me + shmem_n_pes() - 1) % shmem_n_pes();
  bool ok;

  reset(dst);
  start(ctx, hint);
  for (size_t i = 0; i < 2048; ++i) {
    uint64_t value = (uint64_t)me * 4096 + i + 1;
    PUT(ctx, &dst[i], &value, 1, next);
    value = UINT64_MAX;
  }
  P(ctx, &dst[4096], UINT64_C(123), next);
  P(ctx, &dst[4097], UINT64_C(456), me);
  shmem_ctx_quiet(ctx);
  shmem_barrier_all();
  ok = dst[4096] == 123 && dst[4097] == 456;
  for (size_t i = 0; i < 2048; ++i)
    ok &= dst[i] == (uint64_t)previous * 4096 + i + 1;
  report("sessions: source reuse, rollover, target switch and quiet", ok);

  P(ctx, &dst[5000], UINT64_C(71), next);
  shmem_ctx_session_start(ctx, 0, NULL, 0);
  start(ctx, SIZE_MAX);
  P(ctx, &dst[5001], UINT64_C(72), next);
  shmem_ctx_session_stop(ctx);
  shmem_ctx_session_stop(ctx);
  shmem_ctx_quiet(ctx);
  shmem_barrier_all();
  report("sessions: repeated start and stop",
         dst[5000] == 71 && dst[5001] == 72);

  reset(dst);
  for (size_t i = 0; i < WORDS; ++i)
    src[i] = (uint64_t)me + i + 9;
  start(ctx, hint);
  PUT(ctx, dst, src, WORDS, next);
  memset(src, 0xff, WORDS * sizeof(*src));
  shmem_ctx_quiet(ctx);
  shmem_ctx_session_stop(ctx);
  shmem_barrier_all();
  ok = true;
  for (size_t i = 0; i < WORDS; ++i)
    ok &= dst[i] == (uint64_t)previous + i + 9;
  report("sessions: large blocking source reuse", ok);

  reset(dst);
  start(ctx, hint);
  P(ctx, dst, UINT64_C(37), next);
  shmem_ctx_fence(ctx);
  shmem_ctx_uint64_atomic_set(ctx, &signal_word, 1, next);
  shmem_uint64_wait_until(&signal_word, SHMEM_CMP_EQ, 1);
  ok = dst[0] == 37;
  shmem_ctx_session_stop(ctx);
  shmem_ctx_quiet(ctx);
  report("sessions: fence before atomic notification", ok);

  reset(dst);
  start(ctx, hint);
  uint64_t value = 83;
  shmem_ctx_putmem_signal(ctx, dst, &value, sizeof(value), &signal_word, 1,
                          SHMEM_SIGNAL_SET, next);
  value = 0;
  shmem_signal_wait_until(&signal_word, SHMEM_CMP_EQ, 1);
  ok = dst[0] == 83;
  shmem_ctx_session_stop(ctx);
  shmem_ctx_quiet(ctx);
  report("sessions: put with signal", ok);

  reset(dst);
  start(ctx, hint);
  src[0] = 91;
  shmem_ctx_putmem_nbi(ctx, dst, src, sizeof(*src), next);
  shmem_ctx_quiet(ctx);
  shmem_barrier_all();
  src[0] = 0;
  shmem_ctx_getmem_nbi(ctx, src, dst, sizeof(*src), next);
  shmem_ctx_quiet(ctx);
  report("sessions: NBI completion", src[0] == 91);
  shmem_ctx_session_stop(ctx);
}

int main(int argc, char **argv) {
  int provided;
  int requested = SHMEM_THREAD_MULTIPLE;
  if (argc == 2 && strcmp(argv[1], "--single") == 0)
    requested = SHMEM_THREAD_SINGLE;
  else if (argc != 1)
    return EXIT_FAILURE;
  shmem_init_thread(requested, &provided);
  log_init(__FILE__);
  log_routine("shmem_ctx_session_start/stop");
  if (shmem_n_pes() < 2)
    shmem_global_exit(EXIT_FAILURE);

  uint64_t *dst = shmem_malloc(WORDS * sizeof(*dst));
  uint64_t *src = malloc(WORDS * sizeof(*src));
  if (dst == NULL || src == NULL)
    shmem_global_exit(EXIT_FAILURE);

  const long options[] = {0, SHMEM_CTX_PRIVATE, SHMEM_CTX_SERIALIZED};
  const size_t hints[] = {0, 1, SIZE_MAX};
  exercise(SHMEM_CTX_DEFAULT, dst, src, 1);
  reset(dst);
  start(SHMEM_CTX_DEFAULT, 1);
  P(SHMEM_CTX_DEFAULT, dst, UINT64_C(303), (shmem_my_pe() + 1) % shmem_n_pes());
  shmem_barrier_all();
  report("sessions: default-context barrier", dst[0] == 303);
  shmem_ctx_session_stop(SHMEM_CTX_DEFAULT);
  shmem_quiet();
  for (size_t c = 0; c < sizeof(options) / sizeof(options[0]); ++c) {
    shmem_ctx_t ctx;
    if (shmem_ctx_create(options[c], &ctx) != 0)
      shmem_global_exit(EXIT_FAILURE);
    for (size_t h = 0; h < sizeof(hints) / sizeof(hints[0]); ++h)
      exercise(ctx, dst, src, hints[h]);
    shmem_ctx_destroy(ctx);
  }

  shmem_ctx_t a, b;
  if (shmem_ctx_create(SHMEM_CTX_PRIVATE, &a) != 0 ||
      shmem_ctx_create(SHMEM_CTX_PRIVATE, &b) != 0)
    shmem_global_exit(EXIT_FAILURE);
  reset(dst);
  start(a, 1);
  start(b, 1);
  const int next = (shmem_my_pe() + 1) % shmem_n_pes();
  P(a, dst, UINT64_C(101), next);
  P(b, dst + 1, UINT64_C(202), next);
  shmem_ctx_session_stop(b);
  shmem_ctx_quiet(b);
  shmem_barrier_all();
  report("sessions: independent context completion", dst[1] == 202);
  shmem_ctx_session_stop(a);
  shmem_ctx_destroy(a);
  shmem_ctx_destroy(b);
  shmem_barrier_all();
  report("sessions: destroy after stop", dst[0] == 101);
  shmem_ctx_session_start(SHMEM_CTX_INVALID, 0, NULL, 0);
  shmem_ctx_session_stop(SHMEM_CTX_INVALID);

  free(src);
  shmem_free(dst);
  const int rc = any_failure ? EXIT_FAILURE : EXIT_SUCCESS;
  log_close(rc);
  shmem_finalize();
  return rc;
}
