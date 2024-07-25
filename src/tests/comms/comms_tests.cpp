/**
 * @file comms_tests.cpp
 * @brief Contains OpenSHMEM communication/context tests.
 */

#include "comms_tests.hpp"

/**
 * @brief Tests the shmem_ctx_create() function.
 *
 * This test verifies that the shmem_ctx_create() function correctly creates a communication context.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_ctx_create(void) {
  shmem_ctx_t ctx;
  int ret = p_shmem_ctx_create(0, &ctx);
  if (ret != 0) {
    return false;
  }
  p_shmem_ctx_destroy(ctx);
  return true;
}

/**
 * @brief Tests the shmem_team_create_ctx() function.
 *
 * This test verifies that the shmem_team_create_ctx() function correctly creates a context
 * for a specified team.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_create_ctx(void) {
  shmem_team_t team;
  shmem_ctx_t ctx;
  p_shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, p_shmem_n_pes(), NULL, 0, &team);
  int ret = shmem_team_create_ctx(team, 0, &ctx);
  if (ret != 0) {
    return false;
  }
  p_shmem_ctx_destroy(ctx);
  p_shmem_team_destroy(team);
  return true;
}

/**
 * @brief Tests the shmem_ctx_destroy() function.
 *
 * This test verifies that the shmem_ctx_destroy() function correctly destroys a communication context.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_ctx_destroy(void) {
  shmem_ctx_t ctx;
  p_shmem_ctx_create(0, &ctx);
  p_shmem_ctx_destroy(ctx);
  return true;
}

/**
 * @brief Tests the shmem_ctx_get_team() function.
 *
 * This test verifies that the shmem_ctx_get_team() function correctly retrieves the team
 * associated with a given communication context.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_ctx_get_team(void) {
  shmem_ctx_t ctx;
  shmem_team_t team;
  p_shmem_ctx_create(0, &ctx);
  int ret = p_shmem_ctx_get_team(ctx, &team);
  p_shmem_ctx_destroy(ctx);
  return (ret == 0 && team == SHMEM_TEAM_WORLD);
}
