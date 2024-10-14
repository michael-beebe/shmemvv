/**
 * @file c_shmem_team_split_2d.c
 * @brief Unit test for the shmem_team_split_2d() routine.
 */

#include <shmem.h>

#include "shmemvv.h"

bool test_shmem_team_split_2d(void) {
  shmem_team_t team_x, team_y;
  shmem_team_split_2d(SHMEM_TEAM_WORLD, 2, NULL, 0, &team_x, NULL, 0, &team_y);
  int npes_x = shmem_team_n_pes(team_x);
  int npes_y = shmem_team_n_pes(team_y);
  shmem_team_destroy(team_x);
  shmem_team_destroy(team_y);
  return (npes_x > 0 && npes_y > 0);
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

  bool result = test_shmem_team_split_2d();
  int rc = result ? EXIT_SUCCESS : EXIT_FAILURE;
  
  if (mype == 0) {
    display_test_result("C shmem_team_split_2d()", result,
                       false);
  }

  shmem_finalize();
  return rc;
}
