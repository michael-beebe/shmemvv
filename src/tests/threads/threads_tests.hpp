/**
  @file threads _tests.hpp
  @brief Contains function declarations for the OpenSHMEM threads tests.
 */

#ifndef THREADS_TESTS_HPP
#define THREADS_TESTS_HPP

#include "routines.hpp"

#include <shmem.h>
#include <iostream>

bool test_shmem_init_thread(void);
bool test_shmem_query_thread(void);

#endif /* THREADS_TESTS_HPP */
