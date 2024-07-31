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
bool text_shmem_fake_routine(void) {
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
bool text_shmem_init() {
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
bool text_shmem_barrier_all() {
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
bool text_shmem_barrier(void) {
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
int text_shmem_my_pe() {
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
int text_shmem_n_pes() {
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
bool text_shmem_pe_accessible() {
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
std::string text_shmem_info_get_version() {
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
std::string text_shmem_info_get_name() {
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
bool text_shmem_finalize() {
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
bool text_shmem_global_exit() {
  p_shmem_global_exit(0);
  return true;
}

/**
 * @brief Perform all setup tests
 * 
 * @return True if successful false otherwise
 */
bool run_setup_tests(test_options opts, int &mype, int &npes, std::string &version, std::string name) {
  /* Variables to hold test results */
  bool result_shmem_init = true;
  bool result_shmem_init_thread = true;
  bool result_shmem_barrier_all = true;
  bool result_shmem_barrier = true;
  bool result_shmem_my_pe = true;
  bool result_shmem_n_pes = true;
  bool result_shmem_pe_accessible = true;
  bool result_shmem_info_get_version = true;
  bool result_shmem_info_get_name = true;
  
  /* Initialize with shmem_init_thread() if THREADS tests were enabled */
  if (opts.test_threads) {
    if (!check_if_exists("shmem_init_thread")) {
      if (mype == 0) {
        display_not_found_warning("shmem_init_thread()", true);
      }
      return false;
    }
    else {
      result_shmem_init_thread = text_shmem_init_thread();
      if (!result_shmem_init_thread) {
        display_test_result("shmem_init_thread()", result_shmem_init_thread, true);
        return false;
      }
    }
  }
  else {
    /* Initialize with regular shmem_init() if THREADS tests are not enabled */
    if (!check_if_exists("shmem_init")) {
      if (mype == 0) {
        display_not_found_warning("shmem_init()", true);
      }
      return false;
    }
    else {
      result_shmem_init = text_shmem_init();
      if (!result_shmem_init) {
        display_test_result("shmem_init()", result_shmem_init, true);
        return false;
      }
    }
  }

  /* Run shmem_barrier_all() test */
  if (!check_if_exists("shmem_barrier_all")) {
    if (mype == 0) {
      display_not_found_warning("shmem_barrier_all()", true);
    }
    shmem_finalize();
    return false;
  }
  else {
    result_shmem_barrier_all = text_shmem_barrier_all();
    if (!result_shmem_barrier_all) {
      if (shmem_my_pe() == 0) {
        display_test_result("shmem_barrier_all()", result_shmem_barrier_all, true);
      }
      shmem_finalize();
      return false;
    }
  }

  /* Run shmem_my_pe() test */
  shmem_barrier_all();
  if (!check_if_exists("shmem_my_pe")) {
    if (mype == 0) {
      display_not_found_warning("shmem_my_pe()", true);
    }
    shmem_finalize();
    return false;
  }
  else {
    mype = text_shmem_my_pe();
    result_shmem_my_pe = mype >= 0;
    if (!result_shmem_my_pe) {
      if (mype == 0) {
        display_test_result("shmem_my_pe()", result_shmem_my_pe, true);
      }
      shmem_finalize();
      return false;
    }
  }

  /* Run shmem_n_pes() test */
  shmem_barrier_all();
  if (!check_if_exists("shmem_n_pes")) {
    if (mype == 0) {
      display_not_found_warning("shmem_n_pes", true);
    }
    shmem_finalize();
    return false;
  }
  else {
    /* Set npes */
    npes = text_shmem_n_pes();
    result_shmem_n_pes = npes > 0;
    if (!result_shmem_n_pes) {
      if (mype == 0) {
        display_test_result("shmem_n_pes()", result_shmem_n_pes, true);
      }
      shmem_finalize();
      return false;
    }
  }

  /* Run shmem_pe_accessible() test */
  shmem_barrier_all();
  if (!check_if_exists("shmem_pe_accessible")) {
    if (mype == 0) {
      display_not_found_warning("shmem_pe_accessible()", false);
    }
  }
  else {
    result_shmem_pe_accessible = text_shmem_pe_accessible();
    if (!result_shmem_pe_accessible) {
      if (mype == 0) {
        display_test_result("shmem_pe_accessible()", result_shmem_pe_accessible, true);
      }
      shmem_finalize();
      return false;
    }
  }

  /*
    Run test to make sure OpenSHMEM routines that aren't implemented
    don't throw compiler errors
  */
  #ifdef _DEBUG_
    shmem_barrier_all();
    if (!check_if_exists("shmem_fake_routine")) {
      if (mype == 0) {
        display_not_found_warning("shmem_fake_routine()", false);
      }
    }
    else {
      text_shmem_fake_routine();
    }
  #endif

  /* Display ASCII art logo */
  shmem_barrier_all();
  if (mype == 0) {
    display_logo();
  }

  /* Run shmem_barrier() test */
  shmem_barrier_all();
  if (!check_if_exists("shmem_barrier")) {
    if (mype == 0) {
      display_not_found_warning("shmem_barrier()", false);
    }
  }
  else {
    result_shmem_barrier = text_shmem_barrier();
    shmem_barrier_all();
  }

  /* Run shmem_info_get_version() test */
  shmem_barrier_all();
  if (!check_if_exists("shmem_info_get_version")) {
    result_shmem_info_get_version = false;
    if (mype == 0) {
      display_not_found_warning("shmem_info_get_version()", false);
    }
  }
  else {
    version = text_shmem_info_get_version();
    if (version == "") {
      result_shmem_info_get_version = false;
    }
  }

  /* Run shmem_info_get_name() test */
  shmem_barrier_all();
  if (!check_if_exists("shmem_info_get_name")) {
    result_shmem_info_get_name = false;
    if (mype == 0) {
      display_not_found_warning("shmem_info_get_name()", false);
    }
  }
  else {
    name = text_shmem_info_get_name();
    if (name == "") {
      result_shmem_info_get_name = false;
    }
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
    if (!opts.test_threads) {
      display_test_result("shmem_init()", result_shmem_init, true);
    }
    display_test_result("shmem_barrier_all()", result_shmem_barrier_all, true);
    display_test_result("shmem_barrier()", result_shmem_barrier, false);
    display_test_result("shmem_my_pe()", result_shmem_my_pe, true);
    display_test_result("shmem_n_pes()", result_shmem_n_pes, true);
    display_test_result("shmem_pe_accessible()", result_shmem_pe_accessible, true);
    if (version != "1.5" && version != "1.50") {
      std::cerr << YELLOW_COLOR << "shmem_info_get_version() test did not return 1.5... Returned " << version << std::endl;
    }
    else {
      display_test_result("shmem_info_get_version()", result_shmem_info_get_version, false);
    }
    display_test_result("shmem_info_get_name()", result_shmem_info_get_name,false);
  }

  return true;
}

