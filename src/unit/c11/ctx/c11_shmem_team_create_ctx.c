/**
 * @file c11_shmem_team_create_ctx.c
 * @brief Unit test for the shmem_team_create_ctx() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"

bool test_shmem_team_create_ctx(void) {
  shmem_team_t team;
  shmem_ctx_t ctx;
  shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(), NULL, 0,
                           &team);
  int ret = shmem_team_create_ctx(team, 0, &ctx);
  if (ret != 0) {
    return false;
  }
  shmem_ctx_destroy(ctx);
  shmem_team_destroy(team);
  return true;
}

int main(int argc, char *argv[]) {
  shmem_init();

  if (!(shmem_n_pes() <= 2)) {
    if (shmem_my_pe() == 0) {
      display_not_enough_pes("ctx");
    }
  }

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= test_shmem_team_create_ctx();

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_team_create_ctx()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return rc;
}
