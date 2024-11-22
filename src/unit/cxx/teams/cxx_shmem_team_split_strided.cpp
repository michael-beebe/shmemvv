/**
 * @file cxx_shmem_team_split_strided.cpp
 * @brief Unit test for the shmem_team_split_strided() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

bool test_shmem_team_split_strided(void) {
  shmem_team_t team;
  log_routine("shmem_team_split_strided()");

  log_info("Attempting to split SHMEM_TEAM_WORLD into new team");
  log_info("Using stride=1, start=0 and all PEs");

  int ret = shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(), NULL, 0,
                           &team);

  if (ret != 0) {
    log_fail("Team split failed with error code %d", ret);
    return false;
  }

  log_info("Team split successful, getting number of PEs in team");
  int npes = shmem_team_n_pes(team);

  bool result = (npes == shmem_n_pes());
  if (result) {
    log_info("Got correct number of PEs: %d", npes);
  } else {
    log_fail("Got incorrect number of PEs: %d (expected %d)", npes, shmem_n_pes());
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
      log_fail("Test requires at least 2 PEs, but only %d PE(s) available", npes);
      display_not_enough_pes("TEAMS");
    }
    shmem_finalize();
    return EXIT_SUCCESS;
  }

  bool result = test_shmem_team_split_strided();
  int rc = result ? EXIT_SUCCESS : EXIT_FAILURE;

  log_info("Entering barrier after team operations");
  shmem_barrier_all();

  if (mype == 0) {
    display_test_result("CXX shmem_team_split_strided()", result, false);
  }

  log_info("Test completed with %s", result ? "SUCCESS" : "FAILURE");
  log_close(rc);
  shmem_finalize();
  return rc;
}
