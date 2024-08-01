/**
 * @file teams_tests.h
 * @brief Contains function declarations for the OpenSHMEM teams tests.
 */

#ifndef TEAMS_TESTS_H
#define TEAMS_TESTS_H

#include "routines.h"
#include "shmemvv.h"

#include <shmem.h>
#include <stdlib.h>

/**
 * @brief Tests the shmem_team_my_pe() routine.
 *
 * This test verifies that the shmem_team_my_pe() function returns a valid PE number within the team.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_my_pe(void);

/**
 * @brief Tests the shmem_team_n_pes() routine.
 *
 * This test verifies that the shmem_team_n_pes() function returns the correct number of PEs in the team.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_n_pes(void);

/**
 * @brief Tests the shmem_team_get_config() routine.
 *
 * This test verifies that the shmem_team_get_config() function correctly retrieves the team configuration.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_get_config(void);

/**
 * @brief Tests the shmem_team_translate_pe() routine.
 *
 * This test verifies that the shmem_team_translate_pe() function correctly translates a PE number from one team to another.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_translate_pe(void);

/**
 * @brief Tests the shmem_team_split_strided() routine.
 *
 * This test verifies that the shmem_team_split_strided() function correctly splits a team into subteams.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_split_strided(void);

/**
 * @brief Tests the shmem_team_split_2d() routine.
 *
 * This test verifies that the shmem_team_split_2d() function correctly splits a team into two-dimensional subteams.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_split_2d(void);

/**
 * @brief Tests the shmem_team_destroy() routine.
 *
 * This test verifies that the shmem_team_destroy() function correctly destroys a team.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_destroy(void);

/**
 * TODO: write docs
 * 
 */
void run_teams_tests(int mype, int npes);

#endif /* TEAMS_TESTS_H */
