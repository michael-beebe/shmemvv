/**
 * @file shmemvv.h 
 * 
 */

#ifndef SHMEMVV_H
#define SHMEMVV_H

#include <shmem.h>
#include <getopt.h>
#include <stdbool.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* ANSI color codes for pretty output */
#define RESET_COLOR "\033[0m"
#define RED_COLOR "\033[31m"
#define GREEN_COLOR "\033[32m"
#define YELLOW_COLOR "\033[33m"

#define HLINE "--------------------------------------------"

#ifdef __cplusplus
extern "C" {
#endif

/**
  @struct test_options
  @brief Struct to hold selected tests options.
 */
typedef struct {
  bool test_all;               /**< Flag to run all tests */
  bool test_setup;             /**< Flag to run setup tests */
  bool test_threads;           /**< Flag to run thread support tests */
  bool test_mem;               /**< Flag to run memory management tests */
  bool test_teams;             /**< Flag to run team management tests. No-op. */
  bool test_ctx;               /**< Flag to run communication management tests */
  bool test_remote;            /**< Flag to run remote memory access tests */
  bool test_atomics;           /**< Flag to run atomic memory operations tests */
  bool test_signaling;         /**< Flag to run signaling operations tests */
  bool test_collectives;       /**< Flag to run collective operations tests */
  bool test_pt2pt_synch;       /**< Flag to run point-to-point synchronization tests */
  bool test_mem_ordering;      /**< Flag to run memory ordering tests */
  bool test_locking;           /**< Flag to run distributed locking tests */
  bool help;                   /**< Flag to display help message */
} test_options;

/**
  @brief Parses command-line options.
  @param argc Number of command-line arguments.
  @param argv Array of command-line argument strings.
  @param opts Reference to the test options structure.
  @return True if parsing is successful, false otherwise.
 */
bool parse_opts(int argc, char *argv[], test_options *opts);

/**
  @brief Displays usage information.
 */
void display_help(void);

/**
 * @brief Displays the ASCII art logo.
 *
 * This function prints out the ASCII art logo for the test suite.
 */
void display_logo(void);

/**
  @brief Displays a header for the test category.
  @param test_name Name of the test category.
 */
void display_test_header(const char *test_name);

/**
  @brief Displays information about the test suite.
  @param shmem_name Name of the OpenSHMEM library.
  @param shmem_version Version of the OpenSHMEM library.
  @param npes Number of PEs (Processing Elements).
 */
void display_test_info(const char *shmem_name, const char *shmem_version, int npes);

/**
  @brief Checks whether the tested OpenSHMEM implementation has a given routine
  @param routine_name OpenSHMEM routine that we are making sure is present
  @return true if it exists, false otherwise
 */
bool check_if_exists(const char *routine_name);

/**
  @brief Displays a warning message that the given routine is not available in the
        tested OpenSHMEM library
  @param routine_name OpenSHMEM routine
  @param required True if test is required, false otherwise
 */
void display_not_found_warning(const char *routine_name, bool required);

/**
  @brief Print error message saying that there needs to be at least
         2 PEs for the given test type
  @param test_type Category of tests
 */
void display_not_enough_pes(const char *test_type);

/**
  @brief Displays whether the test passed
  @param routine_name OpenSHMEM routine that was tested
  @param passed True if the test passed, false if the test failed
  @param required True if the test is required, false otherwise
 */
void display_test_result(const char *routine_name, bool passed, bool required);

#ifdef __cplusplus
}
#endif

#endif /* SHMEMVV_H */
