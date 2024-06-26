/**
  @file teams_tests.hpp
  @brief Contains function declarations for the OpenSHMEM teams tests.
 */

#ifndef TEAMS_TESTS_HPP
#define TEAMS_TESTS_HPP

#include <shmem.h>
#include <iostream>

/**
 * @brief Tests the shmem_team_my_pe() function.
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_my_pe(void);

/**
 * @brief Tests the shmem_team_n_pes() function.
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_n_pes(void);

/**
 * @brief Tests the shmem_team_config_t structure and related functions.
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_config_t(void);

/**
 * @brief Tests the shmem_team_get_config() function.
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_get_config(void);

/**
 * @brief Tests the shmem_team_translate_pe() function.
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_translate_pe(void);

/**
 * @brief Tests the shmem_team_split_strided() function.
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_split_strided(void);

/**
 * @brief Tests the shmem_team_split_2d() function.
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_split_2d(void);

/**
 * @brief Tests the shmem_team_destroy() function.
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_destroy(void);

#endif /* TEAMS_TESTS_HPP */
