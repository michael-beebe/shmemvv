/**
 * @file c_shmem_init_finalize.c
 * @brief Initialization nesting, repeated init_thread and a fresh generation.
 */
#include <shmem.h>
#include <stdbool.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

int main(void) {
  shmem_init();
  log_init(__FILE__);
  log_routine("shmem_init / shmem_init_thread / shmem_finalize");
  int me = shmem_my_pe();
  int initial_level = -1;
  shmem_query_thread(&initial_level);
  shmem_init();
  int provided = -1;
  int init_rc = shmem_init_thread(SHMEM_THREAD_MULTIPLE, &provided);
  /* shmem_init_thread.tex permits the threading level to change. It requires
     a valid provided level, rather than equality with the first level. After
     unsuccessful initialization, further SHMEM calls are undefined. */
  if (init_rc != 0) {
    log_fail("Repeated init_thread returned error %d", init_rc);
    if (me == 0)
      display_test_result("C repeated shmem_init_thread provided", false,
                          false);
    log_close(EXIT_FAILURE);
    return EXIT_FAILURE;
  }
  int current_level = -1;
  shmem_query_thread(&current_level);
  bool valid_level =
      provided == SHMEM_THREAD_SINGLE || provided == SHMEM_THREAD_FUNNELED ||
      provided == SHMEM_THREAD_SERIALIZED || provided == SHMEM_THREAD_MULTIPLE;
  static bool result;
  result = valid_level && provided == current_level;
  if (!result)
    log_fail(
        "Repeated init_thread provided %d, current level %d, initial level %d",
        provided, current_level, initial_level);
  shmem_barrier_all();
  reduce_test_result("C repeated shmem_init_thread provided", &result, false);
  shmem_barrier_all();
  int rc = result ? EXIT_SUCCESS : EXIT_FAILURE;
  log_close(rc);

  for (int remaining = 2; remaining >= 0; --remaining) {
    shmem_finalize();
    int initialized = -1;
    shmem_query_initialized(&initialized);
    bool passed = initialized != -1 && (initialized != 0) == (remaining > 0);
    if (me == 0 || !passed)
      display_test_result("C matching init/finalize counts", passed, false);
    if (!passed) {
      /* If the runtime was torn down too soon, further SHMEM calls would be
         invalid and obscure the actual failure. */
      return EXIT_FAILURE;
    }
  }

  shmem_init();
  log_init(__FILE__);
  log_routine("shmem_init after complete finalization");
  int initialized = -1;
  shmem_query_initialized(&initialized);
  result = initialized != -1 && initialized != 0;
  shmem_barrier_all();
  reduce_test_result("C reinitialize after finalize", &result, false);
  shmem_barrier_all();
  if (!result)
    rc = EXIT_FAILURE;
  log_close(rc);
  shmem_finalize();
  shmem_query_initialized(&initialized);
  bool after = initialized == 0;
  if (me == 0 || !after)
    display_test_result("C reinitialized finalization", after, false);
  return after ? rc : EXIT_FAILURE;
}
