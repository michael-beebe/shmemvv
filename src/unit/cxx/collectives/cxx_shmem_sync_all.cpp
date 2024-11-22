/**
 * @file c_shmem_sync_all.cpp
 * @brief Unit test for shmem_sync_all().
 */

#include "shmemvv.h"
#include "log.h"

#define TEST_CXX_SHMEM_SYNC_ALL()                                              \
  ({                                                                           \
    log_routine("shmem_sync_all()");                                           \
    static long shared_counter;                                                \
    bool success = true;                                                       \
                                                                               \
    shared_counter = 0;                                                        \
    shmem_barrier_all();                                                       \
                                                                               \
    log_info("incrementing shared counter on PE 0");                           \
    shmem_atomic_inc(&shared_counter, 0);                                      \
                                                                               \
    log_info("calling shmem_sync_all");                                        \
    shmem_sync_all();                                                          \
                                                                               \
    if (shared_counter != shmem_n_pes()) {                                     \
      log_fail("shared counter value %ld does not match expected %d",          \
               shared_counter, shmem_n_pes());                                 \
      success = false;                                                         \
    } else {                                                                   \
      log_info("shared counter matches expected value");                       \
    }                                                                          \
                                                                               \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = TEST_CXX_SHMEM_SYNC_ALL();
  int rc = EXIT_SUCCESS;

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_sync_all()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }
  
  log_close(rc);
  shmem_finalize();

  return rc;
}
