/**
 * @file cxx_shmem_team_translate_pe.cpp
 * @brief Unit test for the shmem_team_translate_pe() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

bool test_shmem_team_translate_pe(void) {
  shmem_team_t team;
  log_routine("shmem_team_translate_pe()");

  log_info("Attempting to split SHMEM_TEAM_WORLD into new team");
  log_info("Using stride=1, start=0 and all PEs");

  int ret = shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(),
                                     NULL, 0, &team);

  if (ret != 0) {
    log_fail("Team split failed with error code %d", ret);
    return false;
  }

  log_info(
      "Team split successful, translating PE 0 from team to SHMEM_TEAM_WORLD");
  int pe_in_team = shmem_team_translate_pe(team, 0, SHMEM_TEAM_WORLD);

  bool result = (pe_in_team >= 0);
  if (result) {
    log_info("Got valid translated PE number: %d", pe_in_team);
  } else {
    log_fail("Got invalid translated PE number: %d", pe_in_team);
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

  bool result = test_shmem_team_translate_pe();
  int rc = result ? EXIT_SUCCESS : EXIT_FAILURE;

  log_info("Entering barrier after team operations");
  shmem_barrier_all();

  if (mype == 0) {
    display_test_result("CXX shmem_team_translate_pe()", result, false);
  }

  log_info("Test completed with %s", result ? "SUCCESS" : "FAILURE");
  log_close(rc);
  shmem_finalize();
  return rc;
}
