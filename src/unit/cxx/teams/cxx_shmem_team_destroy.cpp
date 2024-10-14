/**
 * @file c_shmem_team_destroy.cpp
 * @brief Unit test for the shmem_team_destroy() routine
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"

bool test_shmem_team_destroy(void) {
  shmem_team_t team;
  shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(), NULL, 0,
                           &team);
  shmem_team_destroy(team);
  if (!(team == SHMEM_TEAM_INVALID)) {
    return true;
  } else {
    return false;
  }
}

int main(int argc, char *argv[]) {
  shmem_init();

  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  if (!(npes >= 2)) {
    if (mype == 0) {
      display_not_enough_pes("SIGNALING");
    }
    shmem_finalize();
    return EXIT_SUCCESS;
  }

  bool result = true;
  int rc = EXIT_SUCCESS;

  result = test_shmem_team_destroy();

  shmem_barrier_all();

  if (!result) {
    rc = EXIT_FAILURE;
  }

  if (mype == 0) {
    display_test_result("CXX shmem_team_destroy()", result, false);
  }

  shmem_finalize();
  return rc;
}
