/**
 * @file c11_signaling_tests.c
 * @brief Contains OpenSHMEM signaling tests.
 */

#include "c11_signaling_tests.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Tests the shmem_put_signal() routine.
 *
 * This test verifies that the shmem_put_signal() function correctly transfers a value
 * and sets a signal on the target PE.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_put_signal(void) {
  static long dest = 0;
  static long value = 12345;
  static uint64_t signal = 0;
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  if (npes < 2) {
    return false;
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
 * @brief Tests the shmem_put_signal_nbi() routine.
 *
 * This test verifies that the shmem_put_signal_nbi() function correctly transfers a value
 * and sets a signal on the target PE using non-blocking operations.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_put_signal_nbi(void) {
  static long dest = 0;
  static long value = 67890;
  static uint64_t signal = 0;
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  if (npes < 2) {
    return false;
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
 * @brief Tests the shmem_signal_fetch() routine.
 *
 * This test verifies that the shmem_signal_fetch() function correctly fetches the signal value
 * from the target PE.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_c11_shmem_signal_fetch(void) {
  static uint64_t signal = 1;
  uint64_t fetched_signal = 0;
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  if (npes < 2) {
    return false;
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

/**
 * @brief Run all C11 signaling tests
 */
void run_c11_signaling_tests(int mype, int npes) {
  /* Check to make sure there are at least 2 PEs */
  if (!(npes > 1)) {
    display_not_enough_pes("SIGNALING OPS");
  }
  else {
    /* Run shmem_put_signal() test */
    shmem_barrier_all();
    bool result_shmem_put_signal = test_c11_shmem_put_signal();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_put_signal()", result_shmem_put_signal, false);
    }

    /* Run shmem_put_signal_nbi() test */
    shmem_barrier_all();
    bool result_shmem_put_signal_nbi = test_c11_shmem_put_signal_nbi();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_put_signal_nbi()", result_shmem_put_signal_nbi, false);
    }

    /* Run shmem_signal_fetch() test */
    shmem_barrier_all();
    bool result_shmem_signal_fetch = test_c11_shmem_signal_fetch();
    shmem_barrier_all();
    if (mype == 0) {
      display_test_result("C11 shmem_signal_fetch()", result_shmem_signal_fetch, false);
    }
  }
}

#ifdef __cplusplus
}
#endif

