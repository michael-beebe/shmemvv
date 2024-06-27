/**
  @file comms_tests.cpp
  @brief Contains OpenSHMEM communication/context tests.
 */

#include "comms_tests.hpp"

/**
  @brief Tests the shmem_ctx_create() function.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_ctx_create(void) {
  shmem_ctx_t ctx;
  int ret = shmem_ctx_create(0, &ctx);
  if (ret != 0) {
    return false;  /* Failed to create a context */
  }
  shmem_ctx_destroy(ctx);
  return true;  /* Context created and destroyed successfully */
}

/**
  @brief Tests the shmem_team_create_ctx() function.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_create_ctx(void) {
  shmem_team_t team;
  shmem_ctx_t ctx;
  shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(), NULL, 0, &team);
  int ret = shmem_team_create_ctx(team, 0, &ctx);
  if (ret != 0) {
    return false;  /* Failed to create a context from a team */
  }
  shmem_ctx_destroy(ctx);
  shmem_team_destroy(team);
  return true;  /* Context created and destroyed successfully */
}

/**
  @brief Tests the shmem_ctx_destroy() function.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_ctx_destroy(void) {
  shmem_ctx_t ctx;
  shmem_ctx_create(0, &ctx);
  shmem_ctx_destroy(ctx);
  return true;  /* Context destroyed successfully */
}

/**
  @brief Tests the shmem_ctx_get_team() function.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_ctx_get_team(void) {
  shmem_ctx_t ctx;
  shmem_team_t team;
  shmem_ctx_create(0, &ctx);
  int ret = shmem_ctx_get_team(ctx, &team);
  shmem_ctx_destroy(ctx);
  return (ret == 0 && team == SHMEM_TEAM_WORLD);  /* Expecting the team to be SHMEM_TEAM_WORLD */
}
