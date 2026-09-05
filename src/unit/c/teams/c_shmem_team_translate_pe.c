/**
 * @file c_shmem_team_translate_pe.c
 * @brief Unit test for the shmem_team_translate_pe() routine.
 */

#include <shmem.h>

#include "log.h"
#include "shmemvv.h"
#include "shmemvv_teams.h"

/**
 * @brief Check shmem_team_translate_pe against one triplet.
 */
static bool check_translate(int start, int stride, int size,
                            const char *label) {
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

  int want = expected_team_pe(start, stride, size);

  if (want < 0) {
    if (team != SHMEM_TEAM_INVALID) {
      log_fail("[%s] PE %d is not named by the triplet but did not receive "
               "SHMEM_TEAM_INVALID, so shmem_team_translate_pe cannot be "
               "checked here",
               label, mype);
      passed = false;
    } else {
      int translated = shmem_team_translate_pe(team, 0, SHMEM_TEAM_WORLD);
      log_info("[%s] shmem_team_translate_pe(SHMEM_TEAM_INVALID, 0, "
               "SHMEM_TEAM_WORLD) returned %d",
               label, translated);
      if (translated != -1) {
        log_fail("[%s] Expected -1 when src_team is SHMEM_TEAM_INVALID, got %d",
                 label, translated);
        passed = false;
      }
    }
  } else if (team == SHMEM_TEAM_INVALID) {
    log_fail("[%s] PE %d should be team PE %d but received SHMEM_TEAM_INVALID",
             label, mype, want);
    passed = false;
  } else {
    int team_mype = shmem_team_my_pe(team);
    log_info("[%s] Our PE in team: %d", label, team_mype);

    for (int i = 0; i < size; ++i) {
      int expected_world_pe = start + stride * i;
      log_info("[%s] Calling team_translate_pe(team PE %d -> world team)",
               label, i);
      int pe_in_world = shmem_team_translate_pe(team, i, SHMEM_TEAM_WORLD);
      log_info("[%s] Translation result: team PE %d -> world PE %d "
               "(expected %d)",
               label, i, pe_in_world, expected_world_pe);

      if (pe_in_world != expected_world_pe) {
        log_fail("[%s] Translation failed! Expected world PE %d, got %d", label,
                 expected_world_pe, pe_in_world);
        passed = false;
      }
    }

    int back = shmem_team_translate_pe(SHMEM_TEAM_WORLD, mype, team);
    log_info("[%s] Reverse translation: world PE %d -> team PE %d "
             "(expected %d)",
             label, mype, back, want);
    if (back != want) {
      log_fail("[%s] Reverse translation failed! Expected team PE %d, got %d",
               label, want, back);
      passed = false;
    }

    for (int pe = 0; pe < shmem_n_pes(); ++pe) {
      bool in_team = false;
      for (int i = 0; i < size; ++i) {
        if (start + stride * i == pe) {
          in_team = true;
          break;
        }
      }
      if (in_team) {
        continue;
      }
      int outsider = shmem_team_translate_pe(SHMEM_TEAM_WORLD, pe, team);
      log_info("[%s] World PE %d is not in the team, translation returned %d",
               label, pe, outsider);
      if (outsider != -1) {
        log_fail("[%s] World PE %d is not a member, expected -1, got %d", label,
                 pe, outsider);
        passed = false;
      }
    }

    if (team_mype != want) {
      log_fail("[%s] shmem_team_my_pe() returned %d, expected %d", label,
               team_mype, want);
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

bool test_shmem_team_translate_pe(void) {
  log_routine("shmem_team_translate_pe()");
  int npes = shmem_n_pes();
  bool passed = true;

  if (!check_translate(0, 1, npes, "all PEs, stride 1")) {
    passed = false;
  }

  if (!check_translate(0, 2, (npes + 1) / 2, "even PEs, stride 2")) {
    passed = false;
  }

  if (!check_translate(1, 2, npes / 2, "odd PEs, stride 2")) {
    passed = false;
  }

  if (!check_translate(npes - 1, -1, npes, "all PEs, stride -1")) {
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

  *result = test_shmem_team_translate_pe();
  int rc = *result ? EXIT_SUCCESS : EXIT_FAILURE;

  shmem_barrier_all();
  reduce_test_result("C shmem_team_translate_pe", result, false);
  shmem_barrier_all();

  log_info("Test completed with %s", *result ? "SUCCESS" : "FAILURE");
  shmem_free(result);

  log_close(rc);
  shmem_finalize();
  return rc;
}
