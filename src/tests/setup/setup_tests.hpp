/**
  @file setup_tests.hpp
  @brief Contains function declarations for the OpenSHMEM setup tests.
 */

#ifndef SETUP_TESTS_HPP
#define SETUP_TESTS_HPP

#include <shmem.h>
#include <iostream>
#include <cstring>
#include <string>

/**
  @brief Tests the initialization of OpenSHMEM.
  @return True if the initialization is successful, false otherwise.
 */
bool test_shmem_init(void);

/**
  @brief Tests the barrier synchronization across all PEs.
  @return True if the barrier synchronization is successful, false otherwise.
 */
bool test_shmem_barrier_all(void);

/**
  @brief Tests retrieving the PE number of the calling PE.
  @return The PE number on success, -1 on failure.
 */
int test_shmem_my_pe(void);

/**
  @brief Tests retrieving the number of PEs.
  @return The number of PEs if greater than 0, otherwise 0.
 */
int test_shmem_n_pes(void);

/**
  @brief Tests if a PE is accessible from the calling PE.
  @return True if all PEs are accessible, false otherwise.
 */
bool test_shmem_pe_accessible(void);

/**
  @brief Tests if a pointer is accessible from a remote PE.
  @return True if the pointer is accessible, false otherwise.
 */
bool test_shmem_ptr(void);

/**
  @brief Tests retrieving the OpenSHMEM library version.
  @return The version as a string in the format "major.minor".
 */
std::string test_shmem_info_get_version(void);

/**
  @brief Tests retrieving the name of the OpenSHMEM library.
  @return The name of the library as a string if successful, otherwise an empty string.
 */
std::string test_shmem_info_get_name(void);

/**
  @brief Tests the finalization of OpenSHMEM.
  @return True if the finalization is successful, false otherwise.
 */
bool test_shmem_finalize(void);

/**
  @brief Tests the global exit functionality of OpenSHMEM.
  @return True if the global exit is successful, false otherwise.
 */
bool test_shmem_global_exit(void);

// /**
//   @brief Performs the setup for the test suite.
//   @param argc Number of command-line arguments.
//   @param argv Array of command-line argument strings.
//   @param mype Reference to the PE number.
//   @param npes Reference to the number of PEs.
//   @param version Reference to the string that will hold the version.
//   @param name Reference to the string that will hold the name.
//   @return EXIT_SUCCESS on success, EXIT_FAILURE on failure.
//  */
// int perform_setup(int argc, char *argv[], int &mype, int &npes, std::string &version, std::string &name);

#endif /* SETUP_TESTS_HPP */




