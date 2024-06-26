/**
  @file shmemvv.hpp
  @brief Contains helper function declarations
 */

#ifndef SHMEMVV_HPP
#define SHMEMVV_HPP

#include <shmem.h>
#include <iostream>
#include <getopt.h>
#include <string>
#include <vector>
#include <sstream>

#include "tests/setup/setup_tests.hpp"
#include "tests/threads/threads_tests.hpp"
#include "tests/mem/mem_tests.hpp"
#include "tests/teams/teams_tests.hpp"
#include "tests/comms/comms_tests.hpp"
#include "tests/remote/remote_tests.hpp"
#include "tests/atomics/atomics_tests.hpp"
#include "tests/signaling/signaling_tests.hpp"
#include "tests/collectives/collectives_tests.hpp"
#include "tests/pt2pt/pt2pt_tests.hpp"
#include "tests/mem_ordering/mem_ordering_tests.hpp"
#include "tests/locking/locking_tests.hpp"

/* Struct to hold selected tests */
struct test_options {
  bool all;
  bool test_setup;
  bool test_threads;
  bool test_mem;
  bool test_teams;
  bool test_comms;
  bool test_remote;
  bool test_atomics;
  bool test_signaling;
  bool test_collectives;
  bool test_pt2pt_synch;
  bool test_mem_ordering;
  bool test_locking;
  bool help;

  test_options() :
    all(false), test_setup(false), test_threads(false),
    test_mem(false), test_teams(false), test_comms(false),
    test_remote(false), test_atomics(false), test_signaling(false),
    test_collectives(false), test_pt2pt_synch(false),
    test_mem_ordering(false), test_locking(false), help(false) {}
};

/* Function to parse command-line options */
bool parse_opts(int argc, char *argv[], test_options &opts);

/* Function to display usage information */
void display_help();

/* Function to display logo */
void display_logo();

/* Function to display test header */
void display_test_header(std::string test_name);

/* Function to display test info */
void display_test_info(
  std::string shmem_name,
  std::string shmem_version,
  int npes
);

#endif /* SHMEMVV_HPP */
