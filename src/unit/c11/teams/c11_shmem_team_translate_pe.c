/**
 * @file c11_shmem_team_translate_pe.c
 * @brief Unit test for the shmem_team_translate_pe() routine.
 */

#include <shmem.h>

#include "log.h"
#include "shmemvv.h"

bool test_shmem_team_translate_pe(void) {
  log_routine("shmem_team_translate_pe()");
  shmem_team_t team;
  log_info("splitting into teams");
  shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(), NULL, 0,
                           &team);
  log_info("calling team_translate_pe(split_team -> world team)");
  int pe_in_team = shmem_team_translate_pe(team, 0, SHMEM_TEAM_WORLD);
  if(pe_in_team >= 0)
    log_info("returned pe is valid");
  else
    log_fail("returned pe is invalid! expected >= 0, got %d", pe_in_team);
  shmem_team_destroy(team);
  return (pe_in_team >= 0);
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

  bool result = test_shmem_team_translate_pe();
  int rc = result ? EXIT_SUCCESS : EXIT_FAILURE;

  if (mype == 0) {
    display_test_result("C11 shmem_team_translate_pe()", result, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
