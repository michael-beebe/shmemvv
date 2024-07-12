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
  std::cout << "\n==================================================================" << std::endl;
  std::cout << "===                   Test Information                         ===" << std::endl;
  std::cout << "==================================================================" << std::endl;
  std::cout << "  OpenSHMEM Name:       " << shmem_name << std::endl;
  std::cout << "  OpenSHMEM Version:    " << shmem_version << std::endl;
  std::cout << "  Number of PEs:        " << npes << std::endl;  
}


/* Define global function pointers */
shmem_fake_routine_func p_shmem_fake_routine = nullptr;

/* Setup, Exit, and Query Routines */
shmem_init_func p_shmem_init = nullptr;
shmem_finalize_func p_shmem_finalize = nullptr;
shmem_my_pe_func p_shmem_my_pe = nullptr;
shmem_n_pes_func p_shmem_n_pes = nullptr;
shmem_pe_accessible_func p_shmem_pe_accessible = nullptr;
shmem_barrier_all_func p_shmem_barrier_all = nullptr;
shmem_barrier_func p_shmem_barrier = nullptr;
shmem_info_get_version_func p_shmem_info_get_version = nullptr;
shmem_info_get_name_func p_shmem_info_get_name = nullptr;
shmem_global_exit_func p_shmem_global_exit = nullptr;

/* Thread Support Routines */
shmem_init_thread_func p_shmem_init_thread = nullptr;
shmem_query_thread_func p_shmem_query_thread = nullptr;

/* Memory Management Routines */
shmem_ptr_func p_shmem_ptr = nullptr;
shmem_malloc_func p_shmem_malloc = nullptr;
shmem_free_func p_shmem_free = nullptr;
shmem_realloc_func p_shmem_realloc = nullptr;
shmem_align_func p_shmem_align = nullptr;
shmem_malloc_with_hints_func p_shmem_malloc_with_hints = nullptr;
shmem_calloc_func p_shmem_calloc = nullptr;
shmem_addr_accessible_func p_shmem_addr_accessible = nullptr;

/* Team Management Routines */
shmem_team_my_pe_func p_shmem_team_my_pe = nullptr;
shmem_team_n_pes_func p_shmem_team_n_pes = nullptr;
shmem_team_get_config_func p_shmem_team_get_config = nullptr;
shmem_team_translate_pe_func p_shmem_team_translate_pe = nullptr;
shmem_team_split_strided_func p_shmem_team_split_strided = nullptr;
shmem_team_split_2d_func p_shmem_team_split_2d = nullptr;
shmem_team_destroy_func p_shmem_team_destroy = nullptr;

/* Communication/Context Management Routines */
shmem_ctx_create_func p_shmem_ctx_create = nullptr;
shmem_team_create_ctx_func p_shmem_team_create_ctx = nullptr;
shmem_ctx_destroy_func p_shmem_ctx_destroy = nullptr;
shmem_ctx_get_team_func p_shmem_ctx_get_team = nullptr;

/* Remote Access Routines */
shmem_long_put_func p_shmem_long_put = nullptr;
shmem_long_p_func p_shmem_long_p = nullptr;
shmem_long_iput_func p_shmem_long_iput = nullptr;
shmem_long_get_func p_shmem_long_get = nullptr;
shmem_long_g_func p_shmem_long_g = nullptr;
shmem_long_iget_func p_shmem_long_iget = nullptr;
shmem_long_put_nbi_func p_shmem_long_put_nbi = nullptr;
shmem_long_get_nbi_func p_shmem_long_get_nbi = nullptr;


