/**
 * @file c11_shmem_sync_all.c
 * @brief Unit test for shmem_sync_all().
 */

#include "log.h"
#include "shmemvv.h"

#define TEST_C11_SHMEM_SYNC_ALL()                                              \
  ({                                                                           \
    log_routine("shmem_sync_all()");                                           \
    bool success = true;                                                       \
                                                                               \
    /* Allocate counter in symmetric memory so all PEs can access it */        \
    long *shared_counter = (long *)shmem_malloc(sizeof(long));                 \
    if (shared_counter == NULL) {                                              \
      log_fail("Failed to allocate shared counter");                           \
      success = false;                                                         \
    } else {                                                                   \
      *shared_counter = 0;                                                     \
      log_info("counter @ %p", (void *)shared_counter);                        \
      shmem_barrier_all();                                                     \
                                                                               \
      log_info("executing shmem_atomic_inc");                                  \
      shmem_atomic_inc(shared_counter, 0);                                     \
                                                                               \
      log_info("executing shmem_sync_all");                                    \
      shmem_sync_all();                                                        \
                                                                               \
      /* All PEs read the counter value from PE 0 */                           \
      long final_count = shmem_long_g(shared_counter, 0);                      \
      if (final_count != shmem_n_pes()) {                                      \
        log_info("counter failed validation: expected %d increments, got %ld", \
                 shmem_n_pes(), final_count);                                  \
        success = false;                                                       \
      }                                                                        \
                                                                               \
      shmem_free(shared_counter);                                              \
    }                                                                          \
                                                                               \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = TEST_C11_SHMEM_SYNC_ALL();
  int rc = EXIT_SUCCESS;

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_sync_all", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();

  return rc;
}
