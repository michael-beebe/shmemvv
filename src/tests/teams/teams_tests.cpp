/**
 * @file cxx_teams_tests.cpp
 * @brief Contains OpenSHMEM teams tests.
 */

#include "teams_tests.h"

/**
 * @brief Tests the shmem_team_my_pe() routine.
 *
 * This test verifies that the shmem_team_my_pe() function returns a valid PE
 * number within the team.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_my_pe(void) {
  shmem_team_t team;
  shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(), NULL, 0,
                           &team);
  int my_pe = shmem_team_my_pe(team);
  shmem_team_destroy(team);
  return (my_pe >= 0);
}

/**
 * @brief Tests the shmem_team_n_pes() routine.
 *
 * This test verifies that the shmem_team_n_pes() function returns the correct
 * number of PEs in the team.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_n_pes(void) {
  shmem_team_t team;
  shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(), NULL, 0,
                           &team);
  int npes = shmem_team_n_pes(team);
  shmem_team_destroy(team);
  return (npes == shmem_n_pes());
}

/**
 * @brief Tests the shmem_team_get_config() routine.
 *
 * This test verifies that the shmem_team_get_config() function correctly
 * retrieves the team configuration.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_get_config(void) {
  shmem_team_t team;
  shmem_team_config_t config;
  long config_mask = SHMEM_TEAM_NUM_CONTEXTS;
  shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(), NULL, 0,
                           &team);
  if (team == SHMEM_TEAM_INVALID) {
    return false;
  }
  shmem_team_get_config(team, config_mask, &config);
  bool result = (config.num_contexts >= 0);
  shmem_team_destroy(team);
  return result;
}

/**
 * @brief Tests the shmem_team_translate_pe() routine.
 *
 * This test verifies that the shmem_team_translate_pe() function correctly
 * translates a PE number from one team to another.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_translate_pe(void) {
  shmem_team_t team;
  shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(), NULL, 0,
                           &team);
  int pe_in_team = shmem_team_translate_pe(team, 0, SHMEM_TEAM_WORLD);
  shmem_team_destroy(team);
  return (pe_in_team >= 0);
}

/**
 * @brief Tests the shmem_team_split_strided() routine.
 *
 * This test verifies that the shmem_team_split_strided() function correctly
 * splits a team into subteams.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_split_strided(void) {
  shmem_team_t team;
  shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(), NULL, 0,
                           &team);
  int npes = shmem_team_n_pes(team);
  shmem_team_destroy(team);
  return (npes == shmem_n_pes());
}

/**
 * @brief Tests the shmem_team_split_2d() routine.
 *
 * This test verifies that the shmem_team_split_2d() function correctly splits a
 * team into two-dimensional subteams.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_split_2d(void) {
  shmem_team_t team_x, team_y;
  shmem_team_split_2d(SHMEM_TEAM_WORLD, 2, NULL, 0, &team_x, NULL, 0, &team_y);
  int npes_x = shmem_team_n_pes(team_x);
  int npes_y = shmem_team_n_pes(team_y);
  shmem_team_destroy(team_x);
  shmem_team_destroy(team_y);
  return (npes_x > 0 && npes_y > 0);
}

/**
 * @brief Tests the shmem_team_destroy() routine.
 *
 * This test verifies that the shmem_team_destroy() function correctly destroys
 * a team.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_destroy(void) {
  shmem_team_t team;
  shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(), NULL, 0,
                           &team);
  shmem_team_destroy(team);
  if (!(team == SHMEM_TEAM_INVALID)) {
    return true;
  } else {
    return false;
  }
}

/**
 * @brief Tests the shmem_team_sync() routine.
 *
 * This test verifies that the shmem_team_sync() routine correctly synchronizes
 * all PEs within a team.
 *
 * @return True if the test is successful, false otherwise.
 */
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

/**
 * @brief Run all teams tests
 */
void run_teams_tests(int mype, int npes) {
  /* Run shmem_team_sync() test */
  bool result_shmem_team_sync = test_shmem_team_sync();
  shmem_barrier_all();
  if (mype == 0) {
    display_test_result("shmem_team_sync()", result_shmem_team_sync, false);
  }

  /* Run shmem_team_my_pe() test */
  shmem_barrier_all();
  bool result_shmem_team_my_pe = test_shmem_team_my_pe();
  shmem_barrier_all();
  if (mype == 0) {
    display_test_result("shmem_team_my_pe()", result_shmem_team_my_pe, false);
  }

  /* Run shmem_team_n_pes() test */
  shmem_barrier_all();
  bool result_shmem_team_n_pes = test_shmem_team_n_pes();
  shmem_barrier_all();
  if (mype == 0) {
    display_test_result("shmem_team_n_pes()", result_shmem_team_n_pes, false);
  }

  /* Run shmem_team_get_config() test */
  shmem_barrier_all();
  bool result_shmem_team_get_config = test_shmem_team_get_config();
  shmem_barrier_all();
  if (mype == 0) {
    display_test_result("shmem_team_get_config()", result_shmem_team_get_config,
                        false);
  }

  /* Run shmem_team_translate_pe() test */
  shmem_barrier_all();
  bool result_shmem_team_translate_pe = test_shmem_team_translate_pe();
  shmem_barrier_all();
  if (mype == 0) {
    display_test_result("shmem_team_translate_pe()",
                        result_shmem_team_translate_pe, false);
  }

  /* Run shmem_team_split_strided() test */
  shmem_barrier_all();
  bool result_shmem_team_split_strided = test_shmem_team_split_strided();
  shmem_barrier_all();
  if (mype == 0) {
    display_test_result("shmem_team_split_strided()",
                        result_shmem_team_split_strided, false);
  }

  /* Run shmem_team_split_2d() test */
  shmem_barrier_all();
  bool result_shmem_team_split_2d = test_shmem_team_split_2d();
  shmem_barrier_all();
  if (mype == 0) {
    display_test_result("shmem_team_split_2d()", result_shmem_team_split_2d,
                        false);
  }

  /* Run shmem_team_destroy() test */
  shmem_barrier_all();
  bool result_shmem_team_destroy = test_shmem_team_destroy();
  shmem_barrier_all();
  if (mype == 0) {
    display_test_result("shmem_team_destroy()", result_shmem_team_destroy,
                        false);
  }
}