/* Atomic Memory Operations */
shmem_long_atomic_fetch_func p_shmem_long_atomic_fetch = nullptr;
shmem_long_atomic_set_func p_shmem_long_atomic_set = nullptr;
shmem_long_atomic_compare_swap_func p_shmem_long_atomic_compare_swap = nullptr;
shmem_long_atomic_swap_func p_shmem_long_atomic_swap = nullptr;
shmem_long_atomic_fetch_inc_func p_shmem_long_atomic_fetch_inc = nullptr;
shmem_long_atomic_inc_func p_shmem_long_atomic_inc = nullptr;
shmem_long_atomic_fetch_add_func p_shmem_long_atomic_fetch_add = nullptr;
shmem_long_atomic_add_func p_shmem_long_atomic_add = nullptr;
shmem_long_atomic_fetch_and_func p_shmem_long_atomic_fetch_and = nullptr;
shmem_long_atomic_and_func p_shmem_long_atomic_and = nullptr;
shmem_long_atomic_fetch_or_func p_shmem_long_atomic_fetch_or = nullptr;
shmem_long_atomic_or_func p_shmem_long_atomic_or = nullptr;
shmem_long_atomic_fetch_xor_func p_shmem_long_atomic_fetch_xor = nullptr;
shmem_long_atomic_xor_func p_shmem_long_atomic_xor = nullptr;
shmem_long_atomic_fetch_nbi_func p_shmem_long_atomic_fetch_nbi = nullptr;
shmem_long_atomic_compare_swap_nbi_func p_shmem_long_atomic_compare_swap_nbi = nullptr;
shmem_long_atomic_swap_nbi_func p_shmem_long_atomic_swap_nbi = nullptr;
shmem_long_atomic_fetch_inc_nbi_func p_shmem_long_atomic_fetch_inc_nbi = nullptr;
shmem_long_atomic_fetch_add_nbi_func p_shmem_long_atomic_fetch_add_nbi = nullptr;
shmem_long_atomic_fetch_and_nbi_func p_shmem_long_atomic_fetch_and_nbi = nullptr;
shmem_long_atomic_fetch_or_nbi_func p_shmem_long_atomic_fetch_or_nbi = nullptr;
shmem_long_atomic_fetch_xor_nbi_func p_shmem_long_atomic_fetch_xor_nbi = nullptr;

/* Signaling Operations */
shmem_signal_fetch_func p_shmem_signal_fetch = nullptr;
shmem_long_put_signal_func p_shmem_long_put_signal = nullptr;
shmem_long_put_signal_nbi_func p_shmem_long_put_signal_nbi = nullptr;

/* Collective Routines */
shmem_sync_func p_shmem_sync = nullptr;
shmem_sync_all_func p_shmem_sync_all = nullptr;
shmem_alltoall_func p_shmem_alltoall = nullptr;
shmem_alltoalls_func p_shmem_alltoalls = nullptr;
shmem_broadcast_func p_shmem_broadcast = nullptr;
shmem_collect_func p_shmem_collect = nullptr;
shmem_fcollect_func p_shmem_fcollect = nullptr;
shmem_max_reduce_func p_shmem_max_reduce = nullptr;
shmem_min_reduce_func p_shmem_min_reduce = nullptr;
shmem_sum_reduce_func p_shmem_sum_reduce = nullptr;
shmem_prod_reduce_func p_shmem_prod_reduce = nullptr;

/* Point-to-Point Synchronization Routines */
shmem_wait_until_func p_shmem_wait_until = nullptr;
shmem_wait_until_all_func p_shmem_wait_until_all = nullptr;
shmem_wait_until_any_func p_shmem_wait_until_any = nullptr;
shmem_wait_until_some_func p_shmem_wait_until_some = nullptr;
shmem_wait_until_all_vector_func p_shmem_wait_until_all_vector = nullptr;
shmem_wait_until_any_vector_func p_shmem_wait_until_any_vector = nullptr;
shmem_wait_until_some_vector_func p_shmem_wait_until_some_vector = nullptr;
shmem_test_func p_shmem_test = nullptr;
shmem_test_all_func p_shmem_test_all = nullptr;
shmem_test_any_func p_shmem_test_any = nullptr;
shmem_test_some_func p_shmem_test_some = nullptr;
shmem_test_all_vector_func p_shmem_test_all_vector = nullptr;
shmem_test_any_vector_func p_shmem_test_any_vector = nullptr;
shmem_test_some_vector_func p_shmem_test_some_vector = nullptr;
shmem_signal_wait_until_func p_shmem_signal_wait_until = nullptr;

