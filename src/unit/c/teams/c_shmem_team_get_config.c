/**
 * @file c_shmem_team_get_config.c
 * @brief Unit test for the shmem_team_get_config() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

bool test_shmem_team_get_config(void) {
  log_routine("shmem_team_get_config()");
  shmem_team_t team;
  shmem_team_config_t config;
  long config_mask = SHMEM_TEAM_NUM_CONTEXTS;

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

  log_info("Calling shmem_team_get_config() with config_mask = %ld "
           "(SHMEM_TEAM_NUM_CONTEXTS)",
           config_mask);
  shmem_team_get_config(team, config_mask, &config);

  bool result = (config.num_contexts >= 0);
  if (result) {
    log_info("Team config is valid - num_contexts = %d", config.num_contexts);
  } else {
    log_fail("Team config is invalid - num_contexts = %d (expected >= 0)",
             config.num_contexts);
  }

  log_info("Destroying team");
  shmem_team_destroy(team);
  log_info("Team destroyed successfully");

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
      log_fail("Test requires at least 2 PEs, but only %d PE(s) available", npes);
      display_not_enough_pes("TEAMS");
    }
    shmem_finalize();
    return EXIT_SUCCESS;
  }

  bool result = test_shmem_team_get_config();
  int rc = result ? EXIT_SUCCESS : EXIT_FAILURE;

  if (mype == 0) {
    display_test_result("C shmem_team_get_config()", result, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
