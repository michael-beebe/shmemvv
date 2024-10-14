/**
 * @file c_shmem_team_split_strided.cpp
 * @brief Unit test for the shmem_team_split_strided() routine.
 */

#include <shmem.h>

#include "shmemvv.h"

bool test_shmem_team_split_strided(void) {
  shmem_team_t team;
  shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(), NULL, 0,
                           &team);
  int npes = shmem_team_n_pes(team);
  shmem_team_destroy(team);
  return (npes == shmem_n_pes());
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

  bool result = test_shmem_team_split_strided();
  int rc = result ? EXIT_SUCCESS : EXIT_FAILURE;

  if (mype == 0) {
    display_test_result("CXX shmem_team_split_strided()", result, false);
  }

  shmem_finalize();
  return rc;
}
