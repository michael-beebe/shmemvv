/**
 * @file c11_shmem_sync_all.c
 * @brief Unit test for shmem_sync_all().
 */

#include "shmemvv.h"

#define TEST_C11_SHMEM_SYNC_ALL() \
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

  bool result = TEST_C11_SHMEM_SYNC_ALL();

  shmem_barrier_all();

  if (result) {
    if (shmem_my_pe() == 0) {
      display_test_result("C11 shmem_sync_all()", result, false);
    }
  } else {
    if (shmem_my_pe() == 0) {
      display_test_result("C11 shmem_sync_all()", result, true);
    }
  }

  shmem_finalize();

  return EXIT_SUCCESS;
}