/* Memory Ordering Routines */
shmem_quiet_func p_shmem_quiet = nullptr;
shmem_fence_func p_shmem_fence = nullptr;

/* Distributed Locking Routines */
shmem_set_lock_func p_shmem_set_lock = nullptr;
shmem_clear_lock_func p_shmem_clear_lock = nullptr;

/**
  @brief Loads the OpenSHMEM routines dynamically.
  @return True if successful, false if otherwise
 */
bool load_routines() {
  void *handle = dlopen(NULL, RTLD_LAZY);
  if (!handle) {
    std::cerr << "Failed to open handle: " << dlerror() << std::endl;
    return false;
  }

  p_shmem_fake_routine = reinterpret_cast<shmem_fake_routine_func>(dlsym(handle, "shmem_fake_routine"));

  /* Setup, Exit, and Query Routines */
  p_shmem_init = reinterpret_cast<shmem_init_func>(dlsym(handle, "shmem_init"));
  p_shmem_finalize = reinterpret_cast<shmem_finalize_func>(dlsym(handle, "shmem_finalize"));
  p_shmem_my_pe = reinterpret_cast<shmem_my_pe_func>(dlsym(handle, "shmem_my_pe"));
  p_shmem_n_pes = reinterpret_cast<shmem_n_pes_func>(dlsym(handle, "shmem_n_pes"));
  p_shmem_pe_accessible = reinterpret_cast<shmem_pe_accessible_func>(dlsym(handle, "shmem_pe_accessible"));
  p_shmem_barrier_all = reinterpret_cast<shmem_barrier_all_func>(dlsym(handle, "shmem_barrier_all"));
  p_shmem_barrier = reinterpret_cast<shmem_barrier_func>(dlsym(handle, "shmem_barrier"));
  p_shmem_info_get_version = reinterpret_cast<shmem_info_get_version_func>(dlsym(handle, "shmem_info_get_version"));
  p_shmem_info_get_name = reinterpret_cast<shmem_info_get_name_func>(dlsym(handle, "shmem_info_get_name"));
  p_shmem_global_exit = reinterpret_cast<shmem_global_exit_func>(dlsym(handle, "shmem_global_exit"));

  /* Thread Support Routines */
  p_shmem_init_thread = reinterpret_cast<shmem_init_thread_func>(dlsym(handle, "shmem_init_thread"));
  p_shmem_query_thread = reinterpret_cast<shmem_query_thread_func>(dlsym(handle, "shmem_query_thread"));

  /* Memory Management Routines */
  p_shmem_ptr = reinterpret_cast<shmem_ptr_func>(dlsym(handle, "shmem_ptr"));
  p_shmem_malloc = reinterpret_cast<shmem_malloc_func>(dlsym(handle, "shmem_malloc"));
  p_shmem_free = reinterpret_cast<shmem_free_func>(dlsym(handle, "shmem_free"));
  p_shmem_realloc = reinterpret_cast<shmem_realloc_func>(dlsym(handle, "shmem_realloc"));
  p_shmem_align = reinterpret_cast<shmem_align_func>(dlsym(handle, "shmem_align"));
  p_shmem_malloc_with_hints = reinterpret_cast<shmem_malloc_with_hints_func>(dlsym(handle, "shmem_malloc_with_hints"));
  p_shmem_calloc = reinterpret_cast<shmem_calloc_func>(dlsym(handle, "shmem_calloc"));
  p_shmem_addr_accessible = reinterpret_cast<shmem_addr_accessible_func>(dlsym(handle, "shmem_addr_accessible"));

  /* Team Management Routines */
  p_shmem_team_my_pe = reinterpret_cast<shmem_team_my_pe_func>(dlsym(handle, "shmem_team_my_pe"));
  p_shmem_team_n_pes = reinterpret_cast<shmem_team_n_pes_func>(dlsym(handle, "shmem_team_n_pes"));
  p_shmem_team_get_config = reinterpret_cast<shmem_team_get_config_func>(dlsym(handle, "shmem_team_get_config"));
  p_shmem_team_translate_pe = reinterpret_cast<shmem_team_translate_pe_func>(dlsym(handle, "shmem_team_translate_pe"));
  p_shmem_team_split_strided = reinterpret_cast<shmem_team_split_strided_func>(dlsym(handle, "shmem_team_split_strided"));
  p_shmem_team_split_2d = reinterpret_cast<shmem_team_split_2d_func>(dlsym(handle, "shmem_team_split_2d"));
  p_shmem_team_destroy = reinterpret_cast<shmem_team_destroy_func>(dlsym(handle, "shmem_team_destroy"));

  /* Communication/Context Management Routines */
  p_shmem_ctx_create = reinterpret_cast<shmem_ctx_create_func>(dlsym(handle, "shmem_ctx_create"));
  p_shmem_team_create_ctx = reinterpret_cast<shmem_team_create_ctx_func>(dlsym(handle, "shmem_team_create_ctx"));
  p_shmem_ctx_destroy = reinterpret_cast<shmem_ctx_destroy_func>(dlsym(handle, "shmem_ctx_destroy"));
  p_shmem_ctx_get_team = reinterpret_cast<shmem_ctx_get_team_func>(dlsym(handle, "shmem_ctx_get_team"));

  /* Remote Access Routines */
  p_shmem_long_put = reinterpret_cast<shmem_long_put_func>(dlsym(handle, "shmem_long_put"));
  p_shmem_long_p = reinterpret_cast<shmem_long_p_func>(dlsym(handle, "shmem_long_p"));
  p_shmem_long_iput = reinterpret_cast<shmem_long_iput_func>(dlsym(handle, "shmem_long_iput"));
  p_shmem_long_get = reinterpret_cast<shmem_long_get_func>(dlsym(handle, "shmem_long_get"));
  p_shmem_long_g = reinterpret_cast<shmem_long_g_func>(dlsym(handle, "shmem_long_g"));
  p_shmem_long_iget = reinterpret_cast<shmem_long_iget_func>(dlsym(handle, "shmem_long_iget"));
  p_shmem_long_put_nbi = reinterpret_cast<shmem_long_put_nbi_func>(dlsym(handle, "shmem_long_put_nbi"));
  p_shmem_long_get_nbi = reinterpret_cast<shmem_long_get_nbi_func>(dlsym(handle, "shmem_long_get_nbi"));

  /* Atomic Memory Operations */
  p_shmem_long_atomic_fetch = reinterpret_cast<shmem_long_atomic_fetch_func>(dlsym(handle, "shmem_long_atomic_fetch"));
  p_shmem_long_atomic_set = reinterpret_cast<shmem_long_atomic_set_func>(dlsym(handle, "shmem_long_atomic_set"));
  p_shmem_long_atomic_compare_swap = reinterpret_cast<shmem_long_atomic_compare_swap_func>(dlsym(handle, "shmem_long_atomic_compare_swap"));
  p_shmem_long_atomic_swap = reinterpret_cast<shmem_long_atomic_swap_func>(dlsym(handle, "shmem_long_atomic_swap"));
  p_shmem_long_atomic_fetch_inc = reinterpret_cast<shmem_long_atomic_fetch_inc_func>(dlsym(handle, "shmem_long_atomic_fetch_inc"));
  p_shmem_long_atomic_inc = reinterpret_cast<shmem_long_atomic_inc_func>(dlsym(handle, "shmem_long_atomic_inc"));
  p_shmem_long_atomic_fetch_add = reinterpret_cast<shmem_long_atomic_fetch_add_func>(dlsym(handle, "shmem_long_atomic_fetch_add"));
  p_shmem_long_atomic_add = reinterpret_cast<shmem_long_atomic_add_func>(dlsym(handle, "shmem_long_atomic_add"));
  p_shmem_long_atomic_fetch_and = reinterpret_cast<shmem_long_atomic_fetch_and_func>(dlsym(handle, "shmem_long_atomic_fetch_and"));
  p_shmem_long_atomic_and = reinterpret_cast<shmem_long_atomic_and_func>(dlsym(handle, "shmem_long_atomic_and"));
  p_shmem_long_atomic_fetch_or = reinterpret_cast<shmem_long_atomic_fetch_or_func>(dlsym(handle, "shmem_long_atomic_fetch_or"));
  p_shmem_long_atomic_or = reinterpret_cast<shmem_long_atomic_or_func>(dlsym(handle, "shmem_long_atomic_or"));
  p_shmem_long_atomic_fetch_xor = reinterpret_cast<shmem_long_atomic_fetch_xor_func>(dlsym(handle, "shmem_long_atomic_fetch_xor"));
  p_shmem_long_atomic_xor = reinterpret_cast<shmem_long_atomic_xor_func>(dlsym(handle, "shmem_long_atomic_xor"));
  p_shmem_long_atomic_fetch_nbi = reinterpret_cast<shmem_long_atomic_fetch_nbi_func>(dlsym(handle, "shmem_long_atomic_fetch_nbi"));
  p_shmem_long_atomic_compare_swap_nbi = reinterpret_cast<shmem_long_atomic_compare_swap_nbi_func>(dlsym(handle, "shmem_long_atomic_compare_swap_nbi"));
  p_shmem_long_atomic_swap_nbi = reinterpret_cast<shmem_long_atomic_swap_nbi_func>(dlsym(handle, "shmem_long_atomic_swap_nbi"));
  p_shmem_long_atomic_fetch_inc_nbi = reinterpret_cast<shmem_long_atomic_fetch_inc_nbi_func>(dlsym(handle, "shmem_long_atomic_fetch_inc_nbi"));
  p_shmem_long_atomic_fetch_add_nbi = reinterpret_cast<shmem_long_atomic_fetch_add_nbi_func>(dlsym(handle, "shmem_long_atomic_fetch_add_nbi"));
  p_shmem_long_atomic_fetch_and_nbi = reinterpret_cast<shmem_long_atomic_fetch_and_nbi_func>(dlsym(handle, "shmem_long_atomic_fetch_and_nbi"));
  p_shmem_long_atomic_fetch_or_nbi = reinterpret_cast<shmem_long_atomic_fetch_or_nbi_func>(dlsym(handle, "shmem_long_atomic_fetch_or_nbi"));
  p_shmem_long_atomic_fetch_xor_nbi = reinterpret_cast<shmem_long_atomic_fetch_xor_nbi_func>(dlsym(handle, "shmem_long_atomic_fetch_xor_nbi"));

  /* Signaling Operations */
  p_shmem_signal_fetch = reinterpret_cast<shmem_signal_fetch_func>(dlsym(handle, "shmem_signal_fetch"));
  p_shmem_long_put_signal = reinterpret_cast<shmem_long_put_signal_func>(dlsym(handle, "shmem_long_put_signal"));
  p_shmem_long_put_signal_nbi = reinterpret_cast<shmem_long_put_signal_nbi_func>(dlsym(handle, "shmem_long_put_signal_nbi"));

  /* Collective Routines */
  p_shmem_sync = reinterpret_cast<shmem_sync_func>(dlsym(handle, "shmem_sync"));
  p_shmem_sync_all = reinterpret_cast<shmem_sync_all_func>(dlsym(handle, "shmem_sync_all"));
  p_shmem_alltoall = reinterpret_cast<shmem_alltoall_func>(dlsym(handle, "shmem_alltoall"));
  p_shmem_alltoalls = reinterpret_cast<shmem_alltoalls_func>(dlsym(handle, "shmem_alltoalls"));
  p_shmem_broadcast = reinterpret_cast<shmem_broadcast_func>(dlsym(handle, "shmem_broadcast"));
  p_shmem_collect = reinterpret_cast<shmem_collect_func>(dlsym(handle, "shmem_collect"));
  p_shmem_fcollect = reinterpret_cast<shmem_fcollect_func>(dlsym(handle, "shmem_fcollect"));
  p_shmem_max_reduce = reinterpret_cast<shmem_max_reduce_func>(dlsym(handle, "shmem_max_reduce"));
  p_shmem_min_reduce = reinterpret_cast<shmem_min_reduce_func>(dlsym(handle, "shmem_min_reduce"));
  p_shmem_sum_reduce = reinterpret_cast<shmem_sum_reduce_func>(dlsym(handle, "shmem_sum_reduce"));
  p_shmem_prod_reduce = reinterpret_cast<shmem_prod_reduce_func>(dlsym(handle, "shmem_prod_reduce"));

  /* Point-to-Point Synchronization Routines */
  p_shmem_wait_until = reinterpret_cast<shmem_wait_until_func>(dlsym(handle, "shmem_wait_until"));
  p_shmem_wait_until_all = reinterpret_cast<shmem_wait_until_all_func>(dlsym(handle, "shmem_wait_until_all"));
  p_shmem_wait_until_any = reinterpret_cast<shmem_wait_until_any_func>(dlsym(handle, "shmem_wait_until_any"));
  p_shmem_wait_until_some = reinterpret_cast<shmem_wait_until_some_func>(dlsym(handle, "shmem_wait_until_some"));
  p_shmem_wait_until_all_vector = reinterpret_cast<shmem_wait_until_all_vector_func>(dlsym(handle, "shmem_wait_until_all_vector"));
  p_shmem_wait_until_any_vector = reinterpret_cast<shmem_wait_until_any_vector_func>(dlsym(handle, "shmem_wait_until_any_vector"));
  p_shmem_wait_until_some_vector = reinterpret_cast<shmem_wait_until_some_vector_func>(dlsym(handle, "shmem_wait_until_some_vector"));
  p_shmem_test = reinterpret_cast<shmem_test_func>(dlsym(handle, "shmem_test"));
  p_shmem_test_all = reinterpret_cast<shmem_test_all_func>(dlsym(handle, "shmem_test_all"));
  p_shmem_test_any = reinterpret_cast<shmem_test_any_func>(dlsym(handle, "shmem_test_any"));
  p_shmem_test_some = reinterpret_cast<shmem_test_some_func>(dlsym(handle, "shmem_test_some"));
  p_shmem_test_all_vector = reinterpret_cast<shmem_test_all_vector_func>(dlsym(handle, "shmem_test_all_vector"));
  p_shmem_test_any_vector = reinterpret_cast<shmem_test_any_vector_func>(dlsym(handle, "shmem_test_any_vector"));
  p_shmem_test_some_vector = reinterpret_cast<shmem_test_some_vector_func>(dlsym(handle, "shmem_test_some_vector"));
  p_shmem_signal_wait_until = reinterpret_cast<shmem_signal_wait_until_func>(dlsym(handle, "shmem_signal_wait_until"));

  /* Memory Ordering Routines */
  p_shmem_quiet = reinterpret_cast<shmem_quiet_func>(dlsym(handle, "shmem_quiet"));
  p_shmem_fence = reinterpret_cast<shmem_fence_func>(dlsym(handle, "shmem_fence"));

  /* Distributed Locking Routines */
  p_shmem_set_lock = reinterpret_cast<shmem_set_lock_func>(dlsym(handle, "shmem_set_lock"));
  p_shmem_clear_lock = reinterpret_cast<shmem_clear_lock_func>(dlsym(handle, "shmem_clear_lock"));

  const char *dlsym_error = dlerror();
  if (dlsym_error) {
    std::cerr << "Error loading functions: " << dlsym_error << std::endl;
    dlclose(handle);
    return false;
  }

  return true;
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
    #ifdef _DEBUG_
      if (mype == 0) {
        std::cerr << "Function " << routine_name << " not found: " << dlsym_error << std::endl;
      }
    #endif
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
