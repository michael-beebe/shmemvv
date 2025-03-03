/**
 * @file c_shmem_team_my_pe.c
 * @brief Unit test for the shmem_team_my_pe() routine.
 */

#include <shmem.h>

#include "log.h"
#include "shmemvv.h"

bool test_shmem_team_my_pe(void) {
  log_routine("shmem_team_my_pe()");
  shmem_team_t team;

  log_info("Attempting to split SHMEM_TEAM_WORLD into new team");
  log_info("Using stride=1, start=0 and all PEs");
  int ret = shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(),
                                     NULL, 0, &team);

  if (ret != 0) {
    log_fail("Team split failed with error code %d", ret);
    return false;
  }
  if (team == SHMEM_TEAM_INVALID) {
    log_fail("Team split failed: got SHMEM_TEAM_INVALID");
    return false;
  }
  log_info("Team split successful");

  log_info("Calling shmem_team_my_pe()");
  int my_pe = shmem_team_my_pe(team);
  log_info("Got PE %d", my_pe);

  log_info("Destroying team");
  shmem_team_destroy(team);
  log_info("Team destroyed successfully");

  return (my_pe >= 0);
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

  bool result = test_shmem_team_my_pe();
  int rc = result ? EXIT_SUCCESS : EXIT_FAILURE;

  if (mype == 0) {
    display_test_result("C shmem_team_my_pe", result, false);
  }

  log_info("Test completed with %s", result ? "SUCCESS" : "FAILURE");
  log_close(rc);
  shmem_finalize();
  return rc;
}
