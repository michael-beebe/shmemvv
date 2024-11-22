/**
 * @file c_shmem_ctx_create.cpp
 * @brief Unit test for the shmem_ctx_create routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"
#include "log.h"

/**
 * @brief Tests the creation and destruction of a SHMEM context.
 *
 * This function verifies that the `shmem_ctx_create` and `shmem_ctx_destroy`
 * functions work as expected. It creates a shared memory context, checks that
 * the creation was successful, and then destroys the context.
 *
 * @return true if the test passes, false otherwise.
 */
bool test_shmem_ctx_create_destroy(void) {
  log_routine("shmem_ctx_create_destroy()");

  shmem_ctx_t ctx;
  log_info("attempting to create context");
  int ret = shmem_ctx_create(0, &ctx);
  if (ret != 0) {
    log_fail("shmem_ctx_create returned non-zero value %d", ret);
    return false;
  }
  log_info("context created successfully");

  log_info("destroying context");
  shmem_ctx_destroy(ctx);
  log_info("context destroyed");

  return true;
}

int main(void) {
  shmem_init();
  log_init(__FILE__);

  if (!(shmem_n_pes() <= 2)) {
    if (shmem_my_pe() == 0) {
      display_not_enough_pes("ctx");
    }
    log_close(EXIT_SUCCESS);
    shmem_finalize();
    return EXIT_SUCCESS;
  }

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= test_shmem_ctx_create_destroy();

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_ctx_create()", result, false);
    display_test_result("CXX shmem_ctx_destroy()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
