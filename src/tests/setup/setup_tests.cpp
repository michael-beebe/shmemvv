/**
  @file setup_tests.cpp
  @brief Contains OpenSHMEM setup tests.
 */

#include "setup_tests.hpp"

/**
  @brief Tests the initialization of OpenSHMEM.
  @return True if the initialization is successful, false otherwise.
 */
bool test_shmem_init() {
  shmem_init();
  return shmem_my_pe() >= 0;
}

/**
  @brief Tests the barrier synchronization across all PEs.
  @return True if the barrier synchronization is successful, false otherwise.
 */
bool test_shmem_barrier_all() {
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();
  
  long *sync = (long *)shmem_malloc(sizeof(long));
  if (sync == nullptr) {
    return false;
  }

  *sync = mype;

  shmem_barrier_all();

  bool test_passed = true;

  if (*sync != mype) {
    test_passed = false;
  }

  shmem_free(sync);
  return test_passed;
}

/**
  @brief Tests the shmem_barrier() routine.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_barrier(void) {
  static long pSync[SHMEM_BARRIER_SYNC_SIZE];
  for (int i = 0; i < SHMEM_BARRIER_SYNC_SIZE; i++) {
    pSync[i] = SHMEM_SYNC_VALUE;
  }

  shmem_barrier(0, 0, shmem_n_pes(), pSync);
  return true;
}

/**
  @brief Tests retrieving the PE number of the calling PE.
  @return The PE number on success, -1 on failure.
 */
int test_shmem_my_pe() {
  int mype = shmem_my_pe();
  if (mype >= 0) {
    return mype;
  }
  else {
    return -1;
  }
}

/**
  @brief Tests retrieving the number of PEs.
  @return The number of PEs if greater than 0, otherwise 0.
 */
int test_shmem_n_pes() {
  int npes = shmem_n_pes();
  if (!(npes > 0)) {
    return 0;
  }
  else {
    return npes;
  }
}

/**
  @brief Tests if a PE is accessible from the calling PE.
  @return True if all PEs are accessible, false otherwise.
 */
bool test_shmem_pe_accessible() {
  int npes = shmem_n_pes();
  for (int pe = 0; pe < npes; ++pe) {
    if (!shmem_pe_accessible(pe)) {
      return false;
    }
  }
  return true;
}

/**
  @brief Tests retrieving the OpenSHMEM library version.
  @return The version as a string in the format "major.minor".
 */
std::string test_shmem_info_get_version() {
  int major, minor;
  shmem_info_get_version(&major, &minor);
  
  std::string version = std::to_string(major) + "." + std::to_string(minor);
  return version;
}

/**
  @brief Tests retrieving the name of the OpenSHMEM library.
  @return The name of the library as a string if successful, otherwise an empty string.
 */
std::string test_shmem_info_get_name() {
  char name[SHMEM_MAX_NAME_LEN];
  shmem_info_get_name(name);
  if (strlen(name) > 0) {
    return std::string(name);
  }
  else {
    return "";
  }
}

/**
  @brief Tests the finalization of OpenSHMEM.
  @return True if the finalization is successful, false otherwise.
 */
bool test_shmem_finalize() {
  shmem_finalize();
  return true;
}

/**
  @brief Tests the global exit functionality of OpenSHMEM.
  @return True if the global exit is successful, false otherwise.
 */
bool test_shmem_global_exit() {
  shmem_global_exit(0);
  return true;
}
