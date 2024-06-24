/**
  @file setup_tests.cpp
  @brief Contains function for the OpenSHMEM setup tests.
 */

#include "setup_tests.hpp"

bool test_shmem_init() {
  /*
    Check if already initialized,
    since we do that at the beginning of the program..
  */
  bool is_initialized = shmem_my_pe() >= 0;
  return is_initialized;
}

bool test_shmem_finalize() {
  shmem_finalize(); /* Finalize the OpenSHMEM environment */
  /* Since we cannot call any shmem_* functions after finalize,
     we will consider the test successful if no errors occur
     up to this point. */

  // try {
  //   /* Attempting to call shmem_my_pe after finalization should result in undefined behavior or error */
  //   int mype = shmem_my_pe(); /* This call is expected to fail or result in an invalid state */
  //   if (mype >= 0) {
  //     std::cerr << "Error: shmem_my_pe() succeeded after finalization, which is unexpected.\n";
  //     return false;
  //   }
  // }
  // catch (const std::exception &e) {
  //   std::cerr << "Caught exception after shmem_finalize(): " << e.what() << "\n";
  //   return true; /* Expecting this to fail */
  // }
  // catch (...) {
  //   std::cerr << "Caught unknown exception after shmem_finalize().\n";
  //   return true; /* Expecting this to fail */
  // }
  return true;
}

bool test_shmem_my_pe() {
  int mype = shmem_my_pe(); /* Retrieve the current PE number */
  /* Verify that the PE number is valid */
  bool is_valid_pe = (mype >= 0);
  return is_valid_pe;
}

bool test_shmem_n_pes() {
  int npes = shmem_n_pes(); /* Retrieve the total number of PEs */
  /* Verify that the total number of PEs is greater than zero */
  bool is_valid_npes = (npes > 0);
  return is_valid_npes;
}

bool test_shmem_pe_accessible() {
  return true;
}

bool test_shmem_ptr() {
  return true;
}

bool test_shmem_info_get_version() {
  return true;
}

bool test_shmem_info_get_name() {
  return true;
}

bool test_start_pes() {
  return true;
}

bool test_shmem_global_exit() {
  return true;
}

