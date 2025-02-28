/**
 * @file c_shmem_team_create_ctx.cpp
 * @brief Unit test for the shmem_team_create_ctx() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"
#include "log.h"

/**
 * @brief Tests the `shmem_team_create_ctx` function.
 *
 * This function verifies the correct behavior of `shmem_team_create_ctx` by:
 * 1. Creating a new team using `shmem_team_split_strided`.
 * 2. Calling `shmem_team_create_ctx` to create a context for the team.
 * 3. Checking the return value of `shmem_team_create_ctx` for errors.
 * 4. Destroying the context and team using `shmem_ctx_destroy` and
 * `shmem_team_destroy`, respectively.
 *
 * @return True if the test passes, false otherwise.
 */
bool test_shmem_team_create_ctx(void) {
  log_routine("shmem_team_create_ctx()");

  shmem_team_t team;
  shmem_ctx_t ctx;

  log_info("creating team using shmem_team_split_strided");
  shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(), NULL, 0,
                           &team);

  log_info("creating context for team");
  int ret = shmem_team_create_ctx(team, 0, &ctx);
  if (ret != 0) {
    log_fail("shmem_team_create_ctx returned non-zero value %d", ret);
    return false;
  }
  log_info("context created successfully");

  log_info("destroying context and team");
  shmem_ctx_destroy(ctx);
  shmem_team_destroy(team);
  log_info("cleanup complete");

  return true;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  if (!(shmem_n_pes() <= 2)) {
    if (shmem_my_pe() == 0) {
      display_not_enough_pes("ctx");
    }
    log_close(EXIT_SUCCESS);
    shmem_finalize();
    return EXIT_SUCCESS;
  }

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= test_shmem_team_create_ctx();

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_team_create_ctx()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
