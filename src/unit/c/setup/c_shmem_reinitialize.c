/**
 * @file c_shmem_reinitialize.c
 * @brief Repeated generations preserve nested references and renew contexts.
 */
#include <shmem.h>
#include <stdbool.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

int main(void) {
  int rc = EXIT_SUCCESS;
  static bool result;
  for (int generation = 0; generation < 8; ++generation) {
    int provided = -1;
    int requested =
        generation % 2 ? SHMEM_THREAD_SINGLE : SHMEM_THREAD_MULTIPLE;
    if (shmem_init_thread(requested, &provided) != 0)
      return EXIT_FAILURE;
    log_init(__FILE__);
    log_routine(
        "reinitialize with heap, context reuse and nested finalization");
    int me = shmem_my_pe();
    int npes = shmem_n_pes();
    int current = -1;
    shmem_query_thread(&current);
    result = provided == current && (provided == SHMEM_THREAD_SINGLE ||
                                     provided == SHMEM_THREAD_FUNNELED ||
                                     provided == SHMEM_THREAD_SERIALIZED ||
                                     provided == SHMEM_THREAD_MULTIPLE);
    shmem_init();
    int repeated = -1;
    if (shmem_init_thread(SHMEM_THREAD_SINGLE, &repeated) != 0) {
      log_close(EXIT_FAILURE);
      return EXIT_FAILURE;
    }
    shmem_query_thread(&current);
    result &= repeated == current;
    int *heap = shmem_calloc((size_t)npes, sizeof(*heap));
    shmem_ctx_t first = SHMEM_CTX_INVALID, active = SHMEM_CTX_INVALID;
    if (heap == NULL || shmem_ctx_create(0, &first) != 0) {
      log_fail("generation %d: heap/context allocation failed", generation);
      shmem_global_exit(EXIT_FAILURE);
    }
    shmem_ctx_destroy(first);
    if (shmem_ctx_create(0, &active) != 0 || shmem_ctx_create(0, &first) != 0) {
      log_fail("generation %d: context reuse/allocation failed", generation);
      shmem_global_exit(EXIT_FAILURE);
    }
    /* Leave both a live shareable context and an unused freelist slot for
       the last finalize to release. New generations must not reuse either. */
    shmem_ctx_destroy(first);
    for (int stage = 0; stage < 3; ++stage) {
      int initialized = -1;
      shmem_query_initialized(&initialized);
      if (initialized <= 0) {
        /* Query is legal after finalization; other SHMEM routines are not. */
        log_fail("generation %d stage %d: runtime finalized too early",
                 generation, stage);
        if (me == 0)
          display_test_result("reinitialized intermediate finalize", false,
                              false);
        log_close(EXIT_FAILURE);
        return EXIT_FAILURE;
      }
      int previous = (me + npes - 1) % npes;
      int expected = generation * 100 + stage * 10 + previous;
      shmem_ctx_int_p(active, &heap[me], generation * 100 + stage * 10 + me,
                      (me + 1) % npes);
      shmem_ctx_quiet(active);
      shmem_barrier_all();
      if (heap[previous] != expected) {
        log_fail("generation %d stage %d: got %d, expected %d", generation,
                 stage, heap[previous], expected);
        result = false;
      }
      shmem_barrier_all();
      if (stage < 2)
        shmem_finalize();
    }
    reduce_test_result("reinitialized heap/context and intermediate finalize",
                       &result, false);
    shmem_barrier_all();
    if (!result)
      rc = EXIT_FAILURE;
    shmem_free(heap);
    log_close(rc);
    shmem_finalize();
    int initialized = -1;
    shmem_query_initialized(&initialized);
    bool after = initialized == 0;
    if (me == 0 || !after)
      display_test_result("reinitialized last finalize", after, false);
    if (!after)
      return EXIT_FAILURE;
  }
  return rc;
}
