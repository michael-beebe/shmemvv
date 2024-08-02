/**
  @file  main.cpp
  @brief Driver file for the test suite.
*/

#include "shmemvv.h"
#include "routines.h"
#include "tests.h"

/**
  @brief Main function for running the test suite.
  @param argc Number of command-line arguments.
  @param argv Array of command-line argument strings.
  @return EXIT_SUCCESS on success, EXIT_FAILURE on failure.
*/
int main(int argc, char *argv[]) {
  int mype = 0;
  int npes = 0;
  char version[100] = "";
  char name[100] = "";
  test_options opts;

  void *handle = dlopen(NULL, RTLD_LAZY);
  if (!handle) {
    if (mype == 0) {
      std::cerr << "Failed to open handle: " << dlerror() << std::endl;
      return EXIT_FAILURE;
    }
  }

////////////////////////////////////////////////////////////////////////
  // /* Load OpenSHMEM routines */
  // if (!load_routines()) {
  //   std::cerr << "Failed to load OpenSHMEM routines" << std::endl;
  //   return EXIT_FAILURE;
  // }
////////////////////////////////////////////////////////////////////////

  /* Parse command-line options */
  if (!parse_opts(argc, argv, &opts)) {
    if (mype == 0) {
      display_help();
    }
    return EXIT_FAILURE;
  }

  /* Enable all tests if --all is specified or no specific test is selected */
  if (opts.test_all ||
      !(opts.test_setup || opts.test_threads || opts.test_mem ||
        opts.test_teams || opts.test_ctx || opts.test_remote ||
        opts.test_atomics || opts.test_signaling || opts.test_collectives ||
        opts.test_pt2pt_synch || opts.test_mem_ordering || opts.test_locking)
  ) {
    opts.test_setup = true; opts.test_threads = true; opts.test_mem = true;
    opts.test_teams = true; opts.test_ctx = true; opts.test_remote = true;
    opts.test_atomics = true; opts.test_signaling = true; opts.test_collectives = true;
    opts.test_pt2pt_synch = true; opts.test_mem_ordering = true; opts.test_locking = true;
  }

  /* Display help if requested */
  if (opts.help) {
    if (mype == 0) {
      display_help();
    }
    return EXIT_SUCCESS;
  }
  
  /************************* SETUP **************************/
  if ( !run_setup_tests(opts, mype, npes, version, name) ) {
    return EXIT_FAILURE;
  }

  /************************* THREADS TESTS **************************/
  if (opts.test_threads) {
    shmem_barrier_all();
    if (mype == 0) {
      display_test_header("THREADS"); 
    }
    shmem_barrier_all();
    run_threads_tests(mype, npes);
  }

  /************************* MEMORY TESTS **************************/
  if (opts.test_mem) {
    shmem_barrier_all();
    if (mype == 0) {
      display_test_header("MEMORY MANAGEMENT"); 
    }
    shmem_barrier_all();
    run_mem_tests(mype, npes);
  }

  /************************* TEAMS TESTS **************************/
  if (opts.test_teams) {
    shmem_barrier_all();
    if (mype == 0) {
      display_test_header("TEAMS MANAGEMENT"); 
    }
    shmem_barrier_all();
    run_teams_tests(mype, npes);
  }

  /************************* CTX TESTS **************************/
  if (opts.test_ctx) {
    shmem_barrier_all();
    if (mype == 0) {
      display_test_header("COMMUNICATION / CONTEXT"); 
    }
    shmem_barrier_all();
    run_comms_tests(mype, npes);
  }

  /************************* REMOTE TESTS **************************/
  if (opts.test_remote) {
    shmem_barrier_all();
    if (mype == 0) {
      display_test_header("REMOTE MEMORY ACCESS"); 
    }
    shmem_barrier_all();
    run_c11_remote_tests(mype, npes);
    if (mype == 0) { printf("\n"); }
    run_cxx_remote_tests(mype, npes);
  }

  /************************* ATOMICS TESTS **************************/
  if (opts.test_atomics) {
    shmem_barrier_all();
    if (mype == 0) {
      display_test_header("ATOMIC MEMORY OPS"); 
    }
    run_c11_atomics_tests(mype, npes);
    if (mype == 0) { printf("\n"); }
    run_cxx_atomics_tests(mype, npes);
  }
  
  /************************* SIGNALING TESTS **************************/
  if (opts.test_signaling) {
    shmem_barrier_all();
    if (mype == 0) {
      display_test_header("SIGNALING OPS");
    }
    run_c11_signaling_tests(mype, npes);
    if (mype == 0) { printf("\n"); }
    run_cxx_signaling_tests(mype, npes);
  }

  /************************* COLLECTIVES TESTS **************************/
  if (opts.test_collectives) {
    shmem_barrier_all();
    if (mype == 0) {
      display_test_header("COLLECTIVE OPS");
    }
    shmem_barrier_all();
    run_c11_collectives_tests(mype, npes);
    if (mype == 0) { printf("\n"); }
    run_cxx_collectives_tests(mype, npes);
  }

  /************************* PT2PT TESTS **************************/
  if (opts.test_pt2pt_synch) {
    shmem_barrier_all();
    if (mype == 0) {
      display_test_header("POINT-TO-POINT SYNC OPS");
    }
    shmem_barrier_all();
    run_c11_pt2pt_synch_tests(mype, npes);
    if (mype == 0) { printf("\n"); }
    run_cxx_pt2pt_synch_tests(mype, npes);
  }

  /************************* MEM ORDERING TESTS **************************/
  if (opts.test_mem_ordering) {
    shmem_barrier_all();
    if (mype == 0) {
      display_test_header("MEMORY ORDERING");
    }
    shmem_barrier_all();
    run_mem_ordering_tests(mype, npes);
  }

  /************************* DISTRIBUTED LOCKING TESTS **************************/
  if (opts.test_locking) {
    shmem_barrier_all();
    if (mype == 0) {
      display_test_header("DISTRIBUTED LOCKING");
    }
    shmem_barrier_all();
    run_locking_tests(mype, npes);
  }

  /************************* FINALIZATION **************************/
  shmem_barrier_all();

  if ( !check_if_exists("shmem_finalize") ) {
    display_not_found_warning("shmem_finalize()", true);
  }
  else {
    if (mype == 0) {
      display_test_header("FINALIZATION");
      display_test_result("shmem_finalize()", test_shmem_finalize(), false);
      std::cout << std::endl;
    }
  }

  /* We made it! End the program. */
  return EXIT_SUCCESS;
}
