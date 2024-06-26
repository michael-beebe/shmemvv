/**
  @file mem_tests.hpp
  @brief Contains function declarations for the OpenSHMEM memory management tests.
 */

#ifndef MEM_TESTS_HPP
#define MEM_TESTS_HPP

#include <shmem.h>
#include <iostream>
#include <cstdlib>
#include <cstring>

/**
  @brief Tests if a pointer is accessible from a remote PE.
  @return True if the pointer is accessible, false otherwise.
 */
bool test_shmem_ptr(void);

/**
  @brief Tests the shmem_malloc() and shmem_free() functions.
  @return True if the tests are successful, false otherwise.
 */
bool test_shmem_malloc_free(void);

/**
  @brief Tests the shmem_realloc() function.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_realloc(void);

/**
  @brief Tests the shmem_align() function.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_align(void);

/**
  @brief Tests the shmem_malloc_with_hints() function.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_malloc_with_hints(void);

/**
  @brief Tests the shmem_calloc() function.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_calloc(void);

#endif /* MEM_TESTS_HPP */
