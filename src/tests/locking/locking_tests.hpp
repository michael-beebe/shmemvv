/**
 * @file locking_tests.hpp
 * @brief Contains function declarations for the OpenSHMEM distributed locking tests.
 */

#ifndef LOCKING_TESTS_HPP
#define LOCKING_TESTS_HPP

#include "routines.hpp"
#include <shmem.h>
#include <iostream>
#include <cstdlib>
#include <cstring>

/**
 * @brief Tests the shmem_set_lock() and shmem_clear_lock() routines.
 *
 * This test verifies that the shmem_set_lock() and shmem_clear_lock() routines correctly
 * set and clear a distributed lock. It ensures that the lock is properly set by one PE
 * and the state is correctly observed by another PE.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_lock_unlock(void);

#endif /* LOCKING_TESTS_HPP */
