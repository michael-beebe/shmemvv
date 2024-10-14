/**
 * @file c_shmem_team_sync.c
 * @brief Unit test for the shmem_team_sync() routine.
 */

#include <shmem.h>

#include "shmemvv.h"

bool test_shmem_team_sync(void) {
  static long pSync[SHMEM_SYNC_SIZE];
  static long shared_counter;
  bool success = true;

  for (int i = 0; i < SHMEM_SYNC_SIZE; i++) {
    pSync[i] = SHMEM_SYNC_VALUE;
  }

  shared_counter = 0;
  shmem_barrier_all();

  shmem_team_t team;
  shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(), NULL, 0,
                           &team);

  shmem_atomic_inc(&shared_counter, 0);

  shmem_team_sync(team);

  if (shared_counter != shmem_n_pes()) {
    success = false;
  }

  shmem_team_destroy(team);
  return success;
}

int main(int argc, char *argv[]) {
  shmem_init();
  
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
    display_test_result("C shmem_team_sync()", result,
                       false);
  }

  shmem_finalize();
  return rc;
}
