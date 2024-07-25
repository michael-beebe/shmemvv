/**
 * @file shmemvv.cpp
 * @brief Contains helper functions for the OpenSHMEM verification/validation test suite.
 */

#include "shmemvv.hpp"

/**
 * @brief Displays usage information for the test suite.
 *
 * This function prints out the usage information and available options for the OpenSHMEM
 * verification/validation test suite.
 */
void display_help() {
  std::cout << "\nThis program is a verification/validation test suite for OpenSHMEM implementations.\n";
  std::cout << "\nUsage: shmemvv [options]\n";
  std::cout << "Options:\n";
  std::cout << "  --test_setup         Run setup tests\n";
  std::cout << "  --test_threads       Run thread support tests\n";
  std::cout << "  --test_mem           Run memory management tests\n";
  std::cout << "  --test_teams         Run team management tests\n";
  std::cout << "  --test_ctx           Run communication/context management tests\n";
  std::cout << "  --test_remote        Run remote memory access tests\n";
  std::cout << "  --test_atomics       Run atomic memory operations tests\n";
  std::cout << "  --test_signaling     Run signaling operations tests\n";
  std::cout << "  --test_collectives   Run collective operations tests\n";
  std::cout << "  --test_pt2pt_synch   Run point-to-point synchronization tests\n";
  std::cout << "  --test_mem_ordering  Run memory ordering tests\n";
  std::cout << "  --test_locking       Run distributed locking tests\n";
  std::cout << "  --test_all           (default) Run all tests\n";
  std::cout << "  --help               Display this help message\n";
  std::cout << std::endl;
}

/**
 * @brief Parses command-line options.
 *
 * This function parses the command-line arguments and sets the corresponding options
 * in the provided test_options structure.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @param opts Reference to the test options structure.
 * @return True if parsing is successful, false otherwise.
 */
bool parse_opts(int argc, char *argv[], test_options &opts) {
  /* Define command-line options */
  static struct option long_options[] = {
    {"test_setup", no_argument, 0, 'a'},
    {"test_threads", no_argument, 0, 'b'},
    {"test_mem", no_argument, 0, 'c'},
    {"test_teams", no_argument, 0, 'd'},
    {"test_ctx", no_argument, 0, 'e'},
    {"test_remote", no_argument, 0, 'f'},
    {"test_atomics", no_argument, 0, 'g'},
    {"test_signaling", no_argument, 0, 'h'},
    {"test_collectives", no_argument, 0, 'i'},
    {"test_pt2pt_synch", no_argument, 0, 'j'},
    {"test_mem_ordering", no_argument, 0, 'k'},
    {"test_locking", no_argument, 0, 'l'},
    {"help", no_argument, 0, 'm'},
    {"test_all", no_argument, 0, 'n'},
    {0, 0, 0, 0}
  };

  int option_index = 0;
  int c;
  while ((c = getopt_long(argc, argv, "abcdefghijklmn", long_options, &option_index)) != -1) {
    switch (c) {
      case 'a':
        opts.test_setup = true;
        break;
      case 'b':
        opts.test_threads = true;
        break;
      case 'c':
        opts.test_mem = true;
        break;
      case 'd':
        opts.test_teams = true;
        break;
      case 'e':
        opts.test_ctx = true;
        break;
      case 'f':
        opts.test_remote = true;
        break;
      case 'g':
        opts.test_atomics = true;
        break;
      case 'h':
        opts.test_signaling = true;
        break;
      case 'i':
        opts.test_collectives = true;
        break;
      case 'j':
        opts.test_pt2pt_synch = true;
        break;
      case 'k':
        opts.test_mem_ordering = true;
        break;
      case 'l':
        opts.test_locking = true;
        break;
      case 'm':
        opts.help = true;
        break;
      case 'n':
        opts.test_all = true;
        break;
      default:
        return false;
    }
  }

  /* If no specific tests are selected and --all is not specified, enable all tests */
  if (!(opts.test_setup || opts.test_threads || opts.test_mem || opts.test_teams ||
        opts.test_ctx || opts.test_remote || opts.test_atomics || opts.test_signaling ||
        opts.test_collectives || opts.test_pt2pt_synch || opts.test_mem_ordering || opts.test_locking || opts.test_all)) {
    opts.test_all = true;
  }

  return true;
}

/**
 * @brief Displays the ASCII art logo.
 *
 * This function prints out the ASCII art logo for the test suite.
 */
void display_logo() {
  std::cout << R"(
  ░░░░░░░ ░░   ░░ ░░░    ░░░ ░░░░░░░ ░░░    ░░░ ░░    ░░ ░░    ░░ 
  ▒▒      ▒▒   ▒▒ ▒▒▒▒  ▒▒▒▒ ▒▒      ▒▒▒▒  ▒▒▒▒ ▒▒    ▒▒ ▒▒    ▒▒ 
  ▒▒▒▒▒▒▒ ▒▒▒▒▒▒▒ ▒▒ ▒▒▒▒ ▒▒ ▒▒▒▒▒   ▒▒ ▒▒▒▒ ▒▒ ▒▒    ▒▒ ▒▒    ▒▒ 
       ▓▓ ▓▓   ▓▓ ▓▓  ▓▓  ▓▓ ▓▓      ▓▓  ▓▓  ▓▓  ▓▓  ▓▓   ▓▓  ▓▓  
  ███████ ██   ██ ██      ██ ███████ ██      ██   ████     ████   
)";
}

