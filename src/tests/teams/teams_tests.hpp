/**
  @file teams_tests.hpp
  @brief Contains function declarations for the OpenSHMEM teams tests.
 */

#ifndef TEAMS_TESTS_HPP
#define TEAMS_TESTS_HPP

#include <shmem.h>
#include <iostream>

/**
 * @brief Tests the shmem_team_my_pe() routine.
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_my_pe(void);

/**
 * @brief Tests the shmem_team_n_pes() routine.
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_n_pes(void);

/**
 * @brief Tests the shmem_team_get_config() routine.
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_get_config(void);

/**
 * @brief Tests the shmem_team_translate_pe() routine.
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_translate_pe(void);

/**
 * @brief Tests the shmem_team_split_strided() routine.
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_split_strided(void);

/**
 * @brief Tests the shmem_team_split_2d() routine.
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_split_2d(void);

/**
 * @brief Tests the shmem_team_destroy() routine.
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_destroy(void);

#endif /* TEAMS_TESTS_HPP */
