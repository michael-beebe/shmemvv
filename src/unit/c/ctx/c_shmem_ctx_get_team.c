/**
 * @file c_shmem_ctx_get_team.c
 * @brief Unit test for the shmem_ctx_get_team() routine.
 */


#include <stdio.h>
#include <stdlib.h>
#include <shmem.h>

#include "shmemvv.h"

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
  
  if (result) {
    if (shmem_my_pe() == 0) {
      display_test_result("C shmem_ctx_get_team()", result, false);
    }
  } else {
    if (shmem_my_pe() == 0) {
      display_test_result("C shmem_ctx_get_team()", result, false);
      rc = EXIT_FAILURE;
    }
  }

  shmem_finalize();
  return rc;
}
