/**
 * @file c11_shmem_team_split_2d.c
 * @brief Unit test for the shmem_team_split_2d() routine.
 */

#include <shmem.h>

#include "log.h"
#include "shmemvv.h"

bool test_shmem_team_split_2d(void) {
  log_routine("shmem_team_split_2d()");
  shmem_team_t team_x, team_y;

  log_info("Attempting to split SHMEM_TEAM_WORLD into 2D teams");
  log_info("Using xdim=2 and auto-calculated ydim");

  int ret = shmem_team_split_2d(SHMEM_TEAM_WORLD, 2, NULL, 0, &team_x, NULL, 0,
                                &team_y);

  if (ret != 0) {
    log_fail("Team split failed with error code %d", ret);
    return false;
  }
  if (team_x == SHMEM_TEAM_INVALID || team_y == SHMEM_TEAM_INVALID) {
    log_fail("Team split failed: got SHMEM_TEAM_INVALID");
    return false;
  }
  log_info("Team split successful");

  int npes_x = shmem_team_n_pes(team_x);
  int npes_y = shmem_team_n_pes(team_y);
  log_info("Team dimensions: %d x %d PEs", npes_x, npes_y);

  log_info("Destroying teams");
  shmem_team_destroy(team_x);
  shmem_team_destroy(team_y);
  log_info("Teams destroyed successfully");

  bool result = (npes_x > 0 && npes_y > 0);
  if (!result) {
    log_fail("Invalid team dimensions: %d x %d (expected > 0)", npes_x, npes_y);
  }
  return result;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  log_info("Running on PE %d of %d total PEs", mype, npes);

  if (!(npes >= 2)) {
    if (mype == 0) {
      log_fail("Test requires at least 2 PEs, but only %d PE(s) available",
               npes);
      display_not_enough_pes("TEAMS");
    }
    shmem_finalize();
    return EXIT_SUCCESS;
  }

  bool result = test_shmem_team_split_2d();
  int rc = result ? EXIT_SUCCESS : EXIT_FAILURE;

  if (mype == 0) {
    display_test_result("C11 shmem_team_split_2d", result, false);
  }

  log_info("Test completed with %s", result ? "SUCCESS" : "FAILURE");
  log_close(rc);
  shmem_finalize();
  return rc;
}
