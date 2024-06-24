/**
  @file  main.cpp
  @brief Driver file for the test suite.
*/

#include "shmemvv.hpp"

int main(int argc, char *argv[]) {
  shmem_init();
  int mype = shmem_my_pe();

  test_options opts;

  /* Parse command-line options */
  if (!parse_opts(argc, argv, opts)) {
    if (mype == 0) { display_help(); }
    return EXIT_FAILURE;
  }

  /* Display help if requested */
  if (opts.help) {
    if (mype == 0) { display_help(); }
    return EXIT_SUCCESS;
  }

  /* Display ASCII art logo */
  if (mype == 0) { display_logo(); }

  /* Enable all tests if --all is specified or no specific test is selected */
  if (
    opts.all ||
    !(opts.test_setup || opts.test_threads || opts.test_mem || opts.test_teams ||
      opts.test_comms || opts.test_remote || opts.test_atomics || opts.test_signaling ||
      opts.test_collectives || opts.test_pt2pt_synch || opts.test_mem_ordering || opts.test_locking)
  ) {
    opts.test_setup = true;
    opts.test_threads = true;
    opts.test_mem = true;
    opts.test_teams = true;
    opts.test_comms = true;
    opts.test_remote = true;
    opts.test_atomics = true;
    opts.test_signaling = true;
    opts.test_collectives = true;
    opts.test_pt2pt_synch = true;
    opts.test_mem_ordering = true;
    opts.test_locking = true;
  }

  /* Include headers and execute selected tests */
  if (opts.test_setup) {

    if (mype == 0) {
      std::cout << "--------------------------------------" << std::endl;
      std::cout << "      Running setup tests..." << std::endl;
      std::cout << "--------------------------------------" << std::endl;
    }
    if ( !test_shmem_init() ) {
      if (mype == 0) {
        std::cerr << "shmem_init() test failed." << std::endl;
      }
    }
    else {
      if (mype == 0) {
        std::cout << "shmem_init() test passed!" << std::endl;
      }
    }
    if ( !test_shmem_finalize() ) {
      if (mype == 0) {
        std::cerr << "shmem_finalize() test failed." << std::endl;
      }
    }
    else {
      if (mype == 0) {
        std::cout << "shmem_finalize() test passed!" << std::endl;
      }
    }
    if (!test_shmem_my_pe()) {
      if (mype == 0) {
        std::cerr << "shmem_my_pe() test failed." << std::endl;
      }
    }
    else {
      if (mype == 0) {
        std::cout << "shmem_my_pe() test passed!" << std::endl;
      }
    }
    if (!test_shmem_n_pes()) {
      if (mype == 0) {
        std::cerr << "shmem_n_pes() test failed." << std::endl;
      }
    }
    else {
      if (mype == 0) {
        std::cout << "shmem_n_pes() test passed!" << std::endl;
      }
    }

  }

  if (opts.test_threads) {
    std::cout << "Running thread support tests...\n";
    /* TODO: Call thread support tests here */
  }

  if (opts.test_mem) {
    std::cout << "Running memory management tests...\n";
    /* TODO: Call memory management tests here */
  }

  if (opts.test_teams) {
    std::cout << "Running team management tests...\n";
    /* TODO: Call team management tests here */
  }

  if (opts.test_comms) {
    std::cout << "Running communication management tests...\n";
    /* TODO: Call communication management tests here */
  }

  if (opts.test_remote) {
    std::cout << "Running remote memory access tests...\n";
    /* TODO: Call remote memory access tests here */
  }

  if (opts.test_atomics) {
    std::cout << "Running atomic memory operations tests...\n";
    /* TODO: Call atomic memory operations tests here */
  }

  if (opts.test_signaling) {
    std::cout << "Running signaling operations tests...\n";
    /* TODO: Call signaling operations tests here */
  }

  if (opts.test_collectives) {
    std::cout << "Running collective operations tests...\n";
    /* TODO: Call collective operations tests here */
  }

  if (opts.test_pt2pt_synch) {
    std::cout << "Running point-to-point synchronization tests...\n";
    /* TODO: Call point-to-point synchronization tests here */
  }

  if (opts.test_mem_ordering) {
    std::cout << "Running memory ordering tests...\n";
    /* TODO: Call memory ordering tests here */
  }

  if (opts.test_locking) {
    std::cout << "Running distributed locking tests...\n";
    /* TODO: Call distributed locking tests here */
  }

  shmem_finalize();
  return EXIT_SUCCESS;
}
