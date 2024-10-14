/**
 * @file c_shmem_sync_all.cpp
 * @brief Unit test for shmem_sync_all().
 */

#include "shmemvv.h"

#define TEST_CXX_SHMEM_SYNC_ALL()                                              \
  ({                                                                           \
    static long shared_counter;                                                \
    bool success = true;                                                       \
                                                                               \
    shared_counter = 0;                                                        \
    shmem_barrier_all();                                                       \
                                                                               \
    shmem_atomic_inc(&shared_counter, 0);                                      \
                                                                               \
    shmem_sync_all();                                                          \
                                                                               \
    if (shared_counter != shmem_n_pes()) {                                     \
      success = false;                                                         \
    }                                                                          \
                                                                               \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();

  bool result = TEST_CXX_SHMEM_SYNC_ALL();
  int rc = EXIT_SUCCESS;

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_sync_all()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();

  return EXIT_SUCCESS;
}
