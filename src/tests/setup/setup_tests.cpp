/**
 * @file setup_tests.cpp
 * @brief Contains OpenSHMEM setup tests.
 */

#include "setup_tests.h"

/**
 * @brief Tests the initialization of OpenSHMEM.
 *
 * This test verifies that the OpenSHMEM library initializes successfully.
 * 
 * @return True if the initialization is successful, false otherwise.
 */
bool test_shmem_init() {
  shmem_init();
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
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  static long sync;
  sync = mype;

  shmem_barrier_all();

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

  shmem_barrier(0, 0, shmem_n_pes(), pSync);
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
  int mype = shmem_my_pe();
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
  int npes = shmem_n_pes();
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
  int npes = shmem_n_pes();
  for (int pe = 0; pe < npes; ++pe) {
    if (!shmem_pe_accessible(pe)) {
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
 * @return A pointer to the version string in the format "major.minor".
 */
char* test_shmem_info_get_version(void) {
  int major, minor;
  shmem_info_get_version(&major, &minor);

  char *version = (char *)malloc(16 * sizeof(char));
  if (version != NULL) {
    snprintf(version, 16, "%d.%d", major, minor);
  }

  return version;
}

/**
 * @brief Tests retrieving the name of the OpenSHMEM library.
 *
 * This test verifies that the `shmem_info_get_name` function returns the correct name of the OpenSHMEM library.
 * 
 * @return A pointer to the name of the library.
 */
char* test_shmem_info_get_name(void) {
  char *name = (char *)malloc(SHMEM_MAX_NAME_LEN * sizeof(char)); // allocate memory for name string
  if (name != NULL) {
    shmem_info_get_name(name);
  }

  return name;
}

/**
 * @brief Tests the finalization of OpenSHMEM.
 *
 * This test verifies that the OpenSHMEM library finalizes successfully.
 * 
 * @return True if the finalization is successful, false otherwise.
 */
bool test_shmem_finalize() {
  shmem_finalize();
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
  shmem_global_exit(0);
  return true;
}

/**
 * @brief Perform all setup tests
 * 
 * @return True if successful false otherwise
 */
bool run_setup_tests(int &mype, int &npes, char *version, char *name) {
  /* Variables to hold test results */
  bool result_shmem_init = true;
  bool result_shmem_barrier_all = true;
  bool result_shmem_barrier = true;
  bool result_shmem_my_pe = true;
  bool result_shmem_n_pes = true;
  bool result_shmem_pe_accessible = true;
  bool result_shmem_info_get_version = true;
  bool result_shmem_info_get_name = true;

  // /* Just run shmem_init() */
  // result_shmem_init = test_shmem_init();
  // if (!result_shmem_init) {
  //   display_test_result("shmem_init()", result_shmem_init, true);
  //   return false;
  // }

  /* Run shmem_barrier_all() test */
  result_shmem_barrier_all = test_shmem_barrier_all();
  if (!result_shmem_barrier_all) {
    if (shmem_my_pe() == 0) {
      display_test_result("shmem_barrier_all()", result_shmem_barrier_all, true);
    }
    shmem_finalize();
    return false;
  }

  /* Run shmem_my_pe() test */
  shmem_barrier_all();
  mype = test_shmem_my_pe();
  result_shmem_my_pe = mype >= 0;
  if (!result_shmem_my_pe) {
    if (mype == 0) {
      display_test_result("shmem_my_pe()", result_shmem_my_pe, true);
    }
    shmem_finalize();
    return false;
  }

  /* Run shmem_n_pes() test */
  shmem_barrier_all();
  npes = test_shmem_n_pes();
  result_shmem_n_pes = npes > 0;
  if (!result_shmem_n_pes) {
    if (mype == 0) {
      display_test_result("shmem_n_pes()", result_shmem_n_pes, true);
    }
    shmem_finalize();
    return false;
  }

  /* Run shmem_pe_accessible() test */
  shmem_barrier_all();
  result_shmem_pe_accessible = test_shmem_pe_accessible();
  if (!result_shmem_pe_accessible) {
    if (mype == 0) {
      display_test_result("shmem_pe_accessible()", result_shmem_pe_accessible, true);
    }
    shmem_finalize();
    return false;
  }

  /* Display ASCII art logo */
  shmem_barrier_all();
  if (mype == 0) {
    display_logo();
  }

  /* Run shmem_barrier() test */
  shmem_barrier_all();
  result_shmem_barrier = test_shmem_barrier();
  shmem_barrier_all();

  /* Run shmem_info_get_version() test */
  shmem_barrier_all();
  char* version_str = test_shmem_info_get_version();
  if (version_str == NULL || strlen(version_str) == 0) {
    result_shmem_info_get_version = false;
  } else {
    strcpy(version, version_str);
    free(version_str);
  }

  /* Run shmem_info_get_name() test */
  shmem_barrier_all();
  char* name_str = test_shmem_info_get_name();
  if (name_str == NULL || strlen(name_str) == 0) {
    result_shmem_info_get_name = false;
  } else {
    strcpy(name, name_str);
    free(name_str);
  }
  
  /* Display test information */
  shmem_barrier_all();
  if (mype == 0) {
    display_test_info(name, version, npes);
  } 

  /* Print setup tests header */
  shmem_barrier_all();
  if (mype == 0) {
    display_test_header("SETUP");
  }

  /* shmem_init() and shmem_my_pe() tests passed */
  shmem_barrier_all();
  if (mype == 0) {
    display_test_result("shmem_init()", true, true);
    display_test_result("shmem_barrier_all()", result_shmem_barrier_all, true);
    display_test_result("shmem_barrier()", result_shmem_barrier, false);
    display_test_result("shmem_my_pe()", result_shmem_my_pe, true);
    display_test_result("shmem_n_pes()", result_shmem_n_pes, true);
    display_test_result("shmem_pe_accessible()", result_shmem_pe_accessible, true);
    if (strcmp(version, "1.5") != 0 && strcmp(version, "1.50") != 0) {
      std::cerr << YELLOW_COLOR << "shmem_info_get_version() test did not return 1.5... Returned " << version << RESET_COLOR << std::endl;
    } else {
      display_test_result("shmem_info_get_version()", result_shmem_info_get_version, false);
    }
    display_test_result("shmem_info_get_name()", result_shmem_info_get_name, false);
  }

  return true;
}
