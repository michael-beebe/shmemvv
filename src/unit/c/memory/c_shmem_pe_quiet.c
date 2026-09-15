/**
 * @file c_shmem_pe_quiet.c
 * @brief Unit test for the shmem_pe_quiet() and shmem_ctx_pe_quiet() routines.
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

bool test_shmem_pe_quiet(void) {
  log_routine("shmem_pe_quiet()");
  log_info("Testing shmem_pe_quiet functionality between PEs");

  long *flag = (long *)shmem_malloc(sizeof(long));
  if (flag == NULL) {
    log_fail("Memory allocation failed: shmem_malloc returned NULL pointer");
    return false;
  }
  log_info("Allocated %zu bytes for flag variable at address %p", sizeof(long),
           (void *)flag);

  *flag = 0;
  log_info("Initialized flag at %p to 0", (void *)flag);

  int mype = shmem_my_pe();
  log_info("Current PE: %d", mype);

  log_info("Entering barrier before communication");
  shmem_barrier_all();

  if (mype == 0) {
    int target_pes[1] = {1};
    log_info("PE 0: Putting value 1 to flag on PE 1");
    shmem_long_p(flag, 1, 1);
    log_info("PE 0: Calling shmem_pe_quiet with target_pes={1}, npes=1");
    shmem_pe_quiet(target_pes, 1);
    log_info("PE 0: shmem_pe_quiet returned");
  }

  log_info("Entering barrier after communication");
  shmem_barrier_all();

  bool result = true;
  if (mype == 1) {
    log_info("PE 1: Validating received value");
    if (*flag != 1) {
      log_fail("Validation failed: Expected flag value 1, got %ld", *flag);
      result = false;
    } else {
      log_info("Validation successful: Flag value is 1 as expected");
    }
  } else {
    log_info("PE %d: Waiting while PE 1 validates result", mype);
  }

  /* Test npes=0: should return immediately without error */
  log_info("PE %d: Testing shmem_pe_quiet with npes=0 (no-op)", mype);
  shmem_pe_quiet(NULL, 0);
  log_info("PE %d: shmem_pe_quiet(NULL, 0) returned successfully", mype);

  log_info("Freeing allocated memory at %p", (void *)flag);
  shmem_free(flag);

  log_info("Test completed with %s", result ? "success" : "failure");
  return result;
}

bool test_shmem_ctx_pe_quiet(void) {
  log_routine("shmem_ctx_pe_quiet()");
  log_info("Testing shmem_ctx_pe_quiet functionality between PEs");

  long *flag = (long *)shmem_malloc(sizeof(long));
  if (flag == NULL) {
    log_fail("Memory allocation failed: shmem_malloc returned NULL pointer");
    return false;
  }
  log_info("Allocated %zu bytes for flag variable at address %p", sizeof(long),
           (void *)flag);

  *flag = 0;
  log_info("Initialized flag at %p to 0", (void *)flag);

  int mype = shmem_my_pe();
  log_info("Current PE: %d", mype);

  shmem_ctx_t ctx;
  int ctx_create_status = shmem_ctx_create(0, &ctx);
  if (ctx_create_status != 0) {
    log_fail("Failed to create context");
    shmem_free(flag);
    return false;
  }
  log_info("Successfully created context");

  log_info("Entering barrier before communication");
  shmem_barrier_all();

  if (mype == 0) {
    int target_pes[1] = {1};
    log_info("PE 0: Putting value 2 to flag on PE 1 via context");
    shmem_ctx_long_p(ctx, flag, 2, 1);
    log_info("PE 0: Calling shmem_ctx_pe_quiet with target_pes={1}, npes=1");
    shmem_ctx_pe_quiet(ctx, target_pes, 1);
    log_info("PE 0: shmem_ctx_pe_quiet returned");
  }

  log_info("Entering barrier after communication");
  shmem_barrier_all();

  bool result = true;
  if (mype == 1) {
    log_info("PE 1: Validating received value");
    if (*flag != 2) {
      log_fail("Validation failed: Expected flag value 2, got %ld", *flag);
      result = false;
    } else {
      log_info("Validation successful: Flag value is 2 as expected");
    }
  } else {
    log_info("PE %d: Waiting while PE 1 validates result", mype);
  }

  /* Test npes=0: should return immediately without error */
  log_info("PE %d: Testing shmem_ctx_pe_quiet with npes=0 (no-op)", mype);
  shmem_ctx_pe_quiet(ctx, NULL, 0);
  log_info("PE %d: shmem_ctx_pe_quiet(ctx, NULL, 0) returned successfully",
           mype);

  shmem_ctx_destroy(ctx);
  log_info("Context destroyed");

  log_info("Freeing allocated memory at %p", (void *)flag);
  shmem_free(flag);

  log_info("Test completed with %s", result ? "success" : "failure");
  return result;
}

int main(int argc, char **argv) {
  shmem_init();
  log_init(__FILE__);

  if (!(shmem_n_pes() >= 2)) {
    if (shmem_my_pe() == 0) {
      display_not_enough_pes("MEMORY");
    }
    shmem_finalize();
    return EXIT_SUCCESS;
  }

  int rc = EXIT_SUCCESS;

  bool result = test_shmem_pe_quiet();
  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_pe_quiet", result, false);
  }
  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_barrier_all();

  bool result_ctx = test_shmem_ctx_pe_quiet();
  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_ctx_pe_quiet", result_ctx, false);
  }
  if (!result_ctx) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
