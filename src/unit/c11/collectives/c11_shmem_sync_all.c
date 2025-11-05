/**
 * @file c11_shmem_sync.c
 * @brief Unit test for the shmem_team_sync() routine.
 */

#include <shmem.h>
#include <unistd.h> 

#include "log.h"
#include "shmemvv.h"

int test_shmem_sync_all(){
  log_routine("shmem_team_sync()");
  bool success = true;
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  /* Allocate counter in symmetric memory so all PEs can access it */
  long *shared_counter = (long *)shmem_malloc(sizeof(long));
  if (shared_counter == NULL) {
    log_fail("Failed to allocate shared counter");
    return false;
  }
  *shared_counter = 0;

  log_info("Shared counter initialized at address %p", (void *)shared_counter);
  log_info("Initialized shared counter to 0");
  shmem_barrier_all();
  log_info("Completed initial barrier synchronization");

  if (mype != 0) {
    /* Sleep incrementing PEs to give sync a chance to fail */
    usleep((int)0.25E+6); // sleep for a quarter of a second
    log_info("PE %d Performing atomic increment on shared counter at PE 0", mype);
    shmem_atomic_inc(shared_counter, 0);
    shmem_quiet();

    log_info("Initiating team synchronization");
    shmem_sync_all();
    log_info("Team synchronization completed");
  }
  else{ /* mype == 0 */
    log_info("Initiating team synchronization");
    /* If sync fails, pe will move to validation immediately, failing test */
    shmem_sync_all();
    log_info("Team synchronization completed");

    int expected = npes - 1;
    if (*shared_counter != expected){ 
      log_fail("Shared counter validation failed: expected %d, got %ld",
             expected, *shared_counter);
      if (*shared_counter < expected) {
        log_fail("Team synchronization may have failed to propagate all atomic "
               "operations");
      } else {
        log_fail("Either team split created incorrect number of teams or team "
                "sync duplicated operations");
      }
      success = false;
    }
    else {
      log_info("Shared counter validation successful: value matches expected %d",
             expected);
      success = true; 
    }
  }

  shmem_barrier_all();
  shmem_free(shared_counter);
  return success;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  log_info("Running on PE %d of %d total PEs", mype, npes);

  if (!(npes >= 2)) {
    if (mype == 0) {
      log_fail("Test requires at least 2 PEs, but only %d PE(s) available",
               npes);
      display_not_enough_pes("TEAMS");
    }
    shmem_finalize();
    return EXIT_SUCCESS;
  }
  static int result;
  result = test_shmem_sync_all();

  reduce_test_result("C11 shmem_sync_all", &result, false);

  log_info("Test completed with %s", result ? "SUCCESS" : "FAILURE");
  int rc = result ? EXIT_SUCCESS : EXIT_FAILURE;
  log_close(rc);
  shmem_finalize();
  return rc;
}

