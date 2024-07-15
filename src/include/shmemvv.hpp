/**
  @file shmemvv.hpp
  @brief Contains helper function declarations for the OpenSHMEM verification/validation test suite.
 */

#ifndef SHMEMVV_HPP
#define SHMEMVV_HPP

#include "routines.hpp"

#include <shmem.h>
#include <iostream>
#include <getopt.h>
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include <dlfcn.h>

#include "../tests/setup/setup_tests.hpp"
#include "../tests/threads/threads_tests.hpp"
#include "../tests/mem/mem_tests.hpp"
#include "../tests/teams/teams_tests.hpp"
#include "../tests/comms/comms_tests.hpp"
#include "../tests/remote/remote_tests.hpp"
#include "../tests/atomics/atomics_tests.hpp"
#include "../tests/signaling/signaling_tests.hpp"
#include "../tests/collectives/collectives_tests.hpp"
#include "../tests/pt2pt/pt2pt_tests.hpp"
#include "../tests/mem_ordering/mem_ordering_tests.hpp"
#include "../tests/locking/locking_tests.hpp"

/* ANSI color codes for pretty output */
#define RESET_COLOR "\033[0m"
#define RED_COLOR "\033[31m"
#define GREEN_COLOR "\033[32m"
#define YELLOW_COLOR "\033[33m"

#define DEBUG_LINE "--------------------------------------------"

/**
  @struct test_options
  @brief Struct to hold selected tests options.
 */
struct test_options {
  bool all;                    /**< Flag to run all tests */
  bool test_setup;             /**< Flag to run setup tests */
  bool test_threads;           /**< Flag to run thread support tests */
  bool test_mem;               /**< Flag to run memory management tests */
  bool test_teams;             /**< Flag to run team management tests */
  bool test_ctx;               /**< Flag to run communication management tests */
  bool test_remote;            /**< Flag to run remote memory access tests */
  bool test_atomics;           /**< Flag to run atomic memory operations tests */
  bool test_signaling;         /**< Flag to run signaling operations tests */
  bool test_collectives;       /**< Flag to run collective operations tests */
  bool test_pt2pt_synch;       /**< Flag to run point-to-point synchronization tests */
  bool test_mem_ordering;      /**< Flag to run memory ordering tests */
  bool test_locking;           /**< Flag to run distributed locking tests */
  bool help;                   /**< Flag to display help message */

  /**
    @brief Constructor to initialize all flags to false.
   */
  test_options() :
    all(false), test_setup(false), test_threads(false),
    test_mem(false), test_teams(false), test_ctx(false),
    test_remote(false), test_atomics(false), test_signaling(false),
    test_collectives(false), test_pt2pt_synch(false),
    test_mem_ordering(false), test_locking(false), help(false) {}
};

/**
  @brief Parses command-line options.
  @param argc Number of command-line arguments.
  @param argv Array of command-line argument strings.
  @param opts Reference to the test options structure.
  @return True if parsing is successful, false otherwise.
 */
bool parse_opts(int argc, char *argv[], test_options &opts);

/**
  @brief Displays usage information.
 */
void display_help();

/**
  @brief Displays the ASCII art logo.
 */
void display_logo();

/**
  @brief Displays a header for the test category.
  @param test_name Name of the test category.
 */
void display_test_header(std::string test_name);

/**
  @brief Displays information about the test suite.
  @param shmem_name Name of the OpenSHMEM library.
  @param shmem_version Version of the OpenSHMEM library.
  @param npes Number of PEs (Processing Elements).
 */
void display_test_info(
  std::string shmem_name,
  std::string shmem_version,
  int npes
);

/**
  @brief Loads the OpenSHMEM routines dynamically.
  @return True if successful, false if otherwise
 */
bool load_routines();

/**
  @brief Checks whether the tested OpenSHMEM implementation has a given routine
  @param routine_name OpenSHMEM routine that we are making sure is present
  @return true if it exists, false otherwise
 */
bool check_if_exists(const std::string& routine_name);

/**
  @brief Displays a warning message that the given routine is not avaible in the
        tested OpenSHMEM library
  @param routine_name OpenSHMEM routine
  @param required True if test is required, false otherwise
*/
void display_not_found_warning(std::string routine_name, bool required);

/**
  @brief Print error message saying that there needs to be at least
         2 PEs for the given test type
  @param test_type Category of tests
 */
void display_not_enough_pes(std::string test_type);

/**
  @brief Displays whether the test passed
  @param routine_name OpenSHMEM routine that was tested
  @param passed True if the test passed, false if the test failed
  @param required True if the test is required, false otherwise
 */
void display_test_result(std::string routine_name, bool passed, bool required);

/**
  @brief Run finalization test
  @param mype Current PE
 */
 void finalize_shmemvv(int mype);


#endif /* SHMEMVV_HPP */
