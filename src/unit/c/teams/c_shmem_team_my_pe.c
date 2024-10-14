/**
 * @file c_shmem_team_my_pe.c
 * @brief Unit test for the shmem_team_my_pe() routine.
 */

#include <shmem.h>

#include "shmemvv.h"

bool test_shmem_team_my_pe(void) {
  shmem_team_t team;
  shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(), NULL, 0,
                           &team);
  int my_pe = shmem_team_my_pe(team);
  shmem_team_destroy(team);
  return (my_pe >= 0);
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

  bool result = test_shmem_team_my_pe();
  int rc = result ? EXIT_SUCCESS : EXIT_FAILURE;
  
  if (mype == 0) {
    display_test_result("C shmem_team_my_pe()", result,
                       false);
  }

  shmem_finalize();
  return rc;
}
