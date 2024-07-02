/**
  @file mem_ordering_tests.hpp
  @brief Contains function declarations for the OpenSHMEM memory ordering tests.
 */

#ifndef MEM_ORDERING_TESTS_HPP
#define MEM_ORDERING_TESTS_HPP

#include <shmem.h>
#include <iostream>
#include <cstdlib>
#include <cstring>

/**
  @brief Tests the shmem_fence() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_fence(void);

/**
  @brief Tests the shmem_quiet() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_quiet(void);

#endif /* MEM_ORDERING_TESTS_HPP */
