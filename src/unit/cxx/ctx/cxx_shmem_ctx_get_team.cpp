/**
 * @file c_shmem_ctx_get_team.cpp
 * @brief Unit test for the shmem_ctx_get_team() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"
#include "log.h"

/**
 * @brief Tests the `shmem_ctx_get_team()` function.
 *
 * This test case verifies that the `shmem_ctx_get_team()` function correctly
 * retrieves the team associated with a given SHMEM context.
 *
 * @return `true` if the test passes, `false` otherwise.
 */
bool test_shmem_ctx_get_team(void) {
  log_routine("shmem_ctx_get_team()");

  shmem_ctx_t ctx;
  log_info("attempting to create context");
  int create_ret = shmem_ctx_create(0, &ctx);
  if (create_ret != 0) {
    log_fail("shmem_ctx_create returned non-zero value %d", create_ret);
    return false;
  }
  log_info("context created successfully");

  shmem_team_t team;
  log_info("getting team from context");
  int ret = shmem_ctx_get_team(ctx, &team);
  if (ret != 0) {
    log_fail("shmem_ctx_get_team returned non-zero value %d", ret);
    shmem_ctx_destroy(ctx);
    return false;
  }

  bool success = (team == SHMEM_TEAM_WORLD);
  if (success) {
    log_info("team matches expected SHMEM_TEAM_WORLD");
  } else {
    log_fail("team does not match SHMEM_TEAM_WORLD");
  }

  log_info("destroying context");
  shmem_ctx_destroy(ctx);
  log_info("context destroyed");

  return success;
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

  result &= test_shmem_ctx_get_team();

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_ctx_get_team()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
