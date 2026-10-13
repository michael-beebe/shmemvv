/**
 * @file c_shmem_query_initialized.c
 * @brief Query initialization from the calling and another thread in all
 * states.
 */
#include <shmem.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "shmemvv.h"

static void *query_thread(void *arg) {
  shmem_query_initialized((int *)arg);
  return NULL;
}

static bool test_query_initialized(bool expected) {
  int initialized = -1;
  shmem_query_initialized(&initialized);
  bool passed = initialized != -1 && (initialized != 0) == expected;

  /* Query is legal on another thread even with SHMEM_THREAD_SINGLE, and
     before initialization or after finalization. No other SHMEM call occurs
     on this thread. */
  pthread_t thread;
  int threaded = -1;
  if (pthread_create(&thread, NULL, query_thread, &threaded) != 0)
    return false;
  if (pthread_join(thread, NULL) != 0)
    return false;
  return passed && threaded != -1 && (threaded != 0) == expected;
}

int main(int argc, char **argv) {
  bool before = test_query_initialized(false);
  int provided = -1;
  int init_rc = 0;
  bool threaded_init = argc > 1 && strcmp(argv[1], "--init-thread") == 0;
  if (threaded_init)
    init_rc = shmem_init_thread(SHMEM_THREAD_SINGLE, &provided);
  else
    shmem_init();
  log_init(__FILE__);
  log_routine("shmem_query_initialized");

  static bool result;
  result = before && test_query_initialized(true);
  if (threaded_init)
    result &= init_rc == 0 && provided >= SHMEM_THREAD_SINGLE;
  if (!result)
    log_fail("Initialization query or thread initialization failed");
  shmem_barrier_all();
  reduce_test_result("C shmem_query_initialized before/during init", &result,
                     false);
  shmem_barrier_all();
  int me = shmem_my_pe();
  int rc = result ? EXIT_SUCCESS : EXIT_FAILURE;
  log_close(rc);
  shmem_finalize();

  bool after = test_query_initialized(false);
  /* The normal result helper cannot communicate after finalization. Report
     each failing PE so a remote-rank failure cannot be hidden by PE 0. */
  if (me == 0 || !after)
    display_test_result("C shmem_query_initialized after finalize", after,
                        false);
  return after ? rc : EXIT_FAILURE;
}
