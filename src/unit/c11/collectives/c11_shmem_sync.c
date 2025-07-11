/**
 * @file c11_shmem_sync.c
 * @brief Unit test for the shmem_team_sync() routine.
 */

#include <shmem.h>

#include "log.h"
#include "shmemvv.h"

bool test_shmem_team_sync(void) {
  log_routine("shmem_team_sync()");
  bool success = true;

  /* Allocate counter in symmetric memory so all PEs can access it */
  long *shared_counter = (long *)shmem_malloc(sizeof(long));
  if (shared_counter == NULL) {
    log_fail("Failed to allocate shared counter");
    return false;
  }

  *shared_counter = 0;
  log_info("Shared counter initialized at address %p", (void *)shared_counter);
  log_info("Initialized shared counter to 0");
  shmem_barrier_all();
  log_info("Completed initial barrier synchronization");

  shmem_team_t team;
  log_info("Attempting to split SHMEM_TEAM_WORLD into new team");
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

  log_info("Performing atomic increment on shared counter at PE 0");
  shmem_atomic_inc(shared_counter, 0);

  log_info("Initiating team synchronization");
  shmem_sync(team);
  log_info("Team synchronization completed");

  log_info("Validating shared counter value");
  /* All PEs read the counter value from PE 0 */
  long final_count = shmem_long_g(shared_counter, 0);
  if (final_count != shmem_n_pes()) {
    log_fail("Shared counter validation failed: expected %d, got %ld",
             shmem_n_pes(), final_count);
    if (final_count < shmem_n_pes()) {
      log_fail("Team synchronization may have failed to propagate all atomic "
               "operations");
    } else {
      log_fail("Either team split created incorrect number of teams or team "
               "sync duplicated operations");
    }
    success = false;
  } else {
    log_info("Shared counter validation successful: value matches expected %d",
             shmem_n_pes());
  }

  log_info("Destroying team");
  shmem_team_destroy(team);
  shmem_free(shared_counter);
  log_info("Team destroyed successfully");
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

  if (mype == 0) {
    display_test_result("C11 shmem_sync", result, false);
  }

  log_info("Test completed with %s", result ? "SUCCESS" : "FAILURE");
  log_close(rc);
  shmem_finalize();
  return rc;
}
