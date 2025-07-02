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
  int world_mype = shmem_my_pe();
  int world_npes = shmem_n_pes();

  log_info("World PE %d of %d attempting to split SHMEM_TEAM_WORLD", world_mype, world_npes);
  log_info("Using stride=1, start=0 and all PEs");
  int ret = shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, world_npes,
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
  
  shmem_barrier_all();

  int team_npes = shmem_team_n_pes(team);
  log_info("Team has %d PEs (expected %d)", team_npes, world_npes);
  
  if (team_npes != world_npes) {
    log_fail("Team size mismatch: expected %d, got %d", world_npes, team_npes);
    shmem_team_destroy(team);
    return false;
  }

  log_info("Calling shmem_team_my_pe()");
  int my_pe = shmem_team_my_pe(team);
  log_info("Got team PE %d (world PE %d)", my_pe, world_mype);

  bool valid = false;
  if (my_pe == -1) {
    log_fail("shmem_team_my_pe() returned -1, indicating PE %d is not in team", world_mype);
  } else if (my_pe < 0 || my_pe >= team_npes) {
    log_fail("shmem_team_my_pe() returned invalid PE %d (valid range: 0-%d)", my_pe, team_npes-1);
  } else {
    log_info("shmem_team_my_pe() returned valid team PE %d", my_pe);
    valid = true;
  }

  log_info("Destroying team");
  shmem_team_destroy(team);
  log_info("Team destroyed successfully");

  return valid;
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
