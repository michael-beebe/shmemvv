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
  std::cout << "  --test_ctx           Run communication/context management tests\n";
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
    {"test_ctx", no_argument, 0, 'e'},
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
        opts.all = true;
        break;
      default:
        return false;
    }
  }

  /* If no specific tests are selected and --all is not specified, enable all tests */
  if (!(opts.test_setup || opts.test_threads || opts.test_mem || opts.test_teams ||
        opts.test_ctx || opts.test_remote || opts.test_atomics || opts.test_signaling ||
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
 * @brief Function to check if a routine exists
 * @param routine_name Name of the OpenSHMEM routine to check
 * @param mype Current PE number
 * @return True if the routine exists, false otherwise
 */
bool check_if_exists(const std::string& routine_name, int mype) {
  // void *handle = dlopen(NULL, RTLD_NOW | RTLD_GLOBAL);
  void *handle = dlopen(NULL, RTLD_LAZY);
  if (!handle) {
    if (mype == 0) {
      std::cerr << "Failed to open handle: " << dlerror() << std::endl;
    }
    return false;
  }

  /* Clear any existing errors */
  dlerror();

  void *symbol = dlsym(handle, routine_name.c_str());
  const char *dlsym_error = dlerror();
  if (dlsym_error) {
    if (mype == 0) {
      std::cerr << "Function " << routine_name << " not found: " << dlsym_error << std::endl;
    }
    symbol = nullptr;
  }

  dlclose(handle);
  return symbol != nullptr;
}

/**
  @brief Displays a warning message that the given routine is not avaible in the
        tested OpenSHMEM library
  @param routine_name OpenSHMEM routine
*/
void display_not_found_warning(std::string routine_name) {
  std::cerr << YELLOW_COLOR << "This OpenSHMEM implementation does not support " << routine_name << RESET_COLOR << std::endl;
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

/**
  @brief Run finalization test
  @param mype Current PE
 */
void finalize_shmemvv(int mype) {
  if (mype == 0) { display_test_header("FINALIZATION"); }
  if (mype == 0) { display_test_result("shmem_finalize()", test_shmem_finalize(), false); }
  if (mype == 0) { std::cout << std::endl; }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// /* Structure to hold routine name and found flag */
// struct RoutineCheck {
//   std::string routine_name;
//   bool found;
// };

// /* Callback function for dl_iterate_phdr */
// int callback(struct dl_phdr_info *info, size_t size, void *data) {
//   RoutineCheck *routine_check = static_cast<RoutineCheck *>(data);

//   for (int i = 0; i < info->dlpi_phnum; ++i) {
//     if (info->dlpi_phdr[i].p_type == PT_DYNAMIC) {
//       ElfW(Dyn) *dyn = reinterpret_cast<ElfW(Dyn) *>(info->dlpi_addr + info->dlpi_phdr[i].p_vaddr);
//       ElfW(Sym) *symtab = nullptr;
//       const char *strtab = nullptr;

//       while (dyn->d_tag != DT_NULL) {
//         if (dyn->d_tag == DT_SYMTAB) {
//           symtab = reinterpret_cast<ElfW(Sym) *>(info->dlpi_addr + dyn->d_un.d_ptr);
//         } else if (dyn->d_tag == DT_STRTAB) {
//           strtab = reinterpret_cast<const char *>(info->dlpi_addr + dyn->d_un.d_ptr);
//         }
//         ++dyn;
//       }

//       if (symtab && strtab) {
//         for (size_t j = 0; symtab[j].st_name != 0; ++j) {
//           const char *sym_name = &strtab[symtab[j].st_name];
//           std::cerr << "Checking symbol: " << sym_name << std::endl;
//           if (std::strstr(sym_name, routine_check->routine_name.c_str()) != nullptr) {
//             std::cerr << "Found matching symbol: " << sym_name << std::endl;
//             routine_check->found = true;
//             return 1; /* Stop iterating */
//           }
//         }
//       }
//     }
//   }

//   return 0; /* Continue iterating */
// }

// /**
//  * Function to check if a routine exists
//  * @param routine_name - Name of the OpenSHMEM routine to check
//  * @param mype - Current PE number
//  * @return - True if the routine exists, false otherwise
//  */
// bool check_if_exists(const std::string& routine_name, int mype) {
//   std::cerr << "Entering check_if_exists for " << routine_name << std::endl;

//   RoutineCheck routine_check = {routine_name, false};
//   dl_iterate_phdr(callback, &routine_check);

//   std::cerr << "Exiting check_if_exists for " << routine_name << " with result " << (routine_check.found ? "found" : "not found") << std::endl;
//   return routine_check.found;
// }
//////////////////////////////////////////////////////////////////////////////////////////////////
