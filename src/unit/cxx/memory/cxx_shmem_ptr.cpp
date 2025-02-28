/**
 * @file c_shmem_ptr.cpp
 * @brief Unit test for shmem_ptr
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"
#include "log.h"

bool test_shmem_ptr() {
  log_routine("shmem_ptr()");
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  log_info("attempting to allocate memory");
  int *ptr = (int *)shmem_malloc(sizeof(int));

  if (ptr == NULL) {
    log_fail("shmem_malloc returned NULL");
    return false;
  }
  log_info("successfully allocated memory at %p", (void *)ptr);

  *ptr = mype;
  log_info("initialized local value to %d", mype);

  shmem_barrier_all();

  bool test_passed = true;

  for (int pe = 0; pe < npes; ++pe) {
    log_info("attempting to get pointer to PE %d's memory", pe);
    int *remote_ptr = (int *)shmem_ptr(ptr, pe);

    if (remote_ptr != NULL) {
      int remote_val = *remote_ptr;
      log_info("got remote value %d from PE %d", remote_val, pe);
      if (remote_val != pe) {
        log_fail("incorrect value %d from PE %d (expected %d)", remote_val, pe,
                 pe);
        test_passed = false;
      }
    } else if (pe == mype) {
      log_fail("shmem_ptr returned NULL for local PE");
      test_passed = false;
    } else {
      log_info("shmem_ptr returned NULL for PE %d (may be expected)", pe);
    }
  }

  log_info("freeing allocated memory");
  shmem_free(ptr);
  return test_passed;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = test_shmem_ptr();
  int rc = EXIT_SUCCESS;

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_ptr()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
