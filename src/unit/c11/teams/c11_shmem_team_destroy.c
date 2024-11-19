/**
 * @file c11_shmem_team_destroy.c
 * @brief Unit test for the shmem_team_destroy() routine
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

bool test_shmem_team_destroy(void) {
  shmem_team_t team;
  log_info("splitting into teams");
  shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(), NULL, 0,
                           &team);
  log_info("destroying teams");
  shmem_team_destroy(team);
  if (!(team == SHMEM_TEAM_INVALID)) {
    log_info("team was destroyed properly");
    return true;
  } else {
    log_fail("destroyed team is invalid!");
    return false;
  }
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

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
    display_test_result("C11 shmem_team_destroy()", result, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
