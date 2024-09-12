/**
 * @file cxx_comms_tests.cpp
 * @brief Contains OpenSHMEM communication/context tests.
 */

#include "comms_tests.h"

/****************************************************************/
/**
 * @brief Tests the shmem_ctx_create() function.
 *
 * This test verifies that the shmem_ctx_create() function correctly creates a
 * communication context.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
bool test_shmem_ctx_create(void) {
  shmem_ctx_t ctx;
  int ret = shmem_ctx_create(0, &ctx);
  if (ret != 0) {
    return false;
  }
  shmem_ctx_destroy(ctx);
  return true;
}

/****************************************************************/
/**
 * @brief Tests the shmem_ctx_destroy() function.
 *
 * This test verifies that the shmem_ctx_destroy() function correctly destroys a
 * communication context.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
bool test_shmem_ctx_destroy(void) {
  shmem_ctx_t ctx;
  shmem_ctx_create(0, &ctx);
  shmem_ctx_destroy(ctx);
  return true;
}

/****************************************************************/
/**
 * @brief Run all comms/ctx tests
 */
/****************************************************************/
void run_comms_tests(int mype, int npes) {
  /* Run shmem_ctx_create() test */
  bool result_shmem_ctx_create = test_shmem_ctx_create();
  shmem_barrier_all();
  if (mype == 0) {
    display_test_result("shmem_ctx_create()", result_shmem_ctx_create, false);
  }

  /* Run shmem_ctx_destroy() test */
  shmem_barrier_all();
  bool result_shmem_ctx_destroy = test_shmem_ctx_destroy();
  shmem_barrier_all();
  if (mype == 0) {
    display_test_result("shmem_ctx_destroy()", result_shmem_ctx_destroy, false);
  }
}
