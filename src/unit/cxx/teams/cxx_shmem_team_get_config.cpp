/**
 * @file cxx_shmem_team_get_config.cpp
 * @brief Unit test for the shmem_team_get_config() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

bool test_shmem_team_get_config(void) {
  shmem_team_t team;
  shmem_team_config_t config;
  long config_mask = SHMEM_TEAM_NUM_CONTEXTS;

  log_routine("shmem_team_get_config()");
  log_info("Attempting to split SHMEM_TEAM_WORLD into new team");
  log_info("Using stride=1, start=0 and all PEs");

  shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(), NULL, 0,
                           &team);
  if (team == SHMEM_TEAM_INVALID) {
    log_fail("Team split failed - team is invalid");
    return false;
  }

  log_info("Team split successful, getting team config");
  shmem_team_get_config(team, config_mask, &config);
  bool result = (config.num_contexts >= 0);

  if (result) {
    log_info("Team config retrieved successfully - num_contexts = %d",
             config.num_contexts);
  } else {
    log_fail("Invalid num_contexts value: %d", config.num_contexts);
  }

  log_info("Destroying team");
  shmem_team_destroy(team);
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

  bool result = test_shmem_team_get_config();
  int rc = result ? EXIT_SUCCESS : EXIT_FAILURE;

  log_info("Entering barrier after team operations");
  shmem_barrier_all();

  if (mype == 0) {
    display_test_result("CXX shmem_team_get_config()", result, false);
  }

  log_info("Test completed with %s", result ? "SUCCESS" : "FAILURE");
  log_close(rc);
  shmem_finalize();
  return rc;
}
