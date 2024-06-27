/**
  @file comms_tests.hpp
  @brief Contains function declarations for the OpenSHMEM communication/context tests.
 */

#ifndef COMMS_TESTS_HPP
#define COMMS_TESTS_HPP

#include <shmem.h>
#include <iostream>

/**
  @brief Tests the shmem_ctx_create() function.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_ctx_create(void);

/**
  @brief Tests the shmem_team_create_ctx() function.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_team_create_ctx(void);

/**
  @brief Tests the shmem_ctx_destroy() function.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_ctx_destroy(void);

/**
  @brief Tests the shmem_ctx_get_team() function.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_ctx_get_team(void);

#endif /* COMMS_TESTS_HPP */
