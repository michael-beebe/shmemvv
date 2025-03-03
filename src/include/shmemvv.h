/**
 * @file shmemvv.h
 *
 */

#ifndef SHMEMVV_H
#define SHMEMVV_H

#include <shmem.h>
#include <getopt.h>
#include <stdbool.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* ANSI color codes for pretty output */
#define RESET_COLOR "\033[0m"
#define RED_COLOR "\033[31m"
#define GREEN_COLOR "\033[32m"
#define YELLOW_COLOR "\033[33m"

#define HLINE "--------------------------------------------"

#ifdef __cplusplus
extern "C" {
#endif

/**
  @brief Print error message saying that there needs to be at least
         2 PEs for the given test type
  @param test_type Category of tests
 */
void display_not_enough_pes(const char *test_type);

/**
  @brief Displays whether the test passed
  @param routine_name OpenSHMEM routine that was tested
  @param passed True if the test passed, false if the test failed
  @param required True if the test is required, false otherwise
 */
void display_test_result(const char *routine_name, bool passed, bool required);

#ifdef __cplusplus
}
#endif

#endif /* SHMEMVV_H */
