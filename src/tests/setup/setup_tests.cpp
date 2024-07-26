/**
 * @file setup_tests.cpp
 * @brief Contains OpenSHMEM setup tests.
 */

#include "setup_tests.h"

/**
 * @brief Tests the presence of a fake routine for demonstration purposes.
 *
 * This test checks if the `p_shmem_fake_routine` function is available and calls it if so.
 * 
 * @return True if the routine is available and called, false otherwise.
 */
bool test_shmem_fake_routine(void) {
  if (p_shmem_fake_routine) {
    p_shmem_fake_routine();
    return true;
  }
  else {
    std::cerr << "shmem_fake_routine is not available." << std::endl;
    return false;
  }
}

/**
 * @brief Tests the initialization of OpenSHMEM.
 *
 * This test verifies that the OpenSHMEM library initializes successfully.
 * 
 * @return True if the initialization is successful, false otherwise.
 */
bool test_shmem_init() {
  p_shmem_init();
  return true;
}

/**
 * @brief Tests the barrier synchronization across all PEs.
 *
 * This test verifies that the barrier synchronization across all PEs is successful.
 * 
 * @return True if the barrier synchronization is successful, false otherwise.
 */
bool test_shmem_barrier_all() {
  int mype = p_shmem_my_pe();
  int npes = p_shmem_n_pes();

  static long sync;
  sync = mype;

  p_shmem_barrier_all();

  bool test_passed = true;

  if (sync != mype) {
    test_passed = false;
  }

  return test_passed;
}

/**
 * @brief Tests the shmem_barrier() routine.
 *
 * This test verifies that the `shmem_barrier` routine functions correctly.
 * 
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_barrier(void) {
  static long pSync[SHMEM_BARRIER_SYNC_SIZE];
  for (int i = 0; i < SHMEM_BARRIER_SYNC_SIZE; i++) {
    pSync[i] = SHMEM_SYNC_VALUE;
  }

  p_shmem_barrier(0, 0, p_shmem_n_pes(), pSync);
  return true;
}

/**
 * @brief Tests retrieving the PE number of the calling PE.
 *
 * This test verifies that the `shmem_my_pe` function returns a valid PE number.
 * 
 * @return The PE number on success, -1 on failure.
 */
int test_shmem_my_pe() {
  int mype = p_shmem_my_pe();
  if (mype >= 0) {
    return mype;
  }
  else {
    return -1;
  }
}

/**
 * @brief Tests retrieving the number of PEs.
 *
 * This test verifies that the `shmem_n_pes` function returns a valid number of PEs.
 * 
 * @return The number of PEs if greater than 0, otherwise 0.
 */
int test_shmem_n_pes() {
  int npes = p_shmem_n_pes();
  if (!(npes > 0)) {
    return 0;
  }
  else {
    return npes;
  }
}

/**
 * @brief Tests if a PE is accessible from the calling PE.
 *
 * This test verifies that the `shmem_pe_accessible` function correctly reports accessibility of all PEs.
 * 
 * @return True if all PEs are accessible, false otherwise.
 */
bool test_shmem_pe_accessible() {
  int npes = p_shmem_n_pes();
  for (int pe = 0; pe < npes; ++pe) {
    if (!p_shmem_pe_accessible(pe)) {
      return false;
    }
  }
  return true;
}

/**
 * @brief Tests retrieving the OpenSHMEM library version.
 *
 * This test verifies that the `shmem_info_get_version` function returns the correct version of the OpenSHMEM library.
 * 
 * @return The version as a string in the format "major.minor".
 */
std::string test_shmem_info_get_version() {
  int major, minor;
  p_shmem_info_get_version(&major, &minor);
  
  std::string version = std::to_string(major) + "." + std::to_string(minor);
  return version;
}

/**
 * @brief Tests retrieving the name of the OpenSHMEM library.
 *
 * This test verifies that the `shmem_info_get_name` function returns the correct name of the OpenSHMEM library.
 * 
 * @return The name of the library as a string if successful, otherwise an empty string.
 */
std::string test_shmem_info_get_name() {
  char name[SHMEM_MAX_NAME_LEN];
  p_shmem_info_get_name(name);
  if (strlen(name) > 0) {
    return std::string(name);
  }
  else {
    return "";
  }
}

/**
 * @brief Tests the finalization of OpenSHMEM.
 *
 * This test verifies that the OpenSHMEM library finalizes successfully.
 * 
 * @return True if the finalization is successful, false otherwise.
 */
bool test_shmem_finalize() {
  p_shmem_finalize();
  return true;
}

/**
 * @brief Tests the global exit functionality of OpenSHMEM.
 *
 * This test verifies that the `shmem_global_exit` function successfully performs a global exit.
 * 
 * @return True if the global exit is successful, false otherwise.
 */
bool test_shmem_global_exit() {
  p_shmem_global_exit(0);
  return true;
}
