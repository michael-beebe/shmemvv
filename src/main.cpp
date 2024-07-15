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
  /************************* SETUP **************************/ 
  /* Load OpenSHMEM routines */
  if (!load_routines()) {
    std::cerr << "Failed to load OpenSHMEM routines" << std::endl;
    return EXIT_FAILURE;
  }

  int mype = 0;
  int npes = 0;
  std::string version = "";
  std::string name = "";
  test_options opts;

  /* Variables to hold test results */
  bool result_shmem_init = true;
  bool result_shmem_init_thread = true;
  bool result_shmem_barrier_all = true;
  bool result_shmem_barrier = true;
  bool result_shmem_my_pe = true;
  bool result_shmem_n_pes = true;
  bool result_shmem_pe_accessible = true;
  bool result_shmem_info_get_version = true;
  bool result_shmem_info_get_name = true;

  /* Initialize with shmem_init_thread() if THREADS tests were enabled */
  if (opts.test_threads) {
    if (!check_if_exists("shmem_init_thread", 0)) {
      if (mype == 0) {
        display_not_found_warning("shmem_init_thread()", true);
      }
      return EXIT_FAILURE;
    }
    else {
      result_shmem_init_thread = test_shmem_init_thread();
      if (!result_shmem_init_thread) {
        display_test_result("shmem_init_thread()", result_shmem_init_thread, true);
        return EXIT_FAILURE;
      }
    }
  }
  else {
    /* Initialize with regular shmem_init() if THREADS tests are not enabled */
    if (!check_if_exists("shmem_init", 0)) {
      if (mype == 0) {
        display_not_found_warning("shmem_init()", true);
      }
      return EXIT_FAILURE;
    }
    else {
      result_shmem_init = test_shmem_init();
      if (!result_shmem_init) {
        display_test_result("shmem_init()", result_shmem_init, true);
        return EXIT_FAILURE;
      }
    }
  }

  /* Run shmem_barrier_all() test */
  if (!check_if_exists("shmem_barrier_all", 0)) {
    if (mype == 0) {
      display_not_found_warning("shmem_barrier_all()", true);
    }
    shmem_finalize();
    return EXIT_FAILURE;
  }
  else {
    result_shmem_barrier_all = test_shmem_barrier_all();
    if (!result_shmem_barrier_all) {
      if (shmem_my_pe() == 0) {
        display_test_result("shmem_barrier_all()", result_shmem_barrier_all, true);
      }
      shmem_finalize();
      return EXIT_FAILURE;
    }
  }

  /* Run shmem_my_pe() test */
  shmem_barrier_all();
  if (!check_if_exists("shmem_my_pe", 0)) {
    if (mype == 0) {
      display_not_found_warning("shmem_my_pe()", true);
    }
    shmem_finalize();
    return EXIT_FAILURE;
  }
  else {
    mype = test_shmem_my_pe();
    result_shmem_my_pe = mype >= 0;
    if (!result_shmem_my_pe) {
      if (mype == 0) {
        display_test_result("shmem_my_pe()", result_shmem_my_pe, true);
      }
      shmem_finalize();
      return EXIT_FAILURE;
    }
  }

  /* Run shmem_n_pes() test */
  shmem_barrier_all();
  if (!check_if_exists("shmem_n_pes", mype)) {
    if (mype == 0) {
      display_not_found_warning("shmem_n_pes", true);
    }
    shmem_finalize();
    return EXIT_FAILURE;
  }
  else {
    /* Set npes */
    npes = test_shmem_n_pes();
    result_shmem_n_pes = npes > 0;
    if (!result_shmem_n_pes) {
      if (mype == 0) {
        display_test_result("shmem_n_pes()", result_shmem_n_pes, true);
      }
      shmem_finalize();
      return EXIT_FAILURE;
    }
  }

  /* Run shmem_pe_accessible() test */
  shmem_barrier_all();
  if (!check_if_exists("shmem_pe_accessible", mype)) {
    if (mype == 0) {
      display_not_found_warning("shmem_pe_accessible()", false);
    }
  }
  else {
    result_shmem_pe_accessible = test_shmem_pe_accessible();
    if (!result_shmem_pe_accessible) {
      if (mype == 0) {
        display_test_result("shmem_pe_accessible()", result_shmem_pe_accessible, true);
      }
      shmem_finalize();
      return EXIT_FAILURE;
    }
  }

  /*
    Run test to make sure OpenSHMEM routines that aren't implemented
    don't throw compiler errors
  */
  #ifdef _DEBUG_
    shmem_barrier_all();
    if (!check_if_exists("shmem_fake_routine", mype)) {
      if (mype == 0) {
        display_not_found_warning("shmem_fake_routine()", false);
      }
    }
    else {
      test_shmem_fake_routine();
    }
  #endif

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

  /* Run shmem_barrier() test */
  shmem_barrier_all();
  if (!check_if_exists("shmem_barrier", mype)) {
    if (mype == 0) {
      display_not_found_warning("shmem_barrier()", false);
    }
  }
  else {
    result_shmem_barrier = test_shmem_barrier();
    shmem_barrier_all();
  }

  /* Run shmem_info_get_version() test */
  shmem_barrier_all();
  if (!check_if_exists("shmem_info_get_version", mype)) {
    result_shmem_info_get_version = false;
    if (mype == 0) {
      display_not_found_warning("shmem_info_get_version()", false);
    }
  }
  else {
    version = test_shmem_info_get_version();
    if (version == "") {
      result_shmem_info_get_version = false;
    }
  }

  /* Run shmem_info_get_name() test */
  shmem_barrier_all();
  if (!check_if_exists("shmem_info_get_name", mype)) {
    result_shmem_info_get_name = false;
    if (mype == 0) {
      display_not_found_warning("shmem_info_get_name()", false);
    }
  }
  else {
    name = test_shmem_info_get_name();
    if (name == "") {
      result_shmem_info_get_name = false;
    }
  }

  /* Parse command-line options */
  if (!parse_opts(argc, argv, opts)) {
    if (mype == 0) {
      display_help();
    }
    shmem_finalize();
    return EXIT_FAILURE;
  }

  shmem_barrier_all();

  /* Enable all tests if --all is specified or no specific test is selected */
  if (opts.all ||
      !(opts.test_setup || opts.test_threads || opts.test_mem || opts.test_teams ||
        opts.test_ctx || opts.test_remote || opts.test_atomics || opts.test_signaling ||
        opts.test_collectives || opts.test_pt2pt_synch || opts.test_mem_ordering || opts.test_locking))
  {
    opts.test_setup = true; opts.test_threads = true; opts.test_mem = true; opts.test_teams = true;
    opts.test_ctx = true; opts.test_remote = true; opts.test_atomics = true; opts.test_signaling = true;
    opts.test_collectives = true; opts.test_pt2pt_synch = true; opts.test_mem_ordering = true; opts.test_locking = true;
  }
  
  /* Display test information */
  shmem_barrier_all();
  if (mype == 0) {
    display_test_info(name, version, npes);
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
      display_test_result("shmem_init()", result_shmem_init, true);
    }
    display_test_result("shmem_barrier_all()", result_shmem_barrier_all, true);
    display_test_result("shmem_barrier()", result_shmem_barrier, false);
    display_test_result("shmem_my_pe()", result_shmem_my_pe, true);
    display_test_result("shmem_n_pes()", result_shmem_n_pes, true);
    display_test_result("shmem_pe_accessible()", result_shmem_pe_accessible, true);
    if (version != "1.5" && version != "1.50") {
      std::cerr << YELLOW_COLOR << "shmem_info_get_version() test did not return 1.5... Returned " << version << std::endl;
    }
    else {
      display_test_result("shmem_info_get_version()", result_shmem_info_get_version, false);
    }
    display_test_result("shmem_info_get_name()", result_shmem_info_get_name,false);
  }

  /************************* THREADS TESTS **************************/
  if (opts.test_threads) {
    shmem_barrier_all();
    if (mype == 0) { display_test_header("THREADS"); }
    shmem_barrier_all();
 
    /* If we made it here shmem_init_thread() passed */
    if (mype == 0) {
      display_test_result("shmem_init_thread()", result_shmem_init_thread, true);
    }

    /* Test shmem_query_thread() */
    shmem_barrier_all();
    if ( !check_if_exists("shmem_query_thread", mype) ) {
      if (mype == 0) {
        display_not_found_warning("shmem_query_thread()", false);
      }
    }
    else {
      bool result_shmem_query_thread = test_shmem_query_thread();
      shmem_barrier_all();
      if (mype == 0) {
        display_test_result("shmem_query_thread()", result_shmem_query_thread, false);
      }
    }
  }

  /************************* MEMORY TESTS **************************/
  if (opts.test_mem) {
    shmem_barrier_all();
    if (mype == 0) { display_test_header("MEMORY MANAGEMENT"); }

    /* Test shmem_ptr() */
    shmem_barrier_all();
    if ( !check_if_exists("shmem_ptr", mype) ) {
      if (mype == 0) {
        display_not_found_warning("shmem_ptr()", false);
      }
    }
    else {
      bool result_shmem_ptr = test_shmem_ptr();
      shmem_barrier_all();
      if (mype == 0) {
        display_test_result("shmem_ptr()", result_shmem_ptr, false);
      }
    }

    /* Test shmem_malloc() and shmem_free() */
    if ( check_if_exists("shmem_malloc", mype) && check_if_exists("shmem_free", mype) ) {
      bool result_shmem_malloc_free = test_shmem_malloc_free();
      shmem_barrier_all();
      if (mype == 0) {
        display_test_result("shmem_malloc()", result_shmem_malloc_free, false);
        display_test_result("shmem_free()", result_shmem_malloc_free, false);
      }
    }
    else {
      if ( !check_if_exists("shmem_malloc", mype) ) {
        if (mype == 0) {
          display_not_found_warning("shmem_malloc()", false);
        }
      }
      if ( !check_if_exists("shmem_free", mype) ) {
        if (mype == 0) {
          display_not_found_warning("shmem_free()", false);
        }
      }
    }

    /* Test shmem_addr_accessible */
    shmem_barrier_all();
    if ( !check_if_exists("shmem_addr_accessible", mype) ) {
      if (mype == 0) {
        display_not_found_warning("shmem_addr_accessible", false);
      }
    }
    else {
      bool result_shmem_addr_accessible = test_shmem_addr_accessible();
      shmem_barrier_all();
      if (mype == 0) {
        display_test_result("shmem_addr_accessible()", result_shmem_addr_accessible, false);
      }
    }

    /* Test shmem_realloc() */
    shmem_barrier_all();
    if ( !check_if_exists("shmem_realloc", mype) ) {
      if (mype == 0) {
        display_not_found_warning("shme_realloc()", false);
      }
    }
    else {
      bool result_shmem_realloc = test_shmem_realloc();
      shmem_barrier_all();
      if (mype == 0) {
        display_test_result("shmem_realloc()", result_shmem_realloc, false); 
      }
    }

    /* Test shmem_align() */
    shmem_barrier_all();
    if ( !check_if_exists("shmem_align", mype) ) {
      if (mype == 0) {
        display_not_found_warning("shmem_align()", false);
      }
    }
    else {
      bool result_shmem_align = test_shmem_align();
      shmem_barrier_all();
      if (mype == 0) {
        display_test_result("shmem_align()", result_shmem_align, false);
      }
    }

    /* Test shmem_malloc_with_hints() */ 
    shmem_barrier_all();
    if ( !check_if_exists("shmem_malloc_with_hints", mype) ) {
      if (mype == 0) {
        display_not_found_warning("shmem_malloc_with_hints()", false);
      }
    }
    else {
      bool result_shmem_malloc_with_hints = test_shmem_malloc_with_hints();
      shmem_barrier_all();
      if (mype == 0) {
        display_test_result("shmem_malloc_with_hints()", result_shmem_malloc_with_hints, false);
      }
    }

    /* Test shmem_calloc() */
    shmem_barrier_all();
    if ( !check_if_exists("shmem_calloc", mype) ) {
      if (mype == 0) {
        display_not_found_warning("shmem_calloc()", false);
      }
    }
    else {
      bool result_shmem_calloc = test_shmem_calloc();
      shmem_barrier_all();
      if (mype == 0) {
        display_test_result("shmem_calloc()", result_shmem_calloc, false);
      }
    }
  }

  /************************* TEAMS TESTS **************************/
  if (opts.test_teams) {
    shmem_barrier_all();
    if (mype == 0) { display_test_header("TEAMS MANAGEMENT"); }

    /* Run shmem_team_my_pe() test */
    shmem_barrier_all();
    if ( !check_if_exists("shmem_team_my_pe", mype) ) {
      if (mype == 0) {
        display_not_found_warning("shmem_team_my_pe()", true);
      }
    }
    else {
      bool result_shmem_team_my_pe = test_shmem_team_my_pe();
      shmem_barrier_all();
      if (mype == 0) {
        display_test_result("shmem_team_my_pe()", result_shmem_team_my_pe, false);
      }
    }

    /* Run shmem_team_n_pes() test */
    shmem_barrier_all();
    if ( !check_if_exists("shmem_team_n_pes", mype) ) {
      if (mype == 0) {
        display_not_found_warning("shmem_team_n_pes()", true);
      }
    }
    else {
      bool result_shmem_team_n_pes = test_shmem_team_n_pes();
      shmem_barrier_all();
      if (mype == 0) {
        display_test_result("shmem_team_n_pes()", result_shmem_team_n_pes, false);
      }
    }

    /* Run shmem_team_get_config() test */
    shmem_barrier_all();
    if ( !check_if_exists("shmem_team_get_config", mype) ) {
      if (mype == 0) {
        display_not_found_warning("shmem_team_get_config()", false);
      }
    }
    else {
      bool result_shmem_team_get_config = test_shmem_team_get_config();
      shmem_barrier_all();
      if (mype == 0) {
        display_test_result("shmem_team_get_config()", result_shmem_team_get_config, false);
      }
    }

    /* Run shmem_team_translate_pe() test */
    shmem_barrier_all();
    if ( !check_if_exists("shmem_team_translate_pe", mype) ) {
      if (mype == 0) {
        display_not_found_warning("shmem_team_translate_pe()", false);
      }
    }
    else {
      bool result_shmem_team_translate_pe = test_shmem_team_translate_pe();
      shmem_barrier_all();
      if (mype == 0) {
        display_test_result("shmem_team_translate_pe()", result_shmem_team_translate_pe, false);
      }
    }

    /* Run shmem_team_split_strided() test */
    shmem_barrier_all();
    if ( !check_if_exists("shmem_team_split_strided", mype) ) {
      if (mype == 0) {
        display_not_found_warning("shmem_team_split_strided()", false);
      }
    }
    else {
      bool result_shmem_team_split_strided = test_shmem_team_split_strided();
      shmem_barrier_all();
      if (mype == 0) {
        display_test_result("shmem_team_split_strided()", result_shmem_team_split_strided, false);
      }
    }

    /* Run shmem_team_split_2d() test */
    shmem_barrier_all();
    if ( !check_if_exists("shmem_team_split_2d", mype) ) {
      if (mype == 0) {
        display_not_found_warning("shmem_team_split_2d()", false);
      }
    }
    else {
      bool result_shmem_team_split_2d = test_shmem_team_split_2d();
      shmem_barrier_all();
      if (mype == 0) {
        display_test_result("shmem_team_split_2d()", result_shmem_team_split_2d, false);
      }
    }

    /* Run shmem_team_destroy() test */
    shmem_barrier_all();
    if ( !check_if_exists("shmem_team_destroy", mype) ) {
      if (mype == 0) {
        display_not_found_warning("shmem_team_destroy()", false);
      }
    }
    else {
      bool result_shmem_team_destroy = test_shmem_team_destroy();
      shmem_barrier_all();
      if (mype == 0) {
        display_test_result("shmem_team_destroy()", result_shmem_team_destroy, false);
      }
    }
  }

  /************************* CTX TESTS **************************/
  if (opts.test_ctx) {
    shmem_barrier_all();
    if (mype == 0) { display_test_header("COMMUNICATION / CONTEXT"); }

    /* Run shmem_ctx_create() test */
    shmem_barrier_all();
    if (!check_if_exists("shmem_ctx_create", mype)) {
      if (mype == 0) {
        display_not_found_warning("shmem_ctx_create()", false);
      }
    }
    else {
      bool result_shmem_ctx_create = test_shmem_ctx_create();
      shmem_barrier_all();
      if (mype == 0) {
        display_test_result("shmem_ctx_create()", result_shmem_ctx_create, false);
      }
    }

    /* Run shmem_team_create_ctx() test */
    shmem_barrier_all();
    if (!check_if_exists("shmem_team_create_ctx", mype)) {
      if (mype == 0) {
        display_not_found_warning("shmem_team_create_ctx()", false);
      }
    }
    else {
      bool result_shmem_team_create_ctx = test_shmem_team_create_ctx();
      shmem_barrier_all();
      if (mype == 0) {
        display_test_result("shmem_team_create_ctx()", result_shmem_team_create_ctx, false);
      }
    }

    /* Run shmem_ctx_destroy() test */
    shmem_barrier_all();
    if (!check_if_exists("shmem_ctx_destroy", mype)) {
      if (mype == 0) {
        display_not_found_warning("shmem_ctx_destroy()", false);
      }
    }
    else {
      bool result_shmem_ctx_destroy = test_shmem_ctx_destroy();
      shmem_barrier_all();
      if (mype == 0) {
        display_test_result("shmem_ctx_destroy()", result_shmem_ctx_destroy, false);
      }
    }

    /* Run shmem_ctx_get_team() test */
    shmem_barrier_all();
    if (!check_if_exists("shmem_ctx_get_team", mype)) {
      if (mype == 0) {
        display_not_found_warning("shmem_ctx_get_team()", false);
      }
    }
    else {
      bool result_shmem_ctx_get_team = test_shmem_ctx_get_team();
      shmem_barrier_all();
      if (mype == 0) {
        display_test_result("shmem_ctx_get_team()", result_shmem_ctx_get_team, false);
      }
    }
  }

  /************************* REMOTE TESTS **************************/
  if (opts.test_remote) {
    shmem_barrier_all();
    if (mype == 0) { display_test_header("REMOTE MEMORY ACCESS"); }

    /* Check to make sure there are at least 2 PEs */
    if ( !(npes > 1) ) {
      std::cerr << RED_COLOR << "ERROR: " << RESET_COLOR << "REMOTE MEMORY ACCESS tests require at least 2 PEs!" << std::endl;
    }
    else {
      /* Run shmem_put() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_put", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_long_put()", false);
        }
      }
      else {
        bool result_shmem_put = test_shmem_put();
        shmem_barrier_all();
        if (mype == 0) {
          display_test_result("shmem_put()", result_shmem_put, false);
        }
      }

      /* Run shmem_p() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_p", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_long_p()", false);
        }
      }
      else {
        bool result_shmem_p = test_shmem_p();
        shmem_barrier_all();
        if (mype == 0) {
          display_test_result("shmem_p()", result_shmem_p, false);
        }
      }

      /* Run shmem_iput() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_iput", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_long_iput()", false);
        }
      }
      else {
        bool result_shmem_iput = test_shmem_iput();
        shmem_barrier_all();
        if (mype == 0) {
          display_test_result("shmem_iput()", result_shmem_iput, false);
        }
      }

      /* Run shmem_get() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_get", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_long_get()", false);
        }
      }
      else {
        bool result_shmem_get = test_shmem_get();
        shmem_barrier_all();
        if (mype == 0) {
          display_test_result("shmem_get()", result_shmem_get, false);
        }
      }

      /* Run shmem_g() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_g", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_long_g()", false);
        }
      }
      else {
        bool result_shmem_g = test_shmem_g();
        shmem_barrier_all();
        if (mype == 0) {
          display_test_result("shmem_g()", result_shmem_g, false);
        }
      }

      /* Run shmem_iget() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_iget", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_long_iget()", false);
        }
      }
      else {
        bool result_shmem_iget = test_shmem_iget();
        shmem_barrier_all();
        if (mype == 0) {
          display_test_result("shmem_iget()", result_shmem_iget, false);
        }
      }

      /* Run shmem_put_nbi() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_put_nbi", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_long_put_nbi()", false);
        }
      }
      else {
        bool result_shmem_put_nbi = test_shmem_put_nbi();
        shmem_barrier_all();
        if (mype == 0) {
          display_test_result("shmem_put_nbi()", result_shmem_put_nbi, false);
        }
      }

      /* Run shmem_get_nbi() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_get_nbi", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_long_get_nbi()", false);
        }
      }
      else {
        bool result_shmem_get_nbi = test_shmem_get_nbi();
        shmem_barrier_all();
        if (mype == 0) {
          display_test_result("shmem_get_nbi()", result_shmem_get_nbi, false);
        }
      }
    }
  }
  /************************* ATOMICS TESTS **************************/
  if (opts.test_atomics) {
    shmem_barrier_all();
    if (mype == 0) { display_test_header("ATOMIC MEMORY OPS"); }

    /* Make sure there are at least 2 PEs */
    if (!(npes > 1)) {
      std::cerr << RED_COLOR << "ERROR: " << RESET_COLOR 
      << "ATOMIC MEMORY OPS tests require at least 2 PEs!" << std::endl;
    }
    else {
      /* Run shmem_atomic_fetch() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_atomic_fetch", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_atomic_fetch()", false);
        }
      }
      else {
        bool result_shmem_atomic_fetch = test_shmem_atomic_fetch();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_atomic_fetch()", result_shmem_atomic_fetch, false); }
      }

      /* Run shmem_atomic_set() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_atomic_set", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_atomic_set()", false);
        }
      }
      else {
        bool result_shmem_atomic_set = test_shmem_atomic_set();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_atomic_set()", result_shmem_atomic_set, false); }
      }

      /* Run shmem_atomic_compare_swap() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_atomic_compare_swap", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_atomic_compare_swap()", false);
        }
      }
      else {
        bool result_shmem_atomic_compare_swap = test_shmem_atomic_compare_swap();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_atomic_compare_swap()", result_shmem_atomic_compare_swap, false); }
      }

      /* Run shmem_atomic_swap() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_atomic_swap", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_atomic_swap()", false);
        }
      }
      else {
        bool result_shmem_atomic_swap = test_shmem_atomic_swap();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_atomic_swap()", result_shmem_atomic_swap, false); }
      }

      /* Run shmem_atomic_fetch_inc() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_atomic_fetch_inc", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_atomic_fetch_inc()", false);
        }
      }
      else {
        bool result_shmem_atomic_fetch_inc = test_shmem_atomic_fetch_inc();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_atomic_fetch_inc()", result_shmem_atomic_fetch_inc, false); }
      }

      /* Run shmem_atomic_inc() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_atomic_inc", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_atomic_inc()", false);
        }
      }
      else {
        bool result_shmem_atomic_inc = test_shmem_atomic_inc();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_atomic_inc()", result_shmem_atomic_inc, false); }
      }

      /* Run shmem_atomic_fetch_add() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_atomic_fetch_add", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_atomic_fetch_add()", false);
        }
      }
      else {
        bool result_shmem_atomic_fetch_add = test_shmem_atomic_fetch_add();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_atomic_fetch_add()", result_shmem_atomic_fetch_add, false); }
      }

      /* Run shmem_atomic_add() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_atomic_add", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_atomic_add()", false);
        }
      }
      else {
        bool result_shmem_atomic_add = test_shmem_atomic_add();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_atomic_add()", result_shmem_atomic_add, false); }
      }

      /* Run shmem_atomic_fetch_and() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_atomic_fetch_and", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_atomic_fetch_and()", false);
        }
      }
      else {
        bool result_shmem_atomic_fetch_and = test_shmem_atomic_fetch_and();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_atomic_fetch_and()", result_shmem_atomic_fetch_and, false); }
      }

      /* Run shmem_atomic_and() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_atomic_and", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_atomic_and()", false);
        }
      }
      else {
        bool result_shmem_atomic_and = test_shmem_atomic_and();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_atomic_and()", result_shmem_atomic_and, false); }
      }

      /* Run shmem_atomic_fetch_or() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_atomic_fetch_or", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_atomic_fetch_or()", false);
        }
      }
      else {
        bool result_shmem_atomic_fetch_or = test_shmem_atomic_fetch_or();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_atomic_fetch_or()", result_shmem_atomic_fetch_or, false); }
      }

      /* Run shmem_atomic_or() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_atomic_or", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_atomic_or()", false);
        }
      }
      else {
        bool result_shmem_atomic_or = test_shmem_atomic_or();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_atomic_or()", result_shmem_atomic_or, false); }
      }

      /* Run shmem_atomic_fetch_xor() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_atomic_fetch_xor", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_atomic_fetch_xor()", false);
        }
      }
      else {
        bool result_shmem_atomic_fetch_xor = test_shmem_atomic_fetch_xor();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_atomic_fetch_xor()", result_shmem_atomic_fetch_xor, false); }
      }

      /* Run shmem_atomic_xor() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_atomic_xor", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_atomic_xor()", false);
        }
      }
      else {
        bool result_shmem_atomic_xor = test_shmem_atomic_xor();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_atomic_xor()", result_shmem_atomic_xor, false); }
      }

      /* Run shmem_atomic_fetch_nbi() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_atomic_fetch_nbi", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_atomic_fetch_nbi()", false);
        }
      }
      else {
        bool result_shmem_atomic_fetch_nbi = test_shmem_atomic_fetch_nbi();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_atomic_fetch_nbi()", result_shmem_atomic_fetch_nbi, false); }
      }

      /* Run shmem_atomic_compare_swap_nbi() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_atomic_compare_swap_nbi", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_atomic_compare_swap_nbi()", false);
        }
      }
      else {
        bool result_shmem_atomic_compare_swap_nbi = test_shmem_atomic_compare_swap_nbi();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_atomic_compare_swap_nbi()", result_shmem_atomic_compare_swap_nbi, false); }
      }

      /* Run shmem_atomic_swap_nbi() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_atomic_swap_nbi", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_atomic_swap_nbi()", false);
        }
      }
      else {
        bool result_shmem_atomic_swap_nbi = test_shmem_atomic_swap_nbi();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_atomic_swap_nbi()", result_shmem_atomic_swap_nbi, false); }
      }

      /* Run shmem_atomic_fetch_inc_nbi() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_atomic_fetch_inc_nbi", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_atomic_fetch_inc_nbi()", false);
        }
      }
      else {
        bool result_shmem_atomic_fetch_inc_nbi = test_shmem_atomic_fetch_inc_nbi();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_atomic_fetch_inc_nbi()", result_shmem_atomic_fetch_inc_nbi, false); }
      }

      /* Run shmem_atomic_fetch_add_nbi() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_atomic_fetch_add_nbi", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_atomic_fetch_add_nbi()", false);
        }
      }
      else {
        bool result_shmem_atomic_fetch_add_nbi = test_shmem_atomic_fetch_add_nbi();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_atomic_fetch_add_nbi()", result_shmem_atomic_fetch_add_nbi, false); }
      }

      /* Run shmem_atomic_fetch_and_nbi() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_atomic_fetch_and_nbi", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_atomic_fetch_and_nbi()", false);
        }
      }
      else {
        bool result_shmem_atomic_fetch_and_nbi = test_shmem_atomic_fetch_and_nbi();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_atomic_fetch_and_nbi()", result_shmem_atomic_fetch_and_nbi, false); }
      }

      /* Run shmem_atomic_fetch_or_nbi() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_atomic_fetch_or_nbi", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_atomic_fetch_or_nbi()", false);
        }
      }
      else {
        bool result_shmem_atomic_fetch_or_nbi = test_shmem_atomic_fetch_or_nbi();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_atomic_fetch_or_nbi()", result_shmem_atomic_fetch_or_nbi, false); }
      }

      /* Run shmem_atomic_fetch_xor_nbi() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_atomic_fetch_xor_nbi", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_atomic_fetch_xor_nbi()", false);
        }
      }
      else {
        bool result_shmem_atomic_fetch_xor_nbi = test_shmem_atomic_fetch_xor_nbi();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_atomic_fetch_xor_nbi()", result_shmem_atomic_fetch_xor_nbi, false); }
      }
    }
  }
  
  /************************* SIGNALING TESTS **************************/
  if (opts.test_signaling) {
    shmem_barrier_all();
    if (mype == 0) { display_test_header("SIGNALING OPS"); }

    if (!(npes > 1)) {
      std::cerr << RED_COLOR << "ERROR: " << RESET_COLOR << "SIGNALING OPS tests require at least 2 PEs!" << std::endl;
    }
    else {
      /* Run shmem_put_signal() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_put_signal", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_long_put_signal()", false);
        }
      }
      else {
        bool result_shmem_put_signal = test_shmem_put_signal();
        shmem_barrier_all();
        if (mype == 0) {
          display_test_result("shmem_put_signal()", result_shmem_put_signal, false);
        }
      }

      /* Run shmem_put_signal_nbi() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_put_signal_nbi", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_long_put_signal_nbi()", false);
        }
      }
      else {
        bool result_shmem_put_signal_nbi = test_shmem_put_signal_nbi();
        shmem_barrier_all();
        if (mype == 0) {
          display_test_result("shmem_put_signal_nbi()", result_shmem_put_signal_nbi, false);
        }
      }

      /* Run shmem_signal_fetch() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_signal_fetch", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_signal_fetch()", false);
        }
      }
      else {
        bool result_shmem_signal_fetch = test_shmem_signal_fetch();
        shmem_barrier_all();
        if (mype == 0) {
          display_test_result("shmem_signal_fetch()", result_shmem_signal_fetch, false);
        }
      }
    }
  }

  /************************* COLLECTIVES TESTS **************************/
  if (opts.test_collectives) {
    /* Print project header */
    shmem_barrier_all();
    if (mype == 0) { display_test_header("COLLECTIVE OPS"); }

    /* Check to make sure there are at least 2 PEs */
    if ( !(npes > 1) ) {
      std::cerr << RED_COLOR << "ERROR: " << RESET_COLOR << "COLLECTIVE tests require at least 2 PEs!" << std::endl;
    }
    else {
      /* Run shmem_sync() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_sync", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_sync()", false);
        }
      } 
      else {
        bool result_shmem_sync = test_shmem_sync();
        shmem_barrier_all();
        if (mype == 0) {
          display_test_result("shmem_sync()", result_shmem_sync, false);
        }
      }

      /* Run shmem_sync_all() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_sync_all", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_sync_all()", false);
        }
      } 
      else {
        bool result_shmem_sync_all = test_shmem_sync_all();
        shmem_barrier_all();
        if (mype == 0) {
          display_test_result("shmem_sync_all()", result_shmem_sync_all, false);
        }
      }

      /* Run shmem_alltoall() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_alltoall", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_long_alltoall()", false);
        }
      } 
      else {
        bool result_shmem_alltoall = test_shmem_alltoall();
        shmem_barrier_all();
        if (mype == 0) {
          display_test_result("shmem_alltoall()", result_shmem_alltoall, false);
        }
      }

      /* Run shmem_alltoalls() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_alltoalls", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_long_alltoalls()", false);
        }
      } 
      else {
        bool result_shmem_alltoalls = test_shmem_alltoalls();
        shmem_barrier_all();
        if (mype == 0) {
          display_test_result("shmem_alltoalls()", result_shmem_alltoalls, false);
        }
      }

      /* Run shmem_broadcast() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_broadcast", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_long_broadcast()", false);
        }
      } 
      else {
        bool result_shmem_broadcast = test_shmem_broadcast();
        shmem_barrier_all();
        if (mype == 0) {
          display_test_result("shmem_broadcast()", result_shmem_broadcast, false);
        }
      }

      /* Run shmem_collect() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_collect", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_long_collect()", false);
        }
      } 
      else {
        bool result_shmem_collect = test_shmem_collect();
        shmem_barrier_all();
        if (mype == 0) {
          display_test_result("shmem_collect()", result_shmem_collect, false);
        }
      }

      /* Run shmem_fcollect() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_fcollect", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_long_fcollect()", false);
        }
      } 
      else {
        bool result_shmem_fcollect = test_shmem_fcollect();
        shmem_barrier_all();
        if (mype == 0) {
          display_test_result("shmem_fcollect()", result_shmem_fcollect, false);
        }
      }

      /* Run shmem_max_reduce() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_max_reduce", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_long_max_reduce()", false);
        }
      } 
      else {
        bool result_shmem_max_reduce = test_shmem_max_reduce();
        shmem_barrier_all();
        if (mype == 0) {
          display_test_result("shmem_max_reduce()", result_shmem_max_reduce, false);
        }
      }

      /* Run shmem_min_reduce() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_min_reduce", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_long_min_reduce()", false);
        }
      } 
      else {
        bool result_shmem_min_reduce = test_shmem_min_reduce();
        shmem_barrier_all();
        if (mype == 0) {
          display_test_result("shmem_min_reduce()", result_shmem_min_reduce, false);
        }
      }

      /* Run shmem_sum_reduce() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_sum_reduce", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_long_sum_reduce()", false);
        }
      } 
      else {
        bool result_shmem_sum_reduce = test_shmem_sum_reduce();
        shmem_barrier_all();
        if (mype == 0) {
          display_test_result("shmem_sum_reduce()", result_shmem_sum_reduce, false);
        }
      }

      /* Run shmem_prod_reduce() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_prod_reduce", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_long_prod_reduce()", false);
        }
      } 
      else {
        bool result_shmem_prod_reduce = test_shmem_prod_reduce();
        shmem_barrier_all();
        if (mype == 0) {
          display_test_result("shmem_prod_reduce()", result_shmem_prod_reduce, false);
        }
      }
    }
  }

  /************************* PT2PT TESTS **************************/
  if (opts.test_pt2pt_synch) {
    shmem_barrier_all();
    if (mype == 0) { display_test_header("POINT-TO-POINT SYNC OPS"); }

    if (!(npes > 1)) {
      std::cerr << RED_COLOR << "ERROR: " << RESET_COLOR << "POINT-TO-POINT SYNC OPS tests require at least 2 PEs!" << std::endl;
    }
    else {
      /* Run shmem_wait_until() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_wait_until", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_long_wait_until()", false);
        }
      }
      else {
        bool result_shmem_wait_until = test_shmem_wait_until();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_wait_until()", result_shmem_wait_until, false); }
      }

      /* Run shmem_wait_until_all() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_wait_until_all", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_long_wait_until_all()", false);
        }
      }
      else {
        bool result_shmem_wait_until_all = test_shmem_wait_until_all();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_wait_until_all()", result_shmem_wait_until_all, false); }
      }

      /* Run shmem_wait_until_any() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_wait_until_any", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_long_wait_until_any()", false);
        }
      }
      else {
        bool result_shmem_wait_until_any = test_shmem_wait_until_any();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_wait_until_any()", result_shmem_wait_until_any, false); }
      }

      /* Run shmem_wait_until_some() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_wait_until_some", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_long_wait_until_some()", false);
        }
      }
      else {
        bool result_shmem_wait_until_some = test_shmem_wait_until_some();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_wait_until_some()", result_shmem_wait_until_some, false); }
      }

      /* Run shmem_wait_until_all_vector() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_wait_until_all_vector", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_long_wait_until_all_vector()", false);
        }
      }
      else {
        bool result_shmem_wait_until_all_vector = test_shmem_wait_until_all_vector();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_wait_until_all_vector()", result_shmem_wait_until_all_vector, false); }
      }

      /* Run shmem_wait_until_any_vector() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_wait_until_any_vector", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_long_wait_until_any_vector()", false);
        }
      }
      else {
        bool result_shmem_wait_until_any_vector = test_shmem_wait_until_any_vector();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_wait_until_any_vector()", result_shmem_wait_until_any_vector, false); }
      }

      /* Run shmem_wait_until_some_vector() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_wait_until_some_vector", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_long_wait_until_some_vector()", false);
        }
      }
      else {
        bool result_shmem_wait_until_some_vector = test_shmem_wait_until_some_vector();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_wait_until_some_vector()", result_shmem_wait_until_some_vector, false); }
      }

      /* Run shmem_test() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_test", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_long_test()", false);
        }
      }
      else {
        bool result_shmem_test = test_shmem_test();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_test()", result_shmem_test, false); }
      }

      /* Run shmem_test_all() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_test_all", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_long_test_all()", false);
        }
      }
      else {
        bool result_shmem_test_all = test_shmem_test_all();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_test_all()", result_shmem_test_all, false); }
      }

      /* Run shmem_test_any() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_test_any", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_long_test_any()", false);
        }
      }
      else {
        bool result_shmem_test_any = test_shmem_test_any();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_test_any()", result_shmem_test_any, false); }
      }

      /* Run shmem_test_some() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_test_some", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_long_test_some()", false);
        }
      }
      else {
        bool result_shmem_test_some = test_shmem_test_some();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_test_some()", result_shmem_test_some, false); }
      }

      /* Run shmem_test_all_vector() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_test_all_vector", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_long_test_all_vector()", false);
        }
      }
      else {
        bool result_shmem_test_all_vector = test_shmem_test_all_vector();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_test_all_vector()", result_shmem_test_all_vector, false); }
      }

      /* Run shmem_test_any_vector() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_test_any_vector", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_long_test_any_vector()", false);
        }
      }
      else {
        bool result_shmem_test_any_vector = test_shmem_test_any_vector();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_test_any_vector()", result_shmem_test_any_vector, false); }
      }

      /* Run shmem_test_some_vector() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_long_test_some_vector", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_long_test_some_vector()", false);
        }
      }
      else {
        bool result_shmem_test_some_vector = test_shmem_test_some_vector();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_test_some_vector()", result_shmem_test_some_vector, false); }
      }

      /* Run shmem_signal_wait_until() test */
      shmem_barrier_all();
      if (!check_if_exists("shmem_signal_wait_until", mype)) {
        if (mype == 0) {
          display_not_found_warning("shmem_signal_wait_until()", false);
        }
      }
      else {
        bool result_shmem_signal_wait_until = test_shmem_signal_wait_until();
        shmem_barrier_all();
        if (mype == 0) { display_test_result("shmem_signal_wait_until()", result_shmem_signal_wait_until, false); }
      }
    }
  }

  /************************* MEM ORDERING TESTS **************************/
  if (opts.test_mem_ordering) {
    shmem_barrier_all();
    if (mype == 0) { display_test_header("MEMORY ORDERING"); }

    /* Make sure there are at least 2 PEs */
    if ( !(npes > 1) ) {
      if (mype == 0) {
        std::cerr << RED_COLOR << "ERROR: " << RESET_COLOR << "MEMORY ORDERING tests require at least 2 PEs!" << std::endl;
      }
    }
    else {
      /* Run the shmem_fence() test */
      shmem_barrier_all();
      if ( !check_if_exists("shmem_fence", mype) ) {
        if (mype == 0) {
          display_not_found_warning("shmem_fence()", false);
        }
      }
      else {
        bool result_shmem_fence = test_shmem_fence();
        shmem_barrier_all();
        if (mype == 0) {
          display_test_result("shmem_fence()", result_shmem_fence, false);
        }
      }

      /* Run the shmem_quiet() test */
      shmem_barrier_all();
      if ( !check_if_exists("shmem_quiet", mype) ) {
        if (mype == 0) {
          display_not_found_warning("shmem_quiet()", false);
        }
      }
      else {
        bool result_shmem_quiet = test_shmem_quiet();
        shmem_barrier_all();
        if (mype == 0) {
          display_test_result("shmem_quiet()", result_shmem_quiet, false);
        }
      }
    }
  }

  /************************* DISTRIBUTED LOCKING TESTS **************************/
  if (opts.test_locking) {
    shmem_barrier_all();
    if (mype == 0) { display_test_header("DISTRIBUTED LOCKING"); }
    shmem_barrier_all();

    /* Make sure there are at least 2 PEs */
    if ( !(npes > 1) ) {
      if (mype == 0) {
        std::cerr << RED_COLOR << "ERROR: " << RESET_COLOR << "DISTRIBUTED LOCKING tests require at least 2 PEs!" << std::endl;
      }
    }
    else {
      /* Run the shmem_set_lock and shmem_clear_lock tests */
      shmem_barrier_all();
      if ( !check_if_exists("shmem_set_lock", mype) ) {
        if (mype == 0) {
          display_not_found_warning("shmem_set_lock()", false);
        }
      }
      if ( !check_if_exists("shmem_clear_lock", mype )) {
        if (mype == 0) {
          display_not_found_warning("shmem_clear_lock()", false);
        }
      }
      
      shmem_barrier_all();
      if ( check_if_exists("shmem_set_lock", mype) && check_if_exists("shmem_clear_lock", mype) ) {
        bool result_shmem_lock_unlock = test_shmem_lock_unlock();
        shmem_barrier_all();
        if (mype == 0) {
          display_test_result("shmem_set_lock()", result_shmem_lock_unlock, false);
          display_test_result("shmem_clear_lock()", result_shmem_lock_unlock, false);
        }
      }
    }
  }

  /************************* FINALIZATION **************************/
  /* Run shmem_finalize() test */
  shmem_barrier_all();

  if ( !check_if_exists("shmem_finalize", mype) ) {
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
