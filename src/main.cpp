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
      if (shmem_my_pe() == 0) { display_test_result("shmem_init_thread()", false, true); }
      return EXIT_FAILURE;
    }
  }
  else {
    /* Initialize with regular shmem_init() if THREADS tests are not enabled */
    if (!test_shmem_init()) {
      display_test_result("shmem_init()", false, true);
      return EXIT_FAILURE;
    }
  }

  /* Run shmem_barrier_all() test */
  if (!test_shmem_barrier_all()) {
    if (shmem_my_pe() == 0) { display_test_result("shmem_barrier_all()", false, true); }
    shmem_finalize();
    return EXIT_FAILURE;
  }

  /* Run shmem_my_pe() test */
  shmem_barrier_all();
  mype = test_shmem_my_pe();
  if (mype < 0) {
    if (mype == 0) { display_test_result("shmem_my_pe()", false, true); }
    shmem_finalize();
    return EXIT_FAILURE;
  }

  /* Run shmem_n_pes() test */
  shmem_barrier_all();
  npes = test_shmem_n_pes();
  if (npes <= 0) {
    if (mype == 0) { display_test_result("shmem_n_pes()", false, true); }
    shmem_finalize();
    return EXIT_FAILURE;
  }

  /* Run shmem_pe_accessible() test */
  shmem_barrier_all();
  if (!test_shmem_pe_accessible()) {
    if (mype == 0) { display_test_result("shmem_pe_accessible()", false, true); }
    shmem_finalize();
    return EXIT_FAILURE;
  }

  /* Display help if requested */
  shmem_barrier_all();
  if (opts.help) {
    if (mype == 0) { display_help(); }
    shmem_finalize();
    return EXIT_SUCCESS;
  }

  /* Display ASCII art logo */
  shmem_barrier_all();
  if (mype == 0) { display_logo(); }

  /* Print setup tests header */
  shmem_barrier_all();
  if (mype == 0) { display_test_header("SETUP"); }

  /* shmem_init() and shmem_my_pe() tests passed */
  shmem_barrier_all();
  if (mype == 0) {
    if (!opts.test_threads) { display_test_result("shmem_init()", true, true); }
    display_test_result("shmem_barrier_all()", true, true);
    display_test_result("shmem_my_pe()", true, true);
    display_test_result("shmem_n_pes()", true, true);
    display_test_result("shmem_pe_accessible()", true, true);
  }

  /* Run shmem_info_get_version() test */
  shmem_barrier_all();
  version = test_shmem_info_get_version();
  if (version == "") {
    if (mype == 0) { display_test_result("shmem_info_get_version()", false, false); }
  }
  else if (version != "1.5" && version != "1.50") {
    if (mype == 0) {
      std::cerr << YELLOW_COLOR << "shmem_info_get_version() test did not return 1.5... Returned " << version << std::endl;
    }
  }
  else {
    if (mype == 0) { display_test_result("shmem_info_get_version()", true, false); }
  }

  /* Run shmem_info_get_name() test */
  shmem_barrier_all();
  name = test_shmem_info_get_name();
  if (name == "") {
    if (mype == 0) { display_test_result("shmem_info_get_name()", false, false); }
  }
  else {
    if (mype == 0) { display_test_result("shmem_info_get_name()", true, false); }
  }

  /* Display test information */
  shmem_barrier_all();
  if (mype == 0) { display_test_info(name, version, npes); }

  /************************* THREADS TESTS **************************/
  shmem_barrier_all();
  if (opts.test_threads) {
    if (mype == 0) { display_test_header("THREADS"); }

    /* If we made it here shmem_init_thread() passed */
    shmem_barrier_all();
    if (mype == 0) { display_test_result("shmem_init_thread()", true, true); }

    /* Test shmem_query_thread() */
    shmem_barrier_all();
    bool result_shmem_query_thread = test_shmem_query_thread();
    shmem_barrier_all();
    if (mype == 0) { display_test_result("shmem_query_thread()", result_shmem_query_thread, false); }
  }

  /************************* MEMORY TESTS **************************/
  shmem_barrier_all();
  if (opts.test_mem) {
    if (mype == 0) { display_test_header("MEMORY MANAGEMENT"); }

    /* Test shmem_ptr() */
    shmem_barrier_all();
    bool result_shmem_ptr = test_shmem_ptr();
    shmem_barrier_all();
    if (mype == 0) { display_test_result("shmem_ptr()", result_shmem_ptr, false); }

    /* Test shmem_malloc() and shmem_free() */
    shmem_barrier_all();
    bool result_shmem_malloc_free = test_shmem_malloc_free();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("shmem_malloc()", result_shmem_malloc_free, false);
      display_test_result("shmem_free()", result_shmem_malloc_free, false);
    }

    /* Test shmem_realloc() */
    shmem_barrier_all();
    bool result_shmem_realloc = test_shmem_realloc();
    shmem_barrier_all();
    if (mype == 0) { display_test_result("shmem_realloc()", result_shmem_realloc, false); }

    /* Test shmem_align() */
    shmem_barrier_all();
    bool result_shmem_align = test_shmem_align();
    shmem_barrier_all();
    if (mype == 0) { display_test_result("shmem_align()", result_shmem_align, false); }

    /* Test shmem_malloc_with_hints() */
    shmem_barrier_all();
    bool result_shmem_malloc_with_hints = test_shmem_malloc_with_hints();
    shmem_barrier_all();
    if (mype == 0) { display_test_result("shmem_malloc_with_hints()", result_shmem_malloc_with_hints, false); }

    /* Test shmem_calloc() */
    shmem_barrier_all();
    bool result_shmem_calloc = test_shmem_calloc();
    shmem_barrier_all();
    if (mype == 0) { display_test_result("shmem_calloc()", result_shmem_calloc, false); }
  }

  /************************* TEAMS TESTS **************************/
  shmem_barrier_all();
  if (opts.test_teams) {
    if (mype == 0) { display_test_header("TEAMS MANAGEMENT"); }

    /* Run shmem_team_my_pe() test */
    shmem_barrier_all();
    bool result_shmem_team_my_pe = test_shmem_team_my_pe();
    shmem_barrier_all();
    if (mype == 0) { display_test_result("shmem_team_my_pe()", result_shmem_team_my_pe, false); }

    /* Run shmem_team_n_pes() test */
    shmem_barrier_all();
    bool result_shmem_team_n_pes = test_shmem_team_n_pes();
    shmem_barrier_all();
    if (mype == 0) { display_test_result("shmem_team_n_pes()", result_shmem_team_n_pes, false); }

    /* Run shmem_team_config_t() test */
    shmem_barrier_all();
    bool result_shmem_team_config_t = test_shmem_team_config_t();
    shmem_barrier_all();
    if (mype == 0) { display_test_result("shmem_team_config_t()", result_shmem_team_config_t, false); }

    /* Run shmem_team_get_config() test */
    shmem_barrier_all();
    bool result_shmem_team_get_config = test_shmem_team_get_config();
    shmem_barrier_all();
    if (mype == 0) { display_test_result("shmem_team_get_config()", result_shmem_team_get_config, false); }

    /* Run shmem_team_translate_pe() test */
    shmem_barrier_all();
    bool result_shmem_team_translate_pe = test_shmem_team_translate_pe();
    shmem_barrier_all();
    if (mype == 0) { display_test_result("shmem_team_translate_pe()", result_shmem_team_translate_pe, false); }

    /* Run shmem_team_split_strided() test */
    shmem_barrier_all();
    bool result_shmem_team_split_strided = test_shmem_team_split_strided();
    shmem_barrier_all();
    if (mype == 0) { display_test_result("shmem_team_split_strided()", result_shmem_team_split_strided, false); }

    /* Run shmem_team_split_2d() test */
    shmem_barrier_all();
    bool result_shmem_team_split_2d = test_shmem_team_split_2d();
    shmem_barrier_all();
    if (mype == 0) { display_test_result("shmem_team_split_2d()", result_shmem_team_split_2d, false); }

    /* Run shmem_team_destroy() test */
    shmem_barrier_all();
    bool result_shmem_team_destroy = test_shmem_team_destroy();
    shmem_barrier_all();
    if (mype == 0) { display_test_result("shmem_team_destroy()", result_shmem_team_destroy, false); }
  }

  /************************* COMMS TESTS **************************/
  shmem_barrier_all();
  if (opts.test_comms) {
    if (mype == 0) { display_test_header("COMMUNICATION / CONTEXT"); }

    /* Run shmem_ctx_create() test */
    shmem_barrier_all();
    bool result_shmem_ctx_create = test_shmem_ctx_create();
    shmem_barrier_all();
    if (mype == 0) { display_test_result("shmem_ctx_create()", result_shmem_ctx_create, false); }

    /* Run shmem_team_create_ctx() test */
    shmem_barrier_all();
    bool result_shmem_team_create_ctx = test_shmem_team_create_ctx();
    shmem_barrier_all();
    if (mype == 0) { display_test_result("shmem_team_create_ctx()", result_shmem_team_create_ctx, false); }

    /* Run shmem_ctx_destroy() test */
    shmem_barrier_all();
    bool result_shmem_ctx_destroy = test_shmem_ctx_destroy();
    shmem_barrier_all();
    if (mype == 0) { display_test_result("shmem_ctx_destroy()", result_shmem_ctx_destroy, false); }

    /* Run shmem_ctx_get_team() test */
    shmem_barrier_all();
    bool result_shmem_ctx_get_team = test_shmem_ctx_get_team();
    shmem_barrier_all();
    if (mype == 0) { display_test_result("shmem_ctx_get_team()", result_shmem_ctx_get_team, false); }
  }

  /************************* REMOTE TESTS **************************/
  shmem_barrier_all();
  if (opts.test_remote) {
    if (mype == 0) { display_test_header("REMOTE MEMORY ACCESS"); }
    
    /* Check to make sure there are at least 2 PEs */
    shmem_barrier_all();
    if ( !(npes > 1) ) {
      std::cerr << RED_COLOR << "ERROR: " << RESET_COLOR << "REMOTE MEMORY ACCESS tests require at least 2 PEs!" << std::endl;
    }
    else {
      /* Run shmem_put() test */
      shmem_barrier_all();
      bool result_shmem_put = test_shmem_put();
      shmem_barrier_all();
      if (mype == 0) { display_test_result("shmem_put()", result_shmem_put, false); }

      /* Run shmem_p() test */
      shmem_barrier_all();
      bool result_shmem_p = test_shmem_p();
      shmem_barrier_all();
      if (mype == 0) { display_test_result("shmem_p()", result_shmem_p, false); }

      /* Run shmem_iput() test */
      shmem_barrier_all();
      bool result_shmem_iput = test_shmem_iput();
      shmem_barrier_all();
      if (mype == 0) { display_test_result("shmem_iput()", result_shmem_iput, false); }

      /* Run shmem_get() test */
      shmem_barrier_all();
      bool result_shmem_get = test_shmem_get();
      shmem_barrier_all();
      if (mype == 0) { display_test_result("shmem_get()", result_shmem_get, false); }

      /* Run shmem_g() test */
      shmem_barrier_all();
      bool result_shmem_g = test_shmem_g();
      shmem_barrier_all();
      if (mype == 0) { display_test_result("shmem_g()", result_shmem_g, false); }

      /* Run shmem_iget() test */
      shmem_barrier_all();
      bool result_shmem_iget = test_shmem_iget();
      shmem_barrier_all();
      if (mype == 0) { display_test_result("shmem_iget()", result_shmem_iget, false); }

      /* Run shmem_put_nbi() test */
      shmem_barrier_all();
      bool result_shmem_put_nbi = test_shmem_put_nbi();
      shmem_barrier_all();
      if (mype == 0) { display_test_result("shmem_put_nbi()", result_shmem_put_nbi, false); }

      /* Run shmem_get_nbi() test */
      shmem_barrier_all();
      bool result_shmem_get_nbi = test_shmem_get_nbi();
      shmem_barrier_all();
      if (mype == 0) { display_test_result("shmem_get_nbi()", result_shmem_get_nbi, false); }
    }
  }


  /************************* ATOMICS TESTS **************************/
  shmem_barrier_all();
  if (opts.test_atomics) {
    if (mype == 0) { display_test_header("ATOMIC MEMORY OPS"); }

    /* Make sure there are at least 2 PEs */
    shmem_barrier_all();
    if (!(npes > 1)) {
      std::cerr << RED_COLOR << "ERROR: " << RESET_COLOR << "ATOMIC MEMORY OPS tests require at least 2 PEs!" << std::endl;
    }
    else {
      /* Run shmem_atomic_fetch() test */
      shmem_barrier_all();
      bool result_shmem_atomic_fetch = test_shmem_atomic_fetch();
      shmem_barrier_all();
      if (mype == 0) { display_test_result("shmem_atomic_fetch()", result_shmem_atomic_fetch, false); }

      /* Run shmem_atomic_set() test */
      shmem_barrier_all();
      bool result_shmem_atomic_set = test_shmem_atomic_set();
      shmem_barrier_all();
      if (mype == 0) { display_test_result("shmem_atomic_set()", result_shmem_atomic_set, false); }

      /* Run shmem_atomic_compare_swap() test */
      shmem_barrier_all();
      bool result_shmem_atomic_compare_swap = test_shmem_atomic_compare_swap();
      shmem_barrier_all();
      if (mype == 0) { display_test_result("shmem_atomic_compare_swap()", result_shmem_atomic_compare_swap, false); }

      /* Run shmem_atomic_swap() test */
      shmem_barrier_all();
      bool result_shmem_atomic_swap = test_shmem_atomic_swap();
      shmem_barrier_all();
      if (mype == 0) { display_test_result("shmem_atomic_swap()", result_shmem_atomic_swap, false); }

      /* Run shmem_atomic_fetch_inc() test */
      shmem_barrier_all();
      bool result_shmem_atomic_fetch_inc = test_shmem_atomic_fetch_inc();
      shmem_barrier_all();
      if (mype == 0) { display_test_result("shmem_atomic_fetch_inc()", result_shmem_atomic_fetch_inc, false); }

      /* Run shmem_atomic_inc() test */
      shmem_barrier_all();
      bool result_shmem_atomic_inc = test_shmem_atomic_inc();
      shmem_barrier_all();
      if (mype == 0) { display_test_result("shmem_atomic_inc()", result_shmem_atomic_inc, false); }

      /* Run shmem_atomic_fetch_add() test */
      shmem_barrier_all();
      bool result_shmem_atomic_fetch_add = test_shmem_atomic_fetch_add();
      shmem_barrier_all();
      if (mype == 0) { display_test_result("shmem_atomic_fetch_add()", result_shmem_atomic_fetch_add, false); }

      /* Run shmem_atomic_add() test */
      shmem_barrier_all();
      bool result_shmem_atomic_add = test_shmem_atomic_add();
      shmem_barrier_all();
      if (mype == 0) { display_test_result("shmem_atomic_add()", result_shmem_atomic_add, false); }

      /* Run shmem_atomic_fetch_and() test */
      shmem_barrier_all();
      bool result_shmem_atomic_fetch_and = test_shmem_atomic_fetch_and();
      shmem_barrier_all();
      if (mype == 0) { display_test_result("shmem_atomic_fetch_and()", result_shmem_atomic_fetch_and, false); }

      /* Run shmem_atomic_and() test */
      shmem_barrier_all();
      bool result_shmem_atomic_and = test_shmem_atomic_and();
      shmem_barrier_all();
      if (mype == 0) { display_test_result("shmem_atomic_and()", result_shmem_atomic_and, false); }

      /* Run shmem_atomic_fetch_or() test */
      shmem_barrier_all();
      bool result_shmem_atomic_fetch_or = test_shmem_atomic_fetch_or();
      shmem_barrier_all();
      if (mype == 0) { display_test_result("shmem_atomic_fetch_or()", result_shmem_atomic_fetch_or, false); }

      /* Run shmem_atomic_or() test */
      shmem_barrier_all();
      bool result_shmem_atomic_or = test_shmem_atomic_or();
      shmem_barrier_all();
      if (mype == 0) { display_test_result("shmem_atomic_or()", result_shmem_atomic_or, false); }

      /* Run shmem_atomic_fetch_xor() test */
      shmem_barrier_all();
      bool result_shmem_atomic_fetch_xor = test_shmem_atomic_fetch_xor();
      shmem_barrier_all();
      if (mype == 0) { display_test_result("shmem_atomic_fetch_xor()", result_shmem_atomic_fetch_xor, false); }

      /* Run shmem_atomic_xor() test */
      shmem_barrier_all();
      bool result_shmem_atomic_xor = test_shmem_atomic_xor();
      shmem_barrier_all();
      if (mype == 0) { display_test_result("shmem_atomic_xor()", result_shmem_atomic_xor, false); }

      /* Run shmem_atomic_fetch_nbi() test */
      shmem_barrier_all();
      bool result_shmem_atomic_fetch_nbi = test_shmem_atomic_fetch_nbi();
      shmem_barrier_all();
      if (mype == 0) { display_test_result("shmem_atomic_fetch_nbi()", result_shmem_atomic_fetch_nbi, false); }

      /* Run shmem_atomic_compare_swap_nbi() test */
      shmem_barrier_all();
      bool result_shmem_atomic_compare_swap_nbi = test_shmem_atomic_compare_swap_nbi();
      shmem_barrier_all();
      if (mype == 0) { display_test_result("shmem_atomic_compare_swap_nbi()", result_shmem_atomic_compare_swap_nbi, false); }

      /* Run shmem_atomic_swap_nbi() test */
      shmem_barrier_all();
      bool result_shmem_atomic_swap_nbi = test_shmem_atomic_swap_nbi();
      shmem_barrier_all();
      if (mype == 0) { display_test_result("shmem_atomic_swap_nbi()", result_shmem_atomic_swap_nbi, false); }

      /* Run shmem_atomic_fetch_inc_nbi() test */
      shmem_barrier_all();
      bool result_shmem_atomic_fetch_inc_nbi = test_shmem_atomic_fetch_inc_nbi();
      shmem_barrier_all();
      if (mype == 0) { display_test_result("shmem_atomic_fetch_inc_nbi()", result_shmem_atomic_fetch_inc_nbi, false); }

      /* Run shmem_atomic_fetch_add_nbi() test */
      shmem_barrier_all();
      bool result_shmem_atomic_fetch_add_nbi = test_shmem_atomic_fetch_add_nbi();
      shmem_barrier_all();
      if (mype == 0) { display_test_result("shmem_atomic_fetch_add_nbi()", result_shmem_atomic_fetch_add_nbi, false); }

      /* Run shmem_atomic_fetch_and_nbi() test */
      shmem_barrier_all();
      bool result_shmem_atomic_fetch_and_nbi = test_shmem_atomic_fetch_and_nbi();
      shmem_barrier_all();
      if (mype == 0) { display_test_result("shmem_atomic_fetch_and_nbi()", result_shmem_atomic_fetch_and_nbi, false); }

      /* Run shmem_atomic_fetch_or_nbi() test */
      shmem_barrier_all();
      bool result_shmem_atomic_fetch_or_nbi = test_shmem_atomic_fetch_or_nbi();
      shmem_barrier_all();
      if (mype == 0) { display_test_result("shmem_atomic_fetch_or_nbi()", result_shmem_atomic_fetch_or_nbi, false); }

      /* Run shmem_atomic_fetch_xor_nbi() test */
      shmem_barrier_all();
      bool result_shmem_atomic_fetch_xor_nbi = test_shmem_atomic_fetch_xor_nbi();
      shmem_barrier_all();
      if (mype == 0) { display_test_result("shmem_atomic_fetch_xor_nbi()", result_shmem_atomic_fetch_xor_nbi, false); }
    }
  }

  /************************* SIGNALING TESTS **************************/
  shmem_barrier_all();
  if (opts.test_signaling) {
    if (mype == 0) { display_test_header("SIGNALING OPS"); }
    /* TODO: Call signaling operations tests here */
  }

  /************************* COLLECTIVES TESTS **************************/
  shmem_barrier_all();
  if (opts.test_collectives) {
    if (mype == 0) { display_test_header("COLLECTIVE OPS"); }
    /* TODO: Call collective operations tests here */
  }

  /************************* PT2PT TESTS **************************/
  shmem_barrier_all();
  if (opts.test_pt2pt_synch) {
    if (mype == 0) { display_test_header("PT2PT SYNCHRONIZATION"); }
    /* TODO: Call point-to-point synchronization tests here */
  }

  /************************* MEM ORDERING TESTS **************************/
  shmem_barrier_all();
  if (opts.test_mem_ordering) {
    if (mype == 0) { display_test_header("MEMORY ORDERING"); }
    /* TODO: Call memory ordering tests here */
  }

  /************************* DIS LOCKING TESTS **************************/
  shmem_barrier_all();
  if (opts.test_locking) {
    if (mype == 0) { display_test_header("DISTRIBUTED LOCKING"); }
    /* TODO: Call distributed locking tests here */
  }

  /************************* FINALIZATION **************************/
  /* Run shmem_finalize() test */
  shmem_barrier_all();
  if (mype == 0) { display_test_header("FINALIZATION"); }
  if (mype == 0) { display_test_result("shmem_finalize()", test_shmem_finalize(), false); }
  if (mype == 0) { std::cout << std::endl; }

  /* We made it! End the program. */
  return EXIT_SUCCESS;
}
