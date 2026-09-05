/**
 * @file c_shmem_team_n_pes.c
 * @brief Unit test for the shmem_team_n_pes() routine.
 */

#include <shmem.h>

#include "log.h"
#include "shmemvv.h"
#include "shmemvv_teams.h"

/**
 * @brief Check shmem_team_n_pes against one triplet.
 */
static bool check_n_pes(int start, int stride, int size, const char *label) {
  int mype = shmem_my_pe();
  shmem_team_t team;
  bool passed = true;

  log_info("[%s] Splitting SHMEM_TEAM_WORLD with start=%d, stride=%d, size=%d",
           label, start, stride, size);

  int ret = shmem_team_split_strided(SHMEM_TEAM_WORLD, start, stride, size,
                                     NULL, 0, &team);
  if (ret != 0) {
    log_fail("[%s] Team split failed with error code %d", label, ret);
    return false;
  }

  bool want_member = triplet_names_me(start, stride, size);

  if (!want_member) {
    if (team != SHMEM_TEAM_INVALID) {
      log_fail("[%s] PE %d is not named by the triplet but did not receive "
               "SHMEM_TEAM_INVALID, so shmem_team_n_pes cannot be checked here",
               label, mype);
      passed = false;
    } else {
      log_info("[%s] PE %d is not a member, skipping shmem_team_n_pes", label,
               mype);
    }
  } else if (team == SHMEM_TEAM_INVALID) {
    log_fail("[%s] PE %d is named by the triplet but received "
             "SHMEM_TEAM_INVALID",
             label, mype);
    passed = false;
  } else {
    log_info("[%s] Calling shmem_team_n_pes()", label);
    int team_npes = shmem_team_n_pes(team);
    log_info("[%s] Team has %d PEs (expected %d)", label, team_npes, size);

    if (team_npes != size) {
      log_fail("[%s] Team PE count %d does not match expected count %d", label,
               team_npes, size);
      passed = false;
    }
  }

  if (team != SHMEM_TEAM_INVALID) {
    log_info("[%s] Destroying team", label);
    shmem_team_destroy(team);
  }

  shmem_barrier_all();
  return passed;
}

bool test_shmem_team_n_pes(void) {
  log_routine("shmem_team_n_pes()");
  int npes = shmem_n_pes();
  bool passed = true;

  if (!check_n_pes(0, 1, npes, "all PEs, stride 1")) {
    passed = false;
  }

  if (!check_n_pes(0, 2, (npes + 1) / 2, "even PEs, stride 2")) {
    passed = false;
  }

  if (!check_n_pes(1, 2, npes / 2, "odd PEs, stride 2")) {
    passed = false;
  }

  if (!check_n_pes(npes - 1, -1, npes, "all PEs, stride -1")) {
    passed = false;
  }

  if (!check_n_pes(0, 0, 1, "single PE, stride 0")) {
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

  *result = test_shmem_team_n_pes();
  int rc = *result ? EXIT_SUCCESS : EXIT_FAILURE;

  shmem_barrier_all();
  reduce_test_result("C shmem_team_n_pes", result, false);
  shmem_barrier_all();

  log_info("Test completed with %s", *result ? "SUCCESS" : "FAILURE");
  shmem_free(result);

  log_close(rc);
  shmem_finalize();
  return rc;
}
