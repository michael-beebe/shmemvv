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
  @brief Tests if a pointer is accessible from a remote PE.
  @return True if the pointer is accessible, false otherwise.
 */
bool test_shmem_ptr() {
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();
  int* ptr = (int*)shmem_malloc(sizeof(int));

  if (ptr == nullptr) {
    return false;
  }

  *ptr = mype;

  shmem_barrier_all();

  bool test_passed = true;

  for (int pe = 0; pe < npes; ++pe) {
    if (shmem_addr_accessible(ptr, pe) != 1) {
      test_passed = false;
    }
    else {
      int* remote_ptr = (int*)shmem_ptr(ptr, pe);
      if (pe == mype) {
        if (remote_ptr != ptr) {
          test_passed = false;
        }
      }
      else {
        if (remote_ptr == nullptr) {
          test_passed = false;
        }
      }
    }
  }

  shmem_free(ptr);
  return test_passed;
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
// int perform_setup(int argc, char *argv[], int &mype, int &npes, std::string &version, std::string &name) {
//   /* Run shmem_init() test */
//   if (!test_shmem_init()) {
//     if (shmem_my_pe() == 0) {
//       std::cerr << RED_COLOR << "shmem_init() test FAILED! This test must pass to continue!" << RESET_COLOR << std::endl;
//     }
//     return EXIT_FAILURE;
//   }

//   if (!test_shmem_barrier_all()) {
//     if (shmem_my_pe() == 0) {
//       std::cerr << RED_COLOR << "shmem_barrier_all() test FAILED! This test must pass to continue!" << RESET_COLOR << std::endl;
//     }
//     shmem_finalize();
//     return EXIT_FAILURE;
//   }

//   shmem_barrier_all();

//   /* If shmem_init passes, run shmem_my_pe() test */
//   mype = test_shmem_my_pe();
//   if (mype < 0) {
//     if (mype == 0) {
//       std::cerr << RED_COLOR << "shmem_my_pe() test FAILED! This test must pass to continue!" << RESET_COLOR << std::endl;
//     }
//     shmem_finalize();
//     return EXIT_FAILURE;
//   }

//   shmem_barrier_all();

//   npes = test_shmem_n_pes();
//   if (npes <= 0) {
//     if (mype == 0) {
//       std::cerr << RED_COLOR << "shmem_n_pes() test FAILED! This test must pass to continue!" << RESET_COLOR << std::endl;
//     }
//     shmem_finalize();
//     return EXIT_FAILURE;
//   }


//   shmem_barrier_all();

//   /* shmem_init() and shmem_my_pe() tests passed */
//   if (mype == 0) {
//     std::cout << GREEN_COLOR << "shmem_init() test PASSED!" << RESET_COLOR << std::endl;
//     std::cout << GREEN_COLOR << "shmem_barrier_all() test PASSED!" << RESET_COLOR << std::endl;
//     std::cout << GREEN_COLOR << "shmem_my_pe() test PASSED!" << RESET_COLOR << std::endl;
//     std::cout << GREEN_COLOR << "shmem_n_pes() test PASSED!" << RESET_COLOR << std::endl;
//   }

//   shmem_barrier_all();

//   /* Run shmem_pe_accessible() test */
//   if (!test_shmem_pe_accessible()) {
//     if (mype == 0) {
//       std::cerr << RED_COLOR << "shmem_pe_accessible() test FAILED!" << RESET_COLOR << std::endl;
//     }
//   }
//   else {
//     if (mype == 0) {
//       std::cout << GREEN_COLOR << "shmem_pe_accessible() test PASSED!" << RESET_COLOR << std::endl;
//     }
//   }

//   shmem_barrier_all();

//   /* Run shmem_ptr() test */
//   if (!test_shmem_ptr()) {
//     if (mype == 0) {
//       std::cerr << RED_COLOR << "shmem_ptr() test FAILED!" << RESET_COLOR << std::endl;
//     }
//   }
//   else {
//     if (mype == 0) {
//       std::cout << GREEN_COLOR << "shmem_ptr() test PASSED!" << RESET_COLOR << std::endl;
//     }
//   }

//   shmem_barrier_all();

//   /* Run shmem_info_get_version() test */
//   version = test_shmem_info_get_version();
//   if (version == "") {
//     if (mype == 0) {
//       std::cerr << RED_COLOR << "shmem_info_get_version() test FAILED!" << RESET_COLOR << std::endl;
//     }
//   }
//   else if (version != "1.5" && version != "1.50") {
//     if (mype == 0) {
//       std::cerr << YELLOW_COLOR << "shmem_info_get_version() test did not return 1.5... Returned " << version << std::endl;
//     }
//   }
//   else {
//     if (mype == 0) {
//       std::cout << GREEN_COLOR << "shmem_info_get_version() test PASSED!" << RESET_COLOR << std::endl;
//     }
//   }

//   shmem_barrier_all();

//   /* Run shmem_info_get_name() test */
//   name = test_shmem_info_get_name();
//   if (name == "") {
//     if (mype == 0) {
//       std::cerr << RED_COLOR << "shmem_info_get_name() test FAILED!" << RESET_COLOR << std::endl;
//     }
//   }
//   else {
//     if (mype == 0) {
//       std::cout << GREEN_COLOR << "shmem_info_get_name() test PASSED!" << RESET_COLOR << std::endl;
//     }
//   }

//   return EXIT_SUCCESS;
// }
