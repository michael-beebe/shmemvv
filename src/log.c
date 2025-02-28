#include "log.h"

#ifdef __cplusplus
extern "C" {
#endif

/* I'm not a fan of the global either.
 * But,
 * - The log output can change between a file or STDOUT based on initialization.
 * - The log output is kept seperate between executables.
 * - Passing file pointers to every single test is cumbersome.
 * And so a global.
 */
FILE *LOGGER = NULL;

/**
  @brief Output a line of information to the global LOGGER.

  Prefixed with current time and prefix, in that order. Affixed with \n.
 */
void log_line(const char *string, const char *prefix) {
  time_t time_since_epoch = time(NULL);
  struct tm *timeinfo;
  struct timeval tv;

  time(&time_since_epoch);
  timeinfo = localtime(&time_since_epoch);
  gettimeofday(&tv, NULL);

  fprintf(LOGGER, "[%02d:%02d:%02d.%03ld] %s%s\n", timeinfo->tm_hour,
          timeinfo->tm_min, timeinfo->tm_sec, (long int)tv.tv_usec / 1000,
          prefix, string);
}

/**
   Logs an informational message to the logger.

   @param fstring Format string specifying the message to be logged.
   @param ... Variable arguments to be formatted into the message using
   `fstring`.
 */
void log_info(const char *fstring, ...) {
  va_list va;
  char buf[MAX_LOG_LEN];
  va_start(va, fstring);
  if (vsnprintf(buf, MAX_LOG_LEN, fstring, va) > MAX_LOG_LEN) {
    log_line(buf, "[INFO] (trunc) ");
  } else {
    log_line(buf, "[INFO] ");
  }
  va_end(va);
}

/**
   Logs a warning message to the logger.

   @param fstring Format string specifying the message to be logged.
   @param ... Variable arguments to be formatted into the message using
   `fstring`.
 */
void log_warn(const char *fstring, ...) {
  va_list va;
  char buf[MAX_LOG_LEN];
  va_start(va, fstring);
  if (vsnprintf(buf, MAX_LOG_LEN, fstring, va) > MAX_LOG_LEN) {
    log_line(buf, "[WARN] (trunc) ");
  } else {
    log_line(buf, "[WARN] ");
  }
  va_end(va);
}

/**
   Logs a fatal message to the logger.

   Calling this function does not cause the program to exit.

   @param fstring Format string specifying the message to be logged.
   @param ... Variable arguments to be formatted into the message using
   `fstring`.
 */
void log_fail(const char *fstring, ...) {
  va_list va;
  char buf[MAX_LOG_LEN];
  va_start(va, fstring);
  if (vsnprintf(buf, MAX_LOG_LEN, fstring, va) > MAX_LOG_LEN) {
    log_line(buf, "[FAIL] (trunc) ");
  } else {
    log_line(buf, "[FAIL] ");
  }
  va_end(va);
}

void log_routine(const char *routine) {
  log_line(&routine[0], "------ BEGIN TEST ROUTINE: ");
}

void log_init(const char *test_name) {
  if (LOGGER)
    return;
  const char *path_prefix = getenv("SHMEMVV_LOG_DIR");
  if (!path_prefix)
    path_prefix = "/tmp/";
  char path_buf[256];
  snprintf(path_buf, 256, "%s%s.pe%02d.log", path_prefix,
           basename(strdup(test_name)), shmem_my_pe());
  FILE *maybe_file = fopen(path_buf, "w+");
  if (maybe_file == NULL) {
    fprintf(stderr,
            "Failed to open log file (error %d: %s). Writing to STDOUT.\n",
            errno, strerror(errno));
    maybe_file = stdout;
  }
  LOGGER = maybe_file ? maybe_file : stderr;
  fprintf(LOGGER, "---------- BEGIN TEST: %s\n", test_name);
}

void log_close(int failed) {
  fprintf(LOGGER, "---------- END TEST: %s\n",
          failed == 0 ? "PASSED" : "FAILED");
  fclose(LOGGER);
}

#ifdef __cplusplus
}
#endif
