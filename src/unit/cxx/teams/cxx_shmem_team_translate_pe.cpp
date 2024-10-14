/**
 * @file c_shmem_team_translate_pe.cpp
 * @brief Unit test for the shmem_team_translate_pe() routine.
 */

#include <shmem.h>

#include "shmemvv.h"

bool test_shmem_team_translate_pe(void) {
  shmem_team_t team;
  shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(), NULL, 0,
                           &team);
  int pe_in_team = shmem_team_translate_pe(team, 0, SHMEM_TEAM_WORLD);
  shmem_team_destroy(team);
  return (pe_in_team >= 0);
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
  
  bool result = test_shmem_team_translate_pe();
  int rc = result ? EXIT_SUCCESS : EXIT_FAILURE;
  
  if (mype == 0) {
    display_test_result("CXX shmem_team_translate_pe()", result,
                       false);
  }

  shmem_finalize();
  return rc;
}
