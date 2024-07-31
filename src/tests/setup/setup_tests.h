/**
 * @file setup_tests.h
 * @brief Contains function declarations for the OpenSHMEM setup tests.
 */

#ifndef SETUP_TESTS_H
#define SETUP_TESTS_H

#include "shmemvv.h"
#include "routines.h"

#include "tests.h"

#include <shmem.h>
#include <iostream>
#include <cstring>
#include <string>

/**
 * @brief Tests to make sure routines that aren't
 *        implemented in the tested OpenSHMEM library don't throw
 *        compiler errors.
 * 
 * This test verifies that calling an unimplemented routine in the
 * OpenSHMEM library does not result in compiler errors.
 * 
 * @return True if the routine does not throw compiler errors, false otherwise.
 */
bool text_shmem_fake_routine(void);

/**
 * @brief Tests the initialization of OpenSHMEM.
 *
 * This test verifies that the OpenSHMEM library initializes successfully.
 * 
 * @return True if the initialization is successful, false otherwise.
 */
bool text_shmem_init(void);

/**
 * @brief Tests the barrier synchronization across all PEs.
 *
 * This test verifies that the barrier synchronization across all PEs is successful.
 * 
 * @return True if the barrier synchronization is successful, false otherwise.
 */
bool text_shmem_barrier_all(void);

/**
 * @brief Tests the shmem_barrier() routine.
 *
 * This test verifies that the shmem_barrier() routine functions correctly.
 * 
 * @return True if the test is successful, false otherwise.
 */
bool text_shmem_barrier(void);

/**
 * @brief Tests retrieving the PE number of the calling PE.
 *
 * This test verifies that the shmem_my_pe() function returns a valid PE number.
 * 
 * @return The PE number on success, -1 on failure.
 */
int text_shmem_my_pe(void);

/**
 * @brief Tests retrieving the number of PEs.
 *
 * This test verifies that the shmem_n_pes() function returns a valid number of PEs.
 * 
 * @return The number of PEs if greater than 0, otherwise 0.
 */
int text_shmem_n_pes(void);

/**
 * @brief Tests if a PE is accessible from the calling PE.
 *
 * This test verifies that the shmem_pe_accessible() function correctly reports the accessibility of all PEs.
 * 
 * @return True if all PEs are accessible, false otherwise.
 */
bool text_shmem_pe_accessible(void);

/**
 * @brief Tests retrieving the OpenSHMEM library version.
 *
 * This test verifies that the shmem_info_get_version() function returns the correct version of the OpenSHMEM library.
 * 
 * @return The version as a string in the format "major.minor".
 */
std::string text_shmem_info_get_version(void);

/**
 * @brief Tests retrieving the name of the OpenSHMEM library.
 *
 * This test verifies that the shmem_info_get_name() function returns the correct name of the OpenSHMEM library.
 * 
 * @return The name of the library as a string if successful, otherwise an empty string.
 */
std::string text_shmem_info_get_name(void);

/**
 * @brief Tests the finalization of OpenSHMEM.
 *
 * This test verifies that the OpenSHMEM library finalizes successfully.
 * 
 * @return True if the finalization is successful, false otherwise.
 */
bool text_shmem_finalize(void);

/**
 * @brief Tests the global exit functionality of OpenSHMEM.
 *
 * This test verifies that the shmem_global_exit() function successfully performs a global exit.
 * 
 * @return True if the global exit is successful, false otherwise.
 */
bool text_shmem_global_exit(void);

/**
 * @brief Perform all setup tests
 * 
 * @return True if successful false otherwise
 */
bool run_setup_tests(test_options opts, int &mype, int &npes, std::string &version, std::string name);

#endif /* SETUP_TESTS_h */
