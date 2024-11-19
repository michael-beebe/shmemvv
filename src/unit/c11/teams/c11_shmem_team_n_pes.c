/**
 * @file c11_shmem_team_n_pes.c
 * @brief Unit test for the shmem_team_n_pes() routine.
 */

#include <shmem.h>

#include "log.h"
#include "shmemvv.h"

bool test_shmem_team_n_pes(void) {
  log_routine("shmem_team_n_pes()");
  shmem_team_t team;
  log_info("splitting into teams");
  shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(), NULL, 0,
                           &team);
  log_info("calling team_n_pes");
  int npes = shmem_team_n_pes(team);
  log_info("got team npes %d", npes);
  shmem_team_destroy(team);
  return (npes == shmem_n_pes());
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

  bool result = test_shmem_team_n_pes();
  int rc = result ? EXIT_SUCCESS : EXIT_FAILURE;

  if (mype == 0) {
    display_test_result("C11 shmem_team_n_pes()", result, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
