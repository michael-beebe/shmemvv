/**
 * @file c11_shmem_team_get_config.c
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
  log_info("splitting into teams");
  shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(), NULL, 0,
                           &team);
  if (team == SHMEM_TEAM_INVALID) {
    log_fail("team split failed: got SHMEM_TEAM_INVALID");
    return false;
  }
  log_info("calling shmem_team_get_config(config_mask = %ld "
           "(SHMEM_TEAM_NUM_CONTEXTS))",
           config_mask);
  shmem_team_get_config(team, config_mask, &config);
  bool result = (config.num_contexts >= 0);
  if (result)
    log_info("returned team config is valid");
  else
    log_fail("num contexts returned is invalid! expected >= 0, got %d",
             config.num_contexts);
  shmem_team_destroy(team);
  return result;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  if (!(npes >= 2)) {
    if (mype == 0) {
      display_not_enough_pes("TEAMS");
    }
    shmem_finalize();
    return EXIT_SUCCESS;
  }

  bool result = test_shmem_team_get_config();
  int rc = result ? EXIT_SUCCESS : EXIT_FAILURE;

  if (mype == 0) {
    display_test_result("C11 shmem_team_get_config()", result, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
