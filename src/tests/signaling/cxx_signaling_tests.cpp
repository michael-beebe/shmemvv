/**
 * @file cxx_signaling_tests.cpp
 * @brief Contains OpenSHMEM signaling tests.
 */

#include "cxx_signaling_tests.h"

/**
 * @brief Tests the shmem_put_signal() routine.
 *
 * This test verifies that the shmem_put_signal() function correctly transfers a value
 * and sets a signal on the target PE.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_put_signal(void) {
  static long dest = 0;
  static long value = 12345;
  static uint64_t signal = 0;
  int mype = p_shmem_my_pe();
  int npes = p_shmem_n_pes();

  if (npes < 2) {
    return false;
  }

  int target_pe = (mype + 1) % npes;

  p_shmem_barrier_all();

  if (mype == 0) {
    p_shmem_long_put_signal(&dest, &value, 1, &signal, 1, target_pe, SHMEM_SIGNAL_SET);
  }

  p_shmem_barrier_all();

  if (mype == 1) {
    if (dest != 12345 || signal != 1) {
      return false;
    }
  }

  return true;
}

/**
 * @brief Tests the shmem_put_signal_nbi() routine.
 *
 * This test verifies that the shmem_put_signal_nbi() function correctly transfers a value
 * and sets a signal on the target PE using non-blocking operations.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_put_signal_nbi(void) {
  static long dest = 0;
  static long value = 67890;
  static uint64_t signal = 0;
  int mype = p_shmem_my_pe();
  int npes = p_shmem_n_pes();

  if (npes < 2) {
    return false;
  }

  int target_pe = (mype + 1) % npes;

  p_shmem_barrier_all();

  if (mype == 0) {
    p_shmem_long_put_signal_nbi(&dest, &value, 1, &signal, 1, target_pe, SHMEM_SIGNAL_SET);
    p_shmem_quiet();
  }

  p_shmem_barrier_all();

  if (mype == 1) {
    if (dest != 67890 || signal != 1) {
      return false;
    }
  }

  return true;
}

/**
 * @brief Tests the shmem_signal_fetch() routine.
 *
 * This test verifies that the shmem_signal_fetch() function correctly fetches the signal value
 * from the target PE.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_signal_fetch(void) {
  static uint64_t signal = 1;
  uint64_t fetched_signal = 0;
  int mype = p_shmem_my_pe();
  int npes = p_shmem_n_pes();

  if (npes < 2) {
    return false;
  }

  p_shmem_barrier_all();

  if (mype == 1) {
    fetched_signal = p_shmem_signal_fetch(&signal);
    if (fetched_signal != 1) {
      return false;
    }
  }

  return true;
}
