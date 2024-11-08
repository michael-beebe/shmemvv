#ifndef LOG_H_
#define LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <time.h>
#include <shmem.h>
#include <libgen.h>
#include <stdlib.h>
#include <sys/time.h>

#define MAX_LOG_LEN 512

/* Your step-by-step guide to adding logging to a test:
 * 1. Call once, `log_init(__FILE__)` after `shmem_init()`.
 * 2. Call `log_routine("shmem_routine_im_testing_and_
 *    maybe_the(" #TYPE ")")` whereever applicable.
 * 3. Sprinkle `log_[info,warn,fail]("my format string %s", "with my args")`
 *    as necessary.
 * 4. Call once, `log_close(return_code)` before
 *    `shmem_finalize()`.
 *
 * Logs will be placed into env(SHMEMVV_LOG_DIR) or /tmp if unset.
 */

/**
  @brief Output a line of information to the global LOGGER.

  Prefixed with current time and prefix, in that order. Affixed with \n.
 */
void log_line(const char* string, const char* prefix);

/**
   Logs an informational message to the logger.

   @param fstring Format string specifying the message to be logged.
   @param ... Variable arguments to be formatted into the message using `fstring`.
 */
void log_info(char* fstring, ...);

/**
   Logs a warning message to the logger.

   @param fstring Format string specifying the message to be logged.
   @param ... Variable arguments to be formatted into the message using `fstring`.
 */
void log_warn(char* fstring, ...);

/**
   Logs a fatal message to the logger.

   Calling this function does not cause the program to exit.
   Execution will continue past this function.

   @param fstring Format string specifying the message to be logged.
   @param ... Variable arguments to be formatted into the message using `fstring`.
 */
void log_fail(char* fstring, ...);

void log_init(const char* test_name);

void log_routine(const char* routine);

void log_close(int result);

#ifdef __cplusplus
}
#endif

#endif // _LOG_H_
