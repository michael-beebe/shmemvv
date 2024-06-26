/**
  @file shmemvv.cpp
  @brief Contains helper functions for the OpenSHMEM verification/validation test suite.
 */

#include "shmemvv.hpp"

/**
  @brief Displays usage information for the test suite.
 */
void display_help() {
  std::cout << "\nThis program is a verification/validation test suite for OpenSHMEM implementations.\n";
  std::cout << "\nUsage: shmemvv [options]\n";
  std::cout << "Options:\n";
  std::cout << "  --test_setup         Run setup tests\n";
  std::cout << "  --test_threads       Run thread support tests\n";
  std::cout << "  --test_mem           Run memory management tests\n";
  std::cout << "  --test_teams         Run team management tests\n";
  std::cout << "  --test_comms         Run communication management tests\n";
  std::cout << "  --test_remote        Run remote memory access tests\n";
  std::cout << "  --test_atomics       Run atomic memory operations tests\n";
  std::cout << "  --test_signaling     Run signaling operations tests\n";
  std::cout << "  --test_collectives   Run collective operations tests\n";
  std::cout << "  --test_pt2pt_synch   Run point-to-point synchronization tests\n";
  std::cout << "  --test_mem_ordering  Run memory ordering tests\n";
  std::cout << "  --test_locking       Run distributed locking tests\n";
  std::cout << "  --all                (default) Run all tests\n";
  std::cout << "  --help               Display this help message\n";
  std::cout << std::endl;
}

/**
  @brief Parses command-line options.
  @param argc Number of command-line arguments.
  @param argv Array of command-line argument strings.
  @param opts Reference to the test options structure.
  @return True if parsing is successful, false otherwise.
 */
bool parse_opts(int argc, char *argv[], test_options &opts) {
  /* Define command-line options */
  static struct option long_options[] = {
    {"test_setup", no_argument, 0, 'a'},
    {"test_threads", no_argument, 0, 'b'},
    {"test_mem", no_argument, 0, 'c'},
    {"test_teams", no_argument, 0, 'd'},
    {"test_comms", no_argument, 0, 'e'},
    {"test_remote", no_argument, 0, 'f'},
    {"test_atomics", no_argument, 0, 'g'},
    {"test_signaling", no_argument, 0, 'h'},
    {"test_collectives", no_argument, 0, 'i'},
    {"test_pt2pt_synch", no_argument, 0, 'j'},
    {"test_mem_ordering", no_argument, 0, 'k'},
    {"test_locking", no_argument, 0, 'l'},
    {"help", no_argument, 0, 'm'},
    {"all", no_argument, 0, 'n'},
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
        opts.test_comms = true;
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
        opts.all = true;
        break;
      default:
        return false;
    }
  }

  /* If no specific tests are selected and --all is not specified, enable all tests */
  if (!(opts.test_setup || opts.test_threads || opts.test_mem || opts.test_teams ||
        opts.test_comms || opts.test_remote || opts.test_atomics || opts.test_signaling ||
        opts.test_collectives || opts.test_pt2pt_synch || opts.test_mem_ordering || opts.test_locking || opts.all)) {
    opts.all = true;
  }

  return true;
}

/**
  @brief Displays the ASCII art logo.
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
  @brief Displays a header for the test category.
  @param test_name Name of the test category.
 */
void display_test_header(std::string test_name) {
  if (test_name == "FINALIZATION") {
    std::cout << "\n==================================================================" << std::endl;
    std::cout << "          Running " << test_name << " test..." << std::endl;
    std::cout << "==================================================================" << std::endl;
  }
  else {
    std::cout << "\n==================================================================" << std::endl;
    std::cout << "          Running " << test_name << " tests..." << std::endl;
    std::cout << "==================================================================" << std::endl;
  }
}

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
) {
  std::cout << "\n--------------------------------------------------------------" << std::endl;
  std::cout << "                    Test Information" << std::endl;
  std::cout << "--------------------------------------------------------------" << std::endl;
  std::cout << "  OpenSHMEM Name:       " << shmem_name << std::endl;
  std::cout << "  OpenSHMEM Version:    " << shmem_version << std::endl;
  std::cout << "  Number of PEs:        " << npes << std::endl;  
}

/**
  @brief Displays the result of a test.
  @param routine_name Name of the OpenSHMEM routine that was tested.
  @param passed True if the test passed, false if the test failed.
  @param required True if the test is required, false otherwise.
 */
void display_test_result(std::string routine_name, bool passed, bool required) {
  if (passed) {
    std::cout << routine_name << " test " << GREEN_COLOR << "PASSED!" << RESET_COLOR << std::endl;
  }
  else {
    if (required) {
      std::cerr << routine_name << " test " << RED_COLOR << "FAILED!" << RESET_COLOR << " This test must pass to continue!" << std::endl;
    }
    else {
      std::cerr << routine_name << " test " << RED_COLOR << "FAILED!" << RESET_COLOR << std::endl;
    }
  }
}

