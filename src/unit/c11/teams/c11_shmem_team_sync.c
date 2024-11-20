/**
 * @file c11_shmem_team_sync.c
 * @brief Unit test for the shmem_team_sync() routine.
 */

#include <shmem.h>

#include "log.h"
#include "shmemvv.h"

bool test_shmem_team_sync(void) {
  log_routine("shmem_team_sync()");
  static long shared_counter;
  log_info("counter @ %p", (void *)&shared_counter);
  bool success = true;

  shared_counter = 0;
  log_info("set counter to 0");
  shmem_barrier_all();

  shmem_team_t team;
  log_info("splitting into teams");
  shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(), NULL, 0,
                           &team);

  log_info("incrementing counter");
  shmem_atomic_inc(&shared_counter, 0);

  log_info("calling team_sync");
  shmem_team_sync(team);

  log_info("validating result...");
  if (shared_counter != shmem_n_pes()) {
    log_fail("expected counter = %d, got %d!", shmem_n_pes(), shared_counter);
    if (shared_counter < shmem_n_pes())
      log_fail("team_sync may not have sent all atomic operations!");
    else
      log_fail("team_split_strided may have made too many teams, or team_sync "
               "may have duplicated operations!");
    success = false;
  } else {
    log_info("result is valid!");
  }

  shmem_team_destroy(team);
  return success;
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

  bool result = test_shmem_team_sync();
  int rc = result ? EXIT_SUCCESS : EXIT_FAILURE;

  if (mype == 0) {
    display_test_result("C11 shmem_team_sync()", result, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