/**
 * @brief Displays a header for the test category.
 *
 * This function prints out a header indicating the start of a specific test category.
 *
 * @param test_name Name of the test category.
 */
void display_test_header(std::string test_name) {
  if (test_name == "FINALIZATION") {
    std::cout << "\n==================================================================" << std::endl;
    std::cout << "            Running " << test_name << " test..." << std::endl;
    std::cout << "==================================================================" << std::endl;
  }
  else {
    std::cout << "\n==================================================================" << std::endl;
    std::cout << "            Running " << test_name << " tests..." << std::endl;
    std::cout << "==================================================================" << std::endl;
  }
}

/**
 * @brief Displays information about the test suite.
 *
 * This function prints out information about the OpenSHMEM library being tested, including
 * the library name, version, and the number of PEs (Processing Elements).
 *
 * @param shmem_name Name of the OpenSHMEM library.
 * @param shmem_version Version of the OpenSHMEM library.
 * @param npes Number of PEs (Processing Elements).
 */
void display_test_info(std::string shmem_name, std::string shmem_version, int npes) {
  std::cout << "\n==================================================================" << std::endl;
  std::cout << "===                   Test Information                         ===" << std::endl;
  std::cout << "==================================================================" << std::endl;
  std::cout << "  OpenSHMEM Name:       " << shmem_name << std::endl;
  std::cout << "  OpenSHMEM Version:    " << shmem_version << std::endl;
  std::cout << "  Number of PEs:        " << npes << std::endl;  
}

/**
 * @brief Function to check if a routine exists.
 *
 * This function checks if a given OpenSHMEM routine exists in the currently loaded library.
 *
 * @param routine_name Name of the OpenSHMEM routine to check.
 * @return True if the routine exists, false otherwise.
 */
bool check_if_exists(const std::string& routine_name) {
  void *handle = dlopen(NULL, RTLD_LAZY);

  /* Clear any existing errors */
  dlerror();

  void *symbol = dlsym(handle, routine_name.c_str());

  const char *dlsym_error = dlerror();
  if (dlsym_error) {
    symbol = nullptr;
  }

  dlclose(handle);
  return symbol != nullptr;
}

/**
 * @brief Displays a warning message that the given routine is not available in the
 *        tested OpenSHMEM library.
 *
 * This function prints a warning message indicating that a specific OpenSHMEM routine
 * is not available in the library being tested.
 *
 * @param routine_name Name of the OpenSHMEM routine.
 * @param required True if the test is required, false otherwise.
 */
void display_not_found_warning(std::string routine_name, bool required) {
  if (required) {
    std::cerr << RED_COLOR << "NOT FOUND - required to continue" << RESET_COLOR << ": " << routine_name << std::endl;
  }
  else {
    std::cerr << YELLOW_COLOR << "NOT FOUND" << RESET_COLOR << ": " << routine_name << std::endl;
  }
}

/**
 * @brief Print error message saying that there needs to be at least
 *        2 PEs for the given test type.
 *
 * This function prints an error message indicating that at least 2 PEs
 * are required to run the specified test type.
 *
 * @param test_type Category of tests.
 */
void display_not_enough_pes(std::string test_type) {
  std::cerr << RED_COLOR << "ERROR" << RESET_COLOR << ": The " << test_type << " tests require at least 2 PEs!" << std::endl;
}

/**
 * @brief Displays the result of a test.
 *
 * This function prints out the result of a specific OpenSHMEM routine test.
 *
 * @param routine_name Name of the OpenSHMEM routine that was tested.
 * @param passed True if the test passed, false if the test failed.
 * @param required True if the test is required, false otherwise.
 */
void display_test_result(std::string routine_name, bool passed, bool required) {
  if (passed) {
    std::cout << GREEN_COLOR << "PASSED" << RESET_COLOR << ": " << routine_name << std::endl; 
  }
  else {
    if (required) {
      std::cerr << RED_COLOR << "FAILED" << RESET_COLOR << ": " << routine_name << RED_COLOR << " This test must pass to continue!" << RESET_COLOR << std::endl;
    }
    else {
      std::cerr << RED_COLOR << "FAILED" << RESET_COLOR << ": " << routine_name << std::endl;
    }
  }
}

/**
 * @brief Runs the finalization test.
 *
 * This function runs the finalization test for OpenSHMEM and displays the results.
 *
 * @param mype Current PE.
 */
void finalize_shmemvv(int mype) {
  if (mype == 0) { display_test_header("FINALIZATION"); }
  if (mype == 0) { display_test_result("shmem_finalize()", test_shmem_finalize(), false); }
  if (mype == 0) { std::cout << std::endl; }
}
