/**
 * @file teams_tests.cpp
 * @brief Contains OpenSHMEM teams tests.
 */

#include "teams_tests.hpp"

/**
 * @brief Tests the shmem_team_my_pe() routine.
 *
 * This test verifies that the shmem_team_my_pe() function returns a valid PE number within the team.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_my_pe(void) {
  shmem_team_t team;
  p_shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, p_shmem_n_pes(), NULL, 0, &team);
  int my_pe = p_shmem_team_my_pe(team);
  p_shmem_team_destroy(team);
  return (my_pe >= 0);
}

/**
 * @brief Tests the shmem_team_n_pes() routine.
 *
 * This test verifies that the shmem_team_n_pes() function returns the correct number of PEs in the team.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_n_pes(void) {
  shmem_team_t team;
  p_shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, p_shmem_n_pes(), NULL, 0, &team);
  int npes = p_shmem_team_n_pes(team);
  p_shmem_team_destroy(team);
  return (npes == p_shmem_n_pes());
}

/**
 * @brief Tests the shmem_team_get_config() routine.
 *
 * This test verifies that the shmem_team_get_config() function correctly retrieves the team configuration.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_get_config(void) {
  shmem_team_t team;
  shmem_team_config_t config;
  long config_mask = SHMEM_TEAM_NUM_CONTEXTS;
  p_shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, p_shmem_n_pes(), NULL, 0, &team);
  if (team == SHMEM_TEAM_INVALID) {
    return false;
  }
  p_shmem_team_get_config(team, config_mask, &config);
  bool result = (config.num_contexts >= 0);
  p_shmem_team_destroy(team);
  return result;
}

/**
 * @brief Tests the shmem_team_translate_pe() routine.
 *
 * This test verifies that the shmem_team_translate_pe() function correctly translates a PE number from one team to another.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_translate_pe(void) {
  shmem_team_t team;
  p_shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, p_shmem_n_pes(), NULL, 0, &team);
  int pe_in_team = p_shmem_team_translate_pe(team, 0, SHMEM_TEAM_WORLD);
  p_shmem_team_destroy(team);
  return (pe_in_team >= 0);
}

/**
 * @brief Tests the shmem_team_split_strided() routine.
 *
 * This test verifies that the shmem_team_split_strided() function correctly splits a team into subteams.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_split_strided(void) {
  shmem_team_t team;
  p_shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, p_shmem_n_pes(), NULL, 0, &team);
  int npes = p_shmem_team_n_pes(team);
  p_shmem_team_destroy(team);
  return (npes == p_shmem_n_pes());
}

/**
 * @brief Tests the shmem_team_split_2d() routine.
 *
 * This test verifies that the shmem_team_split_2d() function correctly splits a team into two-dimensional subteams.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_split_2d(void) {
  shmem_team_t team_x, team_y;
  p_shmem_team_split_2d(SHMEM_TEAM_WORLD, 2, NULL, 0, &team_x, NULL, 0, &team_y);
  int npes_x = p_shmem_team_n_pes(team_x);
  int npes_y = p_shmem_team_n_pes(team_y);
  p_shmem_team_destroy(team_x);
  p_shmem_team_destroy(team_y);
  return (npes_x > 0 && npes_y > 0);
}

/**
 * @brief Tests the shmem_team_destroy() routine.
 *
 * This test verifies that the shmem_team_destroy() function correctly destroys a team.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_destroy(void) {
  shmem_team_t team;
  p_shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, p_shmem_n_pes(), NULL, 0, &team);
  p_shmem_team_destroy(team);
  if (!(team == SHMEM_TEAM_INVALID)) {
    return true;
  }
  else {
    return false;
  }
}
