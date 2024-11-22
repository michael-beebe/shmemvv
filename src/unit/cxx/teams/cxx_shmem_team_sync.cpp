/**
 * @file cxx_shmem_team_sync.cpp
 * @brief Unit test for the shmem_team_sync() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

bool test_shmem_team_sync(void) {
  static long pSync[SHMEM_SYNC_SIZE];
  static long shared_counter;
  bool success = true;

  log_routine("shmem_team_sync()");

  log_info("Initializing sync array");
  for (int i = 0; i < SHMEM_SYNC_SIZE; i++) {
    pSync[i] = SHMEM_SYNC_VALUE;
  }

  shared_counter = 0;
  log_info("Entering initial barrier");
  shmem_barrier_all();

  log_info("Attempting to split SHMEM_TEAM_WORLD into new team");
  log_info("Using stride=1, start=0 and all PEs");
  shmem_team_t team;
  int ret = shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(),
                                     NULL, 0, &team);

  if (ret != 0) {
    log_fail("Team split failed with error code %d", ret);
    return false;
  }

  log_info("Incrementing shared counter on PE 0");
  shmem_atomic_inc(&shared_counter, 0);

  log_info("Performing team sync");
  shmem_team_sync(team);

  if (shared_counter != shmem_n_pes()) {
    log_fail("Incorrect counter value after sync: %ld (expected %d)",
             shared_counter, shmem_n_pes());
    success = false;
  } else {
    log_info("Counter value correct after sync: %ld", shared_counter);
  }

  log_info("Destroying team");
  shmem_team_destroy(team);
  return success;
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

  bool result = test_shmem_team_sync();
  int rc = result ? EXIT_SUCCESS : EXIT_FAILURE;

  log_info("Entering barrier after team operations");
  shmem_barrier_all();

  if (mype == 0) {
    display_test_result("CXX shmem_team_sync()", result, false);
  }

  log_info("Test completed with %s", result ? "SUCCESS" : "FAILURE");
  log_close(rc);
  shmem_finalize();
  return rc;
}
