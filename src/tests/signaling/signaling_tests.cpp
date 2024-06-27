/**
  @file signaling_tests.cpp
  @brief Contains OpenSHMEM signaling tests.
 */

#include "signaling_tests.hpp"

/**
  @brief Tests the shmem_put_signal() routine.
  @details This function tests the shmem_put_signal() routine by putting a value
           and a signal to a target PE and verifying the results.
  @return True if the test passes, false otherwise.
  @note Requires more than one PE.
 */
bool test_shmem_put_signal(void) {
  static long dest = 0;
  static long value = 12345;
  static uint64_t signal = 0;
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  if (npes < 2) {
    return false;  /* Test requires more than one PE */
  }

  int target_pe = (mype + 1) % npes;

  shmem_barrier_all();

  if (mype == 0) {
    shmem_put_signal(&dest, &value, 1, &signal, 1, target_pe, SHMEM_SIGNAL_SET);
  }

  shmem_barrier_all();

  if (mype == 1) {
    if (dest != 12345 || signal != 1) {
      return false;
    }
  }

  return true;
}

/**
  @brief Tests the shmem_put_signal_nbi() routine.
  @details This function tests the shmem_put_signal_nbi() routine by putting a value
           and a signal to a target PE in a non-blocking manner and verifying the results.
  @return True if the test passes, false otherwise.
  @note Requires more than one PE.
 */
bool test_shmem_put_signal_nbi(void) {
  static long dest = 0;
  static long value = 67890;
  static uint64_t signal = 0;
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  if (npes < 2) {
    return false;  /* Test requires more than one PE */
  }

  int target_pe = (mype + 1) % npes;

  shmem_barrier_all();

  if (mype == 0) {
    shmem_put_signal_nbi(&dest, &value, 1, &signal, 1, target_pe, SHMEM_SIGNAL_SET);
    shmem_quiet();
  }

  shmem_barrier_all();

  if (mype == 1) {
    if (dest != 67890 || signal != 1) {
      return false;
    }
  }

  return true;
}

/**
  @brief Tests the shmem_signal_fetch() routine.
  @details This function tests the shmem_signal_fetch() routine by fetching a signal
           from a target PE and verifying the result.
  @return True if the test passes, false otherwise.
  @note Requires more than one PE.
 */
bool test_shmem_signal_fetch(void) {
  static uint64_t signal = 1;
  uint64_t fetched_signal = 0;
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  if (npes < 2) {
    return false;  /* Test requires more than one PE */
  }

  shmem_barrier_all();

  if (mype == 1) {
    fetched_signal = shmem_signal_fetch(&signal);
    if (fetched_signal != 1) {
      return false;
    }
  }

  return true;
}
