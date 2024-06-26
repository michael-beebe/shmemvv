/**
  @file  main.cpp
  @brief Driver file for the test suite.
*/

#include "shmemvv.hpp"

/**
  @brief Main function for running the test suite.
  @param argc Number of command-line arguments.
  @param argv Array of command-line argument strings.
  @return EXIT_SUCCESS on success, EXIT_FAILURE on failure.
*/
int main(int argc, char *argv[]) {
  /************************* START OF SETUP **************************/
  int mype;
  int npes;
  std::string version = "";
  std::string name = "";
  test_options opts;

  /* Parse command-line options */
  if (!parse_opts(argc, argv, opts)) {
    display_help();
    return EXIT_FAILURE;
  }

  /* Enable all tests if --all is specified or no specific test is selected */
  if (opts.all ||
      !(opts.test_setup || opts.test_threads || opts.test_mem || opts.test_teams ||
        opts.test_comms || opts.test_remote || opts.test_atomics || opts.test_signaling ||
        opts.test_collectives || opts.test_pt2pt_synch || opts.test_mem_ordering || opts.test_locking))
  {
    opts.test_setup = true; opts.test_threads = true; opts.test_mem = true; opts.test_teams = true;
    opts.test_comms = true; opts.test_remote = true; opts.test_atomics = true; opts.test_signaling = true;
    opts.test_collectives = true; opts.test_pt2pt_synch = true; opts.test_mem_ordering = true; opts.test_locking = true;
  }

  /* Initialize with shmem_init_thread() if THREADS tests were enabled */
  if (opts.test_threads) {
    if (!test_shmem_init_thread()) {
      if (shmem_my_pe() == 0) {
        std::cerr << RED_COLOR << "shmem_init_thread() test FAILED! This test must pass to continue!" << RESET_COLOR << std::endl;
      }
      return EXIT_FAILURE;
    }
  }
  else {
    /* Initialize with regular shmem_init() if THREADS tests are not enabled */
    if (!test_shmem_init()) {
      std::cerr << RED_COLOR << "shmem_init() test FAILED! This test must pass to continue!" << RESET_COLOR << std::endl;
      return EXIT_FAILURE;
    }
  }

  if (!test_shmem_barrier_all()) {
    if (shmem_my_pe() == 0) {
      std::cerr << RED_COLOR << "shmem_barrier_all() test FAILED! This test must pass to continue!" << RESET_COLOR << std::endl;
    }
    shmem_finalize();
    return EXIT_FAILURE;
  }

  /* Run shmem_my_pe() test */
  shmem_barrier_all();
  mype = test_shmem_my_pe();
  if (mype < 0) {
    if (mype == 0) {
      std::cerr << RED_COLOR << "shmem_my_pe() test FAILED! This test must pass to continue!" << RESET_COLOR << std::endl;
    }
    shmem_finalize();
    return EXIT_FAILURE;
  }

  /* Run shmem_n_pes() test */
  shmem_barrier_all();
  npes = test_shmem_n_pes();
  if (npes <= 0) {
    if (mype == 0) {
      std::cerr << RED_COLOR << "shmem_n_pes() test FAILED! This test must pass to continue!" << RESET_COLOR << std::endl;
    }
    shmem_finalize();
    return EXIT_FAILURE;
  }

  /* Display help if requested */
  shmem_barrier_all();
  if (opts.help) {
    if (mype == 0) {
      display_help();
    }
    shmem_finalize();
    return EXIT_SUCCESS;
  }

  /* Display ASCII art logo */
  shmem_barrier_all();
  if (mype == 0) {
    display_logo();
  }

  /* Print setup tests header */
  shmem_barrier_all();
  if (mype == 0) {
    display_test_header("SETUP");
  }

  /* shmem_init() and shmem_my_pe() tests passed */
  shmem_barrier_all();
  if (mype == 0) {
    if (!opts.test_threads) {
      std::cout << GREEN_COLOR << "shmem_init() test PASSED!" << RESET_COLOR << std::endl;
    }
    std::cout << GREEN_COLOR << "shmem_barrier_all() test PASSED!" << RESET_COLOR << std::endl;
    std::cout << GREEN_COLOR << "shmem_my_pe() test PASSED!" << RESET_COLOR << std::endl;
    std::cout << GREEN_COLOR << "shmem_n_pes() test PASSED!" << RESET_COLOR << std::endl;
  }

  /* Run shmem_pe_accessible() test */
  shmem_barrier_all();
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

  /* Run shmem_info_get_version() test */
  shmem_barrier_all();
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

  /* Run shmem_info_get_name() test */
  shmem_barrier_all();
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

  /* Display test information */
  shmem_barrier_all();
  if (mype == 0) { display_test_info(name, version, npes); }

  /************************* START OF THREADS TESTS **************************/
  shmem_barrier_all();
  if (opts.test_threads) {
    if (mype == 0) { display_test_header("THREADS"); }

    /* If we made it here shmem_init_thread() passed */
    if (mype == 0) {
      std::cout << GREEN_COLOR << "shmem_init_thread() test PASSED!" << RESET_COLOR << std::endl;
    }

    /* Test shmem_query_thread() */
    if (!test_shmem_query_thread()) {
      if (mype == 0) {
        std::cerr << RED_COLOR << "shmem_query_thread() test FAILED!" << RESET_COLOR << std::endl;
      }
    }
    else {
      if (mype == 0) {
        std::cout << GREEN_COLOR << "shmem_query_thread() test PASSED!" << RESET_COLOR << std::endl;
      }
    }
  }

  /************************* START OF MEMORY TESTS **************************/
  shmem_barrier_all();
  if (opts.test_mem) {
    if (mype == 0) { display_test_header("MEMORY MANAGEMENT"); }

    /* Test shmem_ptr() */
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

    /* Test shmem_malloc() and shmem_free() */
    shmem_barrier_all();
    if (!test_shmem_malloc_free()) {
      if (mype == 0) {
        std::cerr << RED_COLOR << "shmem_malloc() test FAILED!" << RESET_COLOR << std::endl;
        std::cerr << RED_COLOR << "shmem_free() test FAILED!" << RESET_COLOR << std::endl;
      }
    } 
    else {
      if (mype == 0) {
        std::cout << GREEN_COLOR << "shmem_malloc() test PASSED!" << RESET_COLOR << std::endl;
        std::cout << GREEN_COLOR << "shmem_free() test PASSED!" << RESET_COLOR << std::endl;
      }
    }

    /* Test shmem_realloc() */
    shmem_barrier_all();
    if (!test_shmem_realloc()) {
      if (mype == 0) {
        std::cerr << RED_COLOR << "shmem_realloc() test FAILED!" << RESET_COLOR << std::endl;
      }
    } 
    else {
      if (mype == 0) {
        std::cout << GREEN_COLOR << "shmem_realloc() test PASSED!" << RESET_COLOR << std::endl;
      }
    }

    /* Test shmem_align() */
    shmem_barrier_all();
    if (!test_shmem_align()) {
      if (mype == 0) {
        std::cerr << RED_COLOR << "shmem_align() test FAILED!" << RESET_COLOR << std::endl;
      }
    } 
    else {
      if (mype == 0) {
        std::cout << GREEN_COLOR << "shmem_align() test PASSED!" << RESET_COLOR << std::endl;
      }
    }

    /* Test shmem_malloc_with_hints() */
    shmem_barrier_all();
    if (!test_shmem_malloc_with_hints()) {
      if (mype == 0) {
        std::cerr << RED_COLOR << "shmem_malloc_with_hints() test FAILED!" << RESET_COLOR << std::endl;
      }
    } 
    else {
      if (mype == 0) {
        std::cout << GREEN_COLOR << "shmem_malloc_with_hints() test PASSED!" << RESET_COLOR << std::endl;
      }
    }

    /* Test shmem_calloc() */
    shmem_barrier_all();
    if (!test_shmem_calloc()) {
      if (mype == 0) {
        std::cerr << RED_COLOR << "shmem_calloc() test FAILED!" << RESET_COLOR << std::endl;
      }
    } 
    else {
      if (mype == 0) {
        std::cout << GREEN_COLOR << "shmem_calloc() test PASSED!" << RESET_COLOR << std::endl;
      }
    }
  }

  /************************* START OF TEAMS TESTS **************************/
  shmem_barrier_all();
  if (opts.test_teams) {
    if (mype == 0) { display_test_header("TEAMS MANAGMENT"); }

    /* Run shmem_team_my_pe() test */
    if (!test_shmem_team_my_pe()) {
      if (mype == 0) {
        std::cerr << RED_COLOR << "shmem_team_my_pe() test FAILED!" << RESET_COLOR << std::endl;
      }
    }
    else {
      if (mype == 0) {
        std::cout << GREEN_COLOR << "shmem_team_my_pe() test PASSED!" << RESET_COLOR << std::endl;
      }
    }

    /* Run shmem_team_n_pes() test */
    if (!test_shmem_team_n_pes()) {
      if (mype == 0) {
        std::cerr << RED_COLOR << "shmem_team_n_pes() test FAILED!" << RESET_COLOR << std::endl;
      }
    }
    else {
      if (mype == 0) {
        std::cout << GREEN_COLOR << "shmem_team_n_pes() test PASSED!" << RESET_COLOR << std::endl;
      }
    }

    /* Run shmem_team_config_t() test */
    if (!test_shmem_team_config_t()) {
      if (mype == 0) {
        std::cerr << RED_COLOR << "shmem_team_config_t() test FAILED!" << RESET_COLOR << std::endl;
      }
    }
    else {
      if (mype == 0) {
        std::cout << GREEN_COLOR << "shmem_team_config_t() test PASSED!" << RESET_COLOR << std::endl;
      }
    }

    /* Run shmem_team_get_config() test */
    if (!test_shmem_team_get_config()) {
      if (mype == 0) {
        std::cerr << RED_COLOR << "shmem_team_get_config() test FAILED!" << RESET_COLOR << std::endl;
      }
    }
    else {
      if (mype == 0) {
        std::cout << GREEN_COLOR << "shmem_team_get_config() test PASSED!" << RESET_COLOR << std::endl;
      }
    }

    /* Run shmem_team_translate_pe() test */
    if (!test_shmem_team_translate_pe()) {
      if (mype == 0) {
        std::cerr << RED_COLOR << "shmem_team_translate_pe() test FAILED!" << RESET_COLOR << std::endl;
      }
    }
    else {
      if (mype == 0) {
        std::cout << GREEN_COLOR << "shmem_team_translate_pe() test PASSED!" << RESET_COLOR << std::endl;
      }
    }

    /* Run shmem_team_split_strided() test */
    if (!test_shmem_team_split_strided()) {
      if (mype == 0) {
        std::cerr << RED_COLOR << "shmem_team_split_strided() test FAILED!" << RESET_COLOR << std::endl;
      }
    }
    else {
      if (mype == 0) {
        std::cout << GREEN_COLOR << "shmem_team_split_strided() test PASSED!" << RESET_COLOR << std::endl;
      }
    }

    /* Run shmem_team_split_2d() test */
    if (!test_shmem_team_split_2d()) {
      if (mype == 0) {
        std::cerr << RED_COLOR << "shmem_team_split_2d() test FAILED!" << RESET_COLOR << std::endl;
      }
    }
    else {
      if (mype == 0) {
        std::cout << GREEN_COLOR << "shmem_team_split_2d() test PASSED!" << RESET_COLOR << std::endl;
      }
    }

    /* Run shmem_team_destroy() test */
    if (!test_shmem_team_destroy()) {
      if (mype == 0) {
        std::cerr << RED_COLOR << "shmem_team_destroy() test FAILED!" << RESET_COLOR << std::endl;
      }
    }
    else {
      if (mype == 0) {
        std::cout << GREEN_COLOR << "shmem_team_destroy() test PASSED!" << RESET_COLOR << std::endl;
      }
    }
  }

  /************************* START OF COMMS TESTS **************************/
  shmem_barrier_all();
  if (opts.test_comms) {
    if (mype == 0) { display_test_header("COMMUNICATION / CONTEXT"); }
    /* TODO: Call communication management tests here */
  }

  /************************* START OF REMOTE TESTS **************************/
  shmem_barrier_all();
  if (opts.test_remote) {
    if (mype == 0) { display_test_header("REMOTE MEMORY ACCESS"); }
    /* TODO: Call remote memory access tests here */
  }

  /************************* START OF ATOMICS TESTS **************************/
  shmem_barrier_all();
  if (opts.test_atomics) {
    if (mype == 0) { display_test_header("ATOMIC MEMORY OPS"); }
    /* TODO: Call atomic memory operations tests here */
  }

  /************************* START OF SIGNALING TESTS **************************/
  shmem_barrier_all();
  if (opts.test_signaling) {
    if (mype == 0) { display_test_header("SIGNALING OPS"); }
    /* TODO: Call signaling operations tests here */
  }

  /************************* START OF COLLECTIVES TESTS **************************/
  shmem_barrier_all();
  if (opts.test_collectives) {
    if (mype == 0) { display_test_header("COLLECTIVE OPS"); }
    /* TODO: Call collective operations tests here */
  }

  /************************* START OF PT2PT TESTS **************************/
  shmem_barrier_all();
  if (opts.test_pt2pt_synch) {
    if (mype == 0) { display_test_header("PT2PT SYNCHRONIZATION"); }
    /* TODO: Call point-to-point synchronization tests here */
  }

  /************************* START OF MEM ORDERING TESTS **************************/
  shmem_barrier_all();
  if (opts.test_mem_ordering) {
    if (mype == 0) { display_test_header("MEMORY ORDERING"); }
    /* TODO: Call memory ordering tests here */
  }

  /************************* START OF DIS LOCKING TESTS **************************/
  shmem_barrier_all();
  if (opts.test_locking) {
    if (mype == 0) { display_test_header("DISTRIBUTED LOCKING"); }
    /* TODO: Call distributed locking tests here */
  }

  /************************* START OF FINALIZATION **************************/
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

