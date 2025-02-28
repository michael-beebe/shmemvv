/**
 * @file c_shmem_ctx_create.c
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
  shmem_ctx_t ctx;
  int ret = shmem_ctx_create(0, &ctx);
  if (ret != 0) {
    log_fail("shmem_ctx_create returned non-zero: %d", ret);
    return false;
  }
  log_info("constructed ctx");
  shmem_ctx_destroy(ctx);
  log_info("destroyed ctx");

  return true;
}

int main(void) {
  shmem_init();
  log_init(__FILE__);

  if (!(shmem_n_pes() <= 2)) {
    if (shmem_my_pe() == 0) {
      log_fail("need at least 2 PEs, found %d", shmem_n_pes());
      display_not_enough_pes("ctx");
    }
    shmem_finalize();
    return EXIT_SUCCESS;
  }

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= test_shmem_ctx_create_destroy();

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_ctx_create()", result, false);
    display_test_result("C shmem_ctx_destroy()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
