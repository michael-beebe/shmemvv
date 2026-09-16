/**
 * @file c_shmem_team_ptr.c
 * @brief Unit test for shmem_team_ptr
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

/**
 * @brief Run the pointer sweep of c_shmem_ptr.c against one team.
 *
 * Identical to the loop in the shmem_ptr test, except that PEs are named by
 * their team-relative number and every result is cross-checked against
 * shmem_ptr for the same PE in SHMEM_TEAM_WORLD.  Contains no collectives, so
 * PEs that are not members of the team simply do not call it.
 */
bool test_team_ptr_sweep(shmem_team_t team, const char *label, int *ptr) {
  int mype = shmem_my_pe();
  int team_npes = shmem_team_n_pes(team);
  int team_mype = shmem_team_my_pe(team);
  log_info("[%s] Current team PE: %d, Total team PEs: %d", label, team_mype,
           team_npes);

  bool test_passed = true;

  for (int pe = 0; pe < team_npes; ++pe) {
    log_info("[%s] Testing shmem_team_ptr for team PE %d", label, pe);
    int world_pe = shmem_team_translate_pe(team, pe, SHMEM_TEAM_WORLD);
    if (world_pe < 0) {
      log_fail("[%s] shmem_team_translate_pe returned %d for team PE %d", label,
               world_pe, pe);
      test_passed = false;
      continue;
    }
    log_info("[%s] Team PE %d is PE %d in SHMEM_TEAM_WORLD", label, pe,
             world_pe);

    int *remote_ptr = (int *)shmem_team_ptr(team, ptr, pe);
    log_info("[%s] shmem_team_ptr(team, %p, %d) returned %p", label,
             (void *)ptr, pe, (void *)remote_ptr);

    /* The team spelling and the world spelling name the same PE, so they must
       hand back the same address. */
    int *world_ptr = (int *)shmem_ptr(ptr, world_pe);
    if (remote_ptr != world_ptr) {
      log_fail("[%s] Address mismatch for team PE %d (PE %d in "
               "SHMEM_TEAM_WORLD): shmem_team_ptr returned %p, shmem_ptr "
               "returned %p",
               label, pe, world_pe, (void *)remote_ptr, (void *)world_ptr);
      test_passed = false;
    }

    if (remote_ptr != NULL) {
      log_info("[%s] Validating data through remote pointer on team PE %d",
               label, pe);
      int remote_val = *remote_ptr;
      if (remote_val != world_pe) {
        log_fail("[%s] Data validation failed on team PE %d: Expected %d, got "
                 "%d at address %p",
                 label, pe, world_pe, remote_val, (void *)remote_ptr);
        test_passed = false;
      } else {
        log_info("[%s] Successfully validated data on team PE %d: value %d at "
                 "address %p",
                 label, pe, remote_val, (void *)remote_ptr);
      }
      if (world_pe == mype && remote_ptr != ptr) {
        log_fail("[%s] Team PE %d is the calling PE: expected the dest address "
                 "%p back, got %p",
                 label, pe, (void *)ptr, (void *)remote_ptr);
        test_passed = false;
      }
    } else if (world_pe == mype) {
      log_fail("[%s] shmem_team_ptr failed to return valid pointer for local "
               "PE %d",
               label, mype);
      test_passed = false;
    } else {
      log_warn("[%s] shmem_team_ptr returned NULL for remote team PE %d - "
               "Implementation may not support remote pointers",
               label, pe);
    }
  }

  return test_passed;
}

