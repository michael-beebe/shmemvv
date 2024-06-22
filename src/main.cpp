#include "shmemvv.hpp"

int main(int argc, char *argv[]) {
  test_options opts;

  /* Parse command-line options */
  if (!parse_opts(argc, argv, opts)) {
    display_help();
    return EXIT_FAILURE;
  }

  /* Display help if requested */
  if (opts.help) {
    display_help();
    return EXIT_SUCCESS;
  }

  /* Display ASCII art logo */
  display_logo();

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
    #include "tests/setup/setup_tests.hpp"
    std::cout << "Running setup tests...\n";
    /* Call your setup tests here */
  }

  if (opts.test_threads) {
    #include "tests/threads/threads_tests.hpp"
    std::cout << "Running thread support tests...\n";
    /* Call your thread support tests here */
  }

  if (opts.test_mem) {
    #include "tests/mem/mem_tests.hpp"
    std::cout << "Running memory management tests...\n";
    /* Call your memory management tests here */
  }

  if (opts.test_teams) {
    #include "tests/teams/teams_tests.hpp"
    std::cout << "Running team management tests...\n";
    /* Call your team management tests here */
  }

  if (opts.test_comms) {
    #include "tests/comms/comms_tests.hpp"
    std::cout << "Running communication management tests...\n";
    /* Call your communication management tests here */
  }

  if (opts.test_remote) {
    #include "tests/remote/remote_tests.hpp"
    std::cout << "Running remote memory access tests...\n";
    /* Call your remote memory access tests here */
  }

  if (opts.test_atomics) {
    #include "tests/atomics/atomics_tests.hpp"
    std::cout << "Running atomic memory operations tests...\n";
    /* Call your atomic memory operations tests here */
  }

  if (opts.test_signaling) {
    #include "tests/signaling/signaling_tests.hpp"
    std::cout << "Running signaling operations tests...\n";
    /* Call your signaling operations tests here */
  }

  if (opts.test_collectives) {
    #include "tests/collectives/collectives_tests.hpp"
    std::cout << "Running collective operations tests...\n";
    /* Call your collective operations tests here */
  }

  if (opts.test_pt2pt_synch) {
    #include "tests/pt2pt/pt2pt_tests.hpp"
    std::cout << "Running point-to-point synchronization tests...\n";
    /* Call your point-to-point synchronization tests here */
  }

  if (opts.test_mem_ordering) {
    #include "tests/mem_ordering/mem_ordering_tests.hpp"
    std::cout << "Running memory ordering tests...\n";
    /* Call your memory ordering tests here */
  }

  if (opts.test_locking) {
    #include "tests/locking/locking_tests.hpp"
    std::cout << "Running distributed locking tests...\n";
    /* Call your distributed locking tests here */
  }

  return EXIT_SUCCESS;
}
