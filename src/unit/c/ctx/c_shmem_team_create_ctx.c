/**
 * @file c_shmem_team_create_ctx.c
 * @brief Unit test for the shmem_team_create_ctx() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

/**
 * @brief Tests the `shmem_team_create_ctx` function.
 *
 * This function verifies the correct behavior of `shmem_team_create_ctx` by:
 * 1. Creating a new team using `shmem_team_split_strided`.
 * 2. Calling `shmem_team_create_ctx` to create a context for the team.
 * 3. Checking the return value of `shmem_team_create_ctx` for errors.
 * 4. Destroying the context and team using `shmem_ctx_destroy` and `shmem_team_destroy`, respectively.
 *
 * @return True if the test passes, false otherwise.
 */
bool test_shmem_team_create_ctx(void) {
  shmem_team_t team;
  shmem_ctx_t ctx;
  shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(), NULL, 0,
                           &team);
  log_info("creating teams...");
  int ret = shmem_team_create_ctx(team, 0, &ctx);
  if (ret != 0) {
    log_fail("shmem_team_create_ctx ret'd non-zero: %d", ret);
    return false;
  }
  shmem_ctx_destroy(ctx);
  shmem_team_destroy(team);
  return true;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  if (!(shmem_n_pes() <= 2)) {
    if (shmem_my_pe() == 0) {
      log_fail("need at least 2 PEs, found %d", shmem_n_pes());
      display_not_enough_pes("ctx");
    }
  }

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= test_shmem_team_create_ctx();

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_team_create_ctx()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
