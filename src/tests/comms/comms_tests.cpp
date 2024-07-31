/**
 * @file cxx_comms_tests.cpp
 * @brief Contains OpenSHMEM communication/context tests.
 */

#include "comms_tests.h"

/**
 * @brief Tests the shmem_ctx_create() function.
 *
 * This test verifies that the shmem_ctx_create() function correctly creates a communication context.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_ctx_create(void) {
  shmem_ctx_t ctx;
  int ret = shmem_ctx_create(0, &ctx);
  if (ret != 0) {
    return false;
  }
  shmem_ctx_destroy(ctx);
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
  shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 1, shmem_n_pes(), NULL, 0, &team);
  int ret = shmem_team_create_ctx(team, 0, &ctx);
  if (ret != 0) {
    return false;
  }
  shmem_ctx_destroy(ctx);
  shmem_team_destroy(team);
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
  shmem_ctx_create(0, &ctx);
  shmem_ctx_destroy(ctx);
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
  shmem_ctx_create(0, &ctx);
  int ret = shmem_ctx_get_team(ctx, &team);
  shmem_ctx_destroy(ctx);
  return (ret == 0 && team == SHMEM_TEAM_WORLD);
}

/**
 * TODO: write docs
 * 
 */
void run_comms_tests(int mype, int npes) {
  shmem_barrier_all();
  if (mype == 0) {
    display_test_header("COMMUNICATION / CONTEXT"); 
  }

  /* Run shmem_ctx_create() test */
  shmem_barrier_all();
  if (!check_if_exists("shmem_ctx_create")) {
    if (mype == 0) {
      display_not_found_warning("shmem_ctx_create()", false);
    }
  }
  else {
    bool result_shmem_ctx_create = test_shmem_ctx_create();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("shmem_ctx_create()", result_shmem_ctx_create, false);
    }
  }

  /* Run shmem_team_create_ctx() test */
  shmem_barrier_all();
  if (!check_if_exists("shmem_team_create_ctx")) {
    if (mype == 0) {
      display_not_found_warning("shmem_team_create_ctx()", false);
    }
  }
  else {
    bool result_shmem_team_create_ctx = test_shmem_team_create_ctx();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("shmem_team_create_ctx()", result_shmem_team_create_ctx, false);
    }
  }

  /* Run shmem_ctx_destroy() test */
  shmem_barrier_all();
  if (!check_if_exists("shmem_ctx_destroy")) {
    if (mype == 0) {
      display_not_found_warning("shmem_ctx_destroy()", false);
    }
  }
  else {
    bool result_shmem_ctx_destroy = test_shmem_ctx_destroy();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("shmem_ctx_destroy()", result_shmem_ctx_destroy, false);
    }
  }

  /* Run shmem_ctx_get_team() test */
  shmem_barrier_all();
  if (!check_if_exists("shmem_ctx_get_team")) {
    if (mype == 0) {
      display_not_found_warning("shmem_ctx_get_team()", false);
    }
  }
  else {
    bool result_shmem_ctx_get_team = test_shmem_ctx_get_team();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("shmem_ctx_get_team()", result_shmem_ctx_get_team, false);
    }
  }
}