bool test_shmem_team_ptr() {
  log_routine("shmem_team_ptr()");
  log_info("Testing shmem_team_ptr functionality between PEs");

  int mype = shmem_my_pe();
  int npes = shmem_n_pes();
  log_info("Current PE: %d, Total PEs: %d", mype, npes);

  int *ptr = (int *)shmem_malloc(sizeof(int));
  log_info("Allocated %zu bytes for test variable at address %p", sizeof(int),
           (void *)ptr);

  if (ptr == NULL) {
    log_fail("Memory allocation failed: shmem_malloc returned NULL pointer");
    return false;
  }

  *ptr = mype;
  log_info("Initialized value at %p to PE number %d", (void *)ptr, mype);

  log_info("Entering barrier before pointer tests");
  shmem_barrier_all();

  bool test_passed = true;

  /* SHMEM_TEAM_INVALID must yield a null pointer. */
  log_info("Testing shmem_team_ptr with SHMEM_TEAM_INVALID");
  void *invalid_ptr = shmem_team_ptr(SHMEM_TEAM_INVALID, ptr, 0);
  log_info("shmem_team_ptr(SHMEM_TEAM_INVALID, %p, 0) returned %p", (void *)ptr,
           invalid_ptr);
  if (invalid_ptr != NULL) {
    log_fail("shmem_team_ptr returned %p for SHMEM_TEAM_INVALID, expected NULL",
             invalid_ptr);
    test_passed = false;
  }

  /* On SHMEM_TEAM_WORLD the result must be identical to shmem_ptr. */
  if (!test_team_ptr_sweep(SHMEM_TEAM_WORLD, "SHMEM_TEAM_WORLD", ptr)) {
    test_passed = false;
  }

  /* A strided sub-team, where the team PE number differs from the world PE
     number, is what separates shmem_team_ptr from shmem_ptr. */
  shmem_team_t even_team;
  int even_size = (npes + 1) / 2;
  log_info("Splitting SHMEM_TEAM_WORLD with start = 0, stride = 2, size = %d",
           even_size);
  int ret = shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, 2, even_size, NULL, 0,
                                     &even_team);
  if (ret != 0) {
    log_fail("shmem_team_split_strided failed with return code %d", ret);
    test_passed = false;
  } else {
    /* Trust the handle only as far as the team indices it yields: a negative
       team PE means the team cannot be indexed, whatever the handle claims. */
    bool member = (even_team != SHMEM_TEAM_INVALID) &&
                  (shmem_team_my_pe(even_team) >= 0) &&
                  (shmem_team_n_pes(even_team) > 0);

    if (even_team != SHMEM_TEAM_INVALID && !member) {
      log_fail("Team handle is not SHMEM_TEAM_INVALID, but shmem_team_my_pe() "
               "returned %d of %d PEs - the team cannot be indexed, so "
               "shmem_team_ptr is untestable against it here",
               shmem_team_my_pe(even_team), shmem_team_n_pes(even_team));
      test_passed = false;
    }

    if (member != (mype % 2 == 0)) {
      log_fail("Membership of the even team is wrong: PE %d %s a member", mype,
               member ? "is" : "is not");
      test_passed = false;
    }

    if (member) {
      if (!test_team_ptr_sweep(even_team, "even_team", ptr)) {
        test_passed = false;
      }
    } else {
      log_info("PE %d is not a member of the even team, skipping its sweep",
               mype);
    }

    /* Destroy is keyed off the handle, not off membership: a PE holding a
       non-SHMEM_TEAM_INVALID handle has to release it. */
    if (even_team != SHMEM_TEAM_INVALID) {
      log_info("Destroying even_team");
      shmem_team_destroy(even_team);
    }
  }

  if (test_passed) {
    log_info("All shmem_team_ptr tests completed successfully");
  } else {
    log_fail("shmem_team_ptr validation failed on one or more PEs");
  }

  log_info("Freeing allocated memory at %p", (void *)ptr);
  shmem_free(ptr);
  return test_passed;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  if (!(shmem_n_pes() >= 2)) {
    display_not_enough_pes("TEAMS");
    log_close(EXIT_SUCCESS);
    shmem_finalize();
    return EXIT_SUCCESS;
  }

  /* Whether a PE is in the sub-team depends on its PE number, so PE 0's
     verdict is not representative of the job: the result has to be reduced
     rather than printed from PE 0 alone.  reduce_test_result() reads each
     PE's flag with shmem_g, so it must live in symmetric memory. */
  bool *result = (bool *)shmem_malloc(sizeof(bool));
  if (result == NULL) {
    log_fail("Memory allocation failed: shmem_malloc returned NULL pointer");
    log_close(EXIT_FAILURE);
    shmem_finalize();
    return EXIT_FAILURE;
  }

  *result = test_shmem_team_ptr();
  int rc = EXIT_SUCCESS;

  shmem_barrier_all();
  reduce_test_result("C shmem_team_ptr", result, false);
  shmem_barrier_all();

  if (!*result) {
    rc = EXIT_FAILURE;
  }

  shmem_free(result);

  log_close(rc);
  shmem_finalize();
  return rc;
}
