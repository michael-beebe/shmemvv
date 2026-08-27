/**
 * @file c_shmem_team_destroy.c
 * @brief Unit test for the shmem_team_destroy() routine
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"
#include "shmemvv_teams.h"

/**
 * @brief Create and destroy the team named by the triplet, twice.
 */
static bool check_destroy(int start, int stride, int size, const char *label) {
  int mype = shmem_my_pe();
  bool want_member = triplet_names_me(start, stride, size);
  bool passed = true;

  for (int round = 0; round < 2; ++round) {
    shmem_team_t team;

    log_info("[%s] Round %d: splitting SHMEM_TEAM_WORLD with start=%d, "
             "stride=%d, size=%d",
             label, round, start, stride, size);

    int ret = shmem_team_split_strided(SHMEM_TEAM_WORLD, start, stride, size,
                                       NULL, 0, &team);
    if (ret != 0) {
      log_fail("[%s] Round %d: team split failed with error code %d", label,
               round, ret);
      return false;
    }

    if (want_member && team == SHMEM_TEAM_INVALID) {
      log_fail("[%s] Round %d: PE %d is named by the triplet but received "
               "SHMEM_TEAM_INVALID - a team destroyed in the previous round "
               "was not released",
               label, round, mype);
      passed = false;
    } else if (!want_member && team != SHMEM_TEAM_INVALID) {
      log_fail("[%s] Round %d: PE %d is not named by the triplet but did not "
               "receive SHMEM_TEAM_INVALID",
               label, round, mype);
      passed = false;
    }

    if (team != SHMEM_TEAM_INVALID) {
      int team_npes = shmem_team_n_pes(team);
      if (team_npes != size) {
        log_fail("[%s] Round %d: team has %d PEs, expected %d", label, round,
                 team_npes, size);
        passed = false;
      }
      log_info("[%s] Round %d: destroying team of %d PEs", label, round,
               team_npes);
      shmem_team_destroy(team);
      log_info("[%s] Round %d: team destroyed successfully", label, round);
    } else {
      log_info("[%s] Round %d: PE %d holds SHMEM_TEAM_INVALID, nothing to "
               "destroy",
               label, round, mype);
    }

    shmem_barrier_all();
  }

  return passed;
}

bool test_shmem_team_destroy(void) {
  log_routine("shmem_team_destroy()");
  int npes = shmem_n_pes();
  bool passed = true;

  if (!check_destroy(0, 1, npes, "all PEs, stride 1")) {
    passed = false;
  }

  if (!check_destroy(0, 2, (npes + 1) / 2, "even PEs, stride 2")) {
    passed = false;
  }

  if (!check_destroy(1, 2, npes / 2, "odd PEs, stride 2")) {
    passed = false;
  }

  if (!check_destroy(npes - 1, -1, npes, "all PEs, stride -1")) {
    passed = false;
  }

  return passed;
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
    log_close(EXIT_SUCCESS);
    shmem_finalize();
    return EXIT_SUCCESS;
  }

  bool *result = (bool *)shmem_malloc(sizeof(bool));
  if (result == NULL) {
    log_fail("Memory allocation failed: shmem_malloc returned NULL pointer");
    log_close(EXIT_FAILURE);
    shmem_finalize();
    return EXIT_FAILURE;
  }

  *result = test_shmem_team_destroy();
  int rc = *result ? EXIT_SUCCESS : EXIT_FAILURE;

  log_info("Entering barrier after team operations");
  shmem_barrier_all();
  reduce_test_result("C shmem_team_destroy", result, false);
  shmem_barrier_all();

  log_info("Test completed with %s", *result ? "SUCCESS" : "FAILURE");
  shmem_free(result);

  log_close(rc);
  shmem_finalize();
  return rc;
}
