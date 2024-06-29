// /**
//   @file teams_tests.cpp
//   @brief Contains OpenSHMEM teams tests.
//  */

// #include "teams_tests.hpp"

// /**
//   @brief Tests the shmem_team_my_pe() routine.
//   @return True if the test is successful, false otherwise.
//  */
// bool test_shmem_team_my_pe(void) {
//   shmem_team_t team;
//   shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(), NULL, 0, &team);
//   int my_pe = shmem_team_my_pe(team);
//   shmem_team_destroy(team);
//   return (my_pe >= 0);
// }

// /**
//   @brief Tests the shmem_team_n_pes() routine.
//   @return True if the test is successful, false otherwise.
//  */
// bool test_shmem_team_n_pes(void) {
//   shmem_team_t team;
//   shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(), NULL, 0, &team);
//   int npes = shmem_team_n_pes(team);
//   shmem_team_destroy(team);
//   return (npes == shmem_n_pes());
// }

// /**
//   @brief Tests the shmem_team_config_t structure and related routines.
//   @return True if the test is successful, false otherwise.
//  */
// bool test_shmem_team_config_t(void) {
//   shmem_team_config_t config;
//   config.num_contexts = 2;
//   return (config.num_contexts == 2);
// }

// /**
//   @brief Tests the shmem_team_get_config() routine.
//   @return True if the test is successful, false otherwise.
//  */
// bool test_shmem_team_get_config(void) {
//   shmem_team_t team;
//   shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(), NULL, 0, &team);
//   shmem_team_config_t config;
//   shmem_team_get_config(team, SHMEM_TEAM_NUM_CONTEXTS, &config);
//   shmem_team_destroy(team);
//   return (config.num_contexts >= 0);
// }

// /**
//   @brief Tests the shmem_team_translate_pe() routine.
//   @return True if the test is successful, false otherwise.
//  */
// bool test_shmem_team_translate_pe(void) {
//   shmem_team_t team;
//   shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(), NULL, 0, &team);
//   int pe_in_team = shmem_team_translate_pe(team, 0, SHMEM_TEAM_WORLD);
//   shmem_team_destroy(team);
//   return (pe_in_team >= 0);
// }

// /**
//   @brief Tests the shmem_team_split_strided() routine.
//   @return True if the test is successful, false otherwise.
//  */
// bool test_shmem_team_split_strided(void) {
//   shmem_team_t team;
//   shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(), NULL, 0, &team);
//   int npes = shmem_team_n_pes(team);
//   shmem_team_destroy(team);
//   return (npes == shmem_n_pes());
// }

// /**
//   @brief Tests the shmem_team_split_2d() routine.
//   @return True if the test is successful, false otherwise.
//  */
// bool test_shmem_team_split_2d(void) {
//   shmem_team_t team_x, team_y;
//   shmem_team_split_2d(SHMEM_TEAM_WORLD, 2, NULL, 0, &team_x, NULL, 0, &team_y);
//   int npes_x = shmem_team_n_pes(team_x);
//   int npes_y = shmem_team_n_pes(team_y);
//   shmem_team_destroy(team_x);
//   shmem_team_destroy(team_y);
//   return (npes_x > 0 && npes_y > 0);
// }

// /**
//   @brief Tests the shmem_team_destroy() routine.
//   @return True if the test is successful, false otherwise.
//  */
// bool test_shmem_team_destroy(void) {
//   shmem_team_t team;
//   shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(), NULL, 0, &team);

//   if (team == SHMEM_TEAM_INVALID) {
//     return false;  /* Failed to create a valid team */
//   }

//   int ret = shmem_team_destroy(team);
//   return (ret == 0);  /* 0 indicates success */
// }














/**
  @file teams_tests.cpp
  @brief Contains OpenSHMEM teams tests.
 */

#include "teams_tests.hpp"

/**
  @brief Tests the shmem_team_my_pe() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_my_pe(void) {
  shmem_team_t team;
  shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(), NULL, 0, &team);
  int my_pe = shmem_team_my_pe(team);
  shmem_team_destroy(team);
  return (my_pe >= 0);
}

/**
  @brief Tests the shmem_team_n_pes() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_n_pes(void) {
  shmem_team_t team;
  shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(), NULL, 0, &team);
  int npes = shmem_team_n_pes(team);
  shmem_team_destroy(team);
  return (npes == shmem_n_pes());
}

/**
  @brief Tests the shmem_team_config_t structure and related routines.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_config_t(void) {
  shmem_team_config_t config;
  config.num_contexts = 2;
  return (config.num_contexts == 2);
}

/**
  @brief Tests the shmem_team_get_config() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_get_config(void) {
  shmem_team_t team;
  shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(), NULL, 0, &team);
  shmem_team_config_t config;
  shmem_team_get_config(team, &config);
  shmem_team_destroy(team);
  return (config.num_contexts >= 0);
}

/**
  @brief Tests the shmem_team_translate_pe() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_translate_pe(void) {
  shmem_team_t team;
  shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(), NULL, 0, &team);
  int pe_in_team = shmem_team_translate_pe(team, 0, SHMEM_TEAM_WORLD);
  shmem_team_destroy(team);
  return (pe_in_team >= 0);
}

/**
  @brief Tests the shmem_team_split_strided() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_split_strided(void) {
  shmem_team_t team;
  shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(), NULL, 0, &team);
  int npes = shmem_team_n_pes(team);
  shmem_team_destroy(team);
  return (npes == shmem_n_pes());
}

/**
  @brief Tests the shmem_team_split_2d() routine.
  @return True if the test is successful, false otherwise.
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
  @brief Tests the shmem_team_destroy() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_destroy(void) {
  shmem_team_t team;
  shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(), NULL, 0, &team);

  if (team == SHMEM_TEAM_INVALID) {
    return false;  /* Failed to create a valid team */
  }

  int ret = shmem_team_destroy(team);
  return (ret == 0);  /* 0 indicates success */
}

