/**
  @file  main.cpp
  @brief Driver file for the test suite.
*/

#include "shmemvv.hpp"

/* ANSI color codes for pretty output */
#define RESET_COLOR "\033[0m"
#define RED_COLOR "\033[31m"
#define GREEN_COLOR "\033[32m"
#define YELLOW_COLOR "\033[33m"

/**
  @brief Main function for running the test suite.
  @param argc Number of command-line arguments.
  @param argv Array of command-line argument strings.
  @return EXIT_SUCCESS on success, EXIT_FAILURE on failure.
*/
int main(int argc, char *argv[]) {
  /************************* BEGINNING OF SETUP **************************/
  int mype;
  int npes;
  std::string version = "";
  std::string name = "";
  test_options opts;

  /* Run shmem_init() test */
  if (!test_shmem_init()) {
    if (shmem_my_pe() == 0) {
      std::cerr << RED_COLOR << "shmem_init() test FAILED! This test must pass to continue!" << RESET_COLOR << std::endl;
    }
    return EXIT_FAILURE;
  }

  if (!test_shmem_barrier_all()) {
    if (shmem_my_pe() == 0) {
      std::cerr << RED_COLOR << "shmem_barrier_all() test FAILED! This test must pass to continue!" << RESET_COLOR << std::endl;
    }
    shmem_finalize();
    return EXIT_FAILURE;
  }
  shmem_barrier_all();

  /* If shmem_init passes, run shmem_my_pe() test */
  mype = test_shmem_my_pe();
  if (mype < 0) {
    if (mype == 0) {
      std::cerr << RED_COLOR << "shmem_my_pe() test FAILED! This test must pass to continue!" << RESET_COLOR << std::endl;
    }
    shmem_finalize();
    return EXIT_FAILURE;
  }
  shmem_barrier_all();

  npes = test_shmem_n_pes();
  if (npes <= 0) {
    if (mype == 0) {
      std::cerr << RED_COLOR << "shem_n_pes() test FAILED! This test must pass to continue!" << RESET_COLOR << std::endl;
    }
    shmem_finalize();
    return EXIT_FAILURE;
  }
  shmem_barrier_all();

  /* Parse command-line options */
  if (!parse_opts(argc, argv, opts)) {
    if (mype == 0) {
      display_help();
    }
    return EXIT_FAILURE;
  }
  shmem_barrier_all();

  /* Display help if requested */
  if (opts.help) {
    if (mype == 0) {
      display_help();
    }
    return EXIT_SUCCESS;
  }
  shmem_barrier_all();

  /* Display ASCII art logo */
  if (mype == 0) {
    display_logo();
  }
  shmem_barrier_all();

  /* Print setup tests header */
  if (mype == 0) {
    display_test_header("SETUP");
  }
  shmem_barrier_all();

  /* shmem_init() and shmem_my_pe() tests passed */
  if (mype == 0) {
    std::cout << GREEN_COLOR << "shmem_init() test PASSED!" << RESET_COLOR << std::endl;
    std::cout << GREEN_COLOR << "shmem_barrier_all() test PASSED!" << RESET_COLOR << std::endl;
    std::cout << GREEN_COLOR << "shmem_my_pe() test PASSED!" << RESET_COLOR << std::endl;
    std::cout << GREEN_COLOR << "shmem_n_pes() test PASSED!" << RESET_COLOR << std::endl;
  }
  shmem_barrier_all();

  /* Run shmem_pe_accessible() test */
  if (!test_shmem_pe_accessible()) {
    if (mype == 0) {
      std::cerr << RED_COLOR << "shmem_pe_accessible() test FAILED!" << RESET_COLOR << std::endl;
    }
  }
  else {
    if (mype == 0) {
      std::cout << GREEN_COLOR << "shmem_pe_accessible() test PASSED!" << RESET_COLOR << std::endl;
    }
  }
  shmem_barrier_all();

  /* Run shmem_ptr() test */
  if (!test_shmem_ptr()) {
    if (mype == 0) {
      std::cerr << RED_COLOR << "shmem_ptr() test FAILED!" << RESET_COLOR << std::endl;
    }
  } 
  else {
    if (mype == 0) {
      std::cout << GREEN_COLOR << "shmem_ptr() test PASSED!" << RESET_COLOR << std::endl;
    }
  }
  shmem_barrier_all();

  /* Run shmem_info_get_version() test */
  version = test_shmem_info_get_version();
  if (version == "") {
    if (mype == 0) {
      std::cerr << RED_COLOR << "shmem_info_get_version() test FAILED!" << RESET_COLOR << std::endl;
    }
  }
  else if (version != "1.5" && version != "1.50") {
    if (mype == 0) {
      std::cerr << YELLOW_COLOR << "shmem_info_get_version() test did not return 1.5... Returned " << version << std::endl;
    }
  }
  else {
    if (mype == 0) {
      std::cout << GREEN_COLOR << "shmem_info_get_version() test PASSED!" << RESET_COLOR << std::endl;
    }
  }
  shmem_barrier_all();

  /* Run shmem_info_get_name() test */
  name = test_shmem_info_get_name();
  if (name == "") {
    if (mype == 0) {
      std::cerr << RED_COLOR << "shmem_info_get_name() test FAILED!" << RESET_COLOR << std::endl;
    }
  }
  else {
    if (mype == 0) {
      std::cout << GREEN_COLOR << "shmem_info_get_name() test PASSED!" << RESET_COLOR << std::endl;
    }
  }
  shmem_barrier_all();

  /* Display test information */
  if (mype == 0) { display_test_info(name, version, npes); }

  /************************* END OF SETUP **************************/

  shmem_barrier_all();

  /* Enable all tests if --all is specified or no specific test is selected */
  if (
    opts.all ||
    !(opts.test_setup || opts.test_threads || opts.test_mem || opts.test_teams ||
      opts.test_comms || opts.test_remote || opts.test_atomics || opts.test_signaling ||
      opts.test_collectives || opts.test_pt2pt_synch || opts.test_mem_ordering || opts.test_locking)
  ) {
    opts.test_setup = true; opts.test_threads = true; opts.test_mem = true;
    opts.test_teams = true; opts.test_comms = true; opts.test_remote = true;
    opts.test_atomics = true; opts.test_signaling = true; opts.test_collectives = true;
    opts.test_pt2pt_synch = true; opts.test_mem_ordering = true; opts.test_locking = true;
  }

  shmem_barrier_all();
  if (opts.test_threads) {
    if (mype == 0) { display_test_header("THREADS"); }

    if (!test_shmem_init_thread()) {
      if (mype == 0) {
        std::cout << RED_COLOR << "shmem_init_thread() test FAILED!" << RESET_COLOR << std::endl;
      }
    }
    else {
      if (mype == 0) {
        std::cout << GREEN_COLOR << "shmem_init_thread() test PASSED!" << RESET_COLOR << std::endl;
      }
    }
  }

  shmem_barrier_all();
  if (opts.test_mem) {
    if (mype == 0) { display_test_header("MEMORY MANAGEMENT"); }
    /* TODO: Call memory management tests here */
  }

  shmem_barrier_all();
  if (opts.test_teams) {
    if (mype == 0) { display_test_header("TEAMS MANAGMENT"); }
    /* TODO: Call team management tests here */
  }

  shmem_barrier_all();
  if (opts.test_comms) {
    if (mype == 0) { display_test_header("COMMUNICATION MANAGEMENT"); }
    /* TODO: Call communication management tests here */
  }

  shmem_barrier_all();
  if (opts.test_remote) {
    if (mype == 0) { display_test_header("REMOTE MEMORY ACCESS"); }
    /* TODO: Call remote memory access tests here */
  }

  shmem_barrier_all();
  if (opts.test_atomics) {
    if (mype == 0) { display_test_header("ATOMIC MEMORY OPS"); }
    /* TODO: Call atomic memory operations tests here */
  }

  shmem_barrier_all();
  if (opts.test_signaling) {
    if (mype == 0) { display_test_header("SIGNALING OPS"); }
    /* TODO: Call signaling operations tests here */
  }

  shmem_barrier_all();
  if (opts.test_collectives) {
    if (mype == 0) { display_test_header("COLLECTIVE OPS"); }
    /* TODO: Call collective operations tests here */
  }

  shmem_barrier_all();
  if (opts.test_pt2pt_synch) {
    if (mype == 0) { display_test_header("PT2PT SYNCHRONIZATION"); }
    /* TODO: Call point-to-point synchronization tests here */
  }

  shmem_barrier_all();
  if (opts.test_mem_ordering) {
    if (mype == 0) { display_test_header("MEMORY ORDERING"); }
    /* TODO: Call memory ordering tests here */
  }

  shmem_barrier_all();
  if (opts.test_locking) {
    if (mype == 0) { display_test_header("DISTRIBUTED LOCKING"); }
    /* TODO: Call distributed locking tests here */
  }

  /* Run shmem_finalize() test */
  shmem_barrier_all();
  if (mype == 0) { display_test_header("FINALIZATION"); }
  if (!test_shmem_finalize()) {
    if (mype == 0) {
      std::cerr << RED_COLOR << "shmem_finalize() test FAILED!\n" << RESET_COLOR << std::endl;
    }
  }
  else {
    if (mype == 0) {
      std::cout << GREEN_COLOR << "shmem_finalize() test PASSED!\n" << RESET_COLOR << std::endl;
    }
  }

  return EXIT_SUCCESS;
}

