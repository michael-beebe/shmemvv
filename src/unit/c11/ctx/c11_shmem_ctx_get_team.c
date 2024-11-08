/**
 * @file c11_shmem_ctx_get_team.c
 * @brief Unit test for the shmem_ctx_get_team() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

/**
 * @brief Tests the `shmem_ctx_get_team()` function.
 *
 * This test case verifies that the `shmem_ctx_get_team()` function correctly retrieves the team associated with a given SHMEM context.
 *
 * @return `true` if the test passes, `false` otherwise.
 */
bool test_shmem_ctx_get_team(void) {
  shmem_ctx_t ctx;
  shmem_ctx_create(0, &ctx);
  log_info("constructed ctx");
  shmem_team_t team;
  int ret = shmem_ctx_get_team(ctx, &team);
  if(ret)
    log_fail("shmem_ctx_get_team ret'd non-zero: %d", ret);
  shmem_ctx_destroy(ctx);
  if(team != SHMEM_TEAM_WORLD)
    log_fail("shmem_ctx_get_team didn't set team to SHMEM_TEAM_WORLD");
  return (ret == 0 && team == SHMEM_TEAM_WORLD);
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  if (!(shmem_n_pes() <= 2)) {
    if (shmem_my_pe() == 0) {
      log_fail("need at least 2 PEs, found %d", shmem_n_pes());
      display_not_enough_pes("ctx");
    }
    shmem_finalize();
    return EXIT_SUCCESS;
  }

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= test_shmem_ctx_get_team();

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_ctx_get_team()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
