/**
  @file mem_tests.hpp
  @brief Contains function declarations for the OpenSHMEM memory management tests.
 */

#ifndef MEM_TESTS_HPP
#define MEM_TESTS_HPP

#include "routines.hpp"

#include <shmem.h>
#include <iostream>
#include <cstdlib>
#include <cstring>

/**
  @brief Tests the shmem_ptr() routine.
  @return True if the pointer is accessible, false otherwise.
 */
bool test_shmem_ptr(void);

/**
  @brief Tests the shmem_malloc() and shmem_free() functions.
  @return True if the tests are successful, false otherwise.
 */
bool test_shmem_malloc_free(void);

/**
  @brief Tests the shmem_realloc() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_realloc(void);

/**
  @brief Tests the shmem_align() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_align(void);

/**
  @brief Tests the shmem_malloc_with_hints() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_malloc_with_hints(void);

/**
  @brief Tests the shmem_calloc() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_calloc(void);

#endif /* MEM_TESTS_HPP */
