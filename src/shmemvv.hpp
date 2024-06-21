#ifndef SHMEMVV_HPP
#define SHMEMVV_HPP

#include <shmem.h> /* we're gonna need this... */

#include <iostream>
#include <getopt.h>
#include <string.h>

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

#endif /* SHMEMVV_HPP */
