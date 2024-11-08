/**
 * @file c_shmem_ctx_get_team.cpp
 * @brief Unit test for the shmem_ctx_get_team() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

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
  shmem_team_t team;
  int ret = shmem_ctx_get_team(ctx, &team);
  shmem_ctx_destroy(ctx);
  return (ret == 0 && team == SHMEM_TEAM_WORLD);
}

int main(int argc, char *argv[]) {
  shmem_init();

  if (!(shmem_n_pes() <= 2)) {
    if (shmem_my_pe() == 0) {
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
    display_test_result("CXX shmem_ctx_get_team()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return rc;
}
