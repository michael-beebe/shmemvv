/**
 * @file cxx_remote_tests.cpp
 * @brief Contains OpenSHMEM remote memory access tests.
 */

#include "cxx_remote_tests.h"

/**
 * @brief Tests the shmem_put() function.
 *
 * This test verifies that the shmem_put() function correctly transfers data from PE 0 to PE 1.
 *
 * @details PE 0 puts data into an array on PE 1.
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_put(void) {
  static long src[10], dest[10];
  int mype = p_shmem_my_pe();
  int npes = p_shmem_n_pes();

  for (int i = 0; i < 10; i++) {
    src[i] = i + mype;
  }

  p_shmem_barrier_all();

  if (mype == 0) {
    p_shmem_long_put(dest, src, 10, 1);
  }

  p_shmem_barrier_all();

  if (mype == 1) {
    for (int i = 0; i < 10; i++) {
      if (dest[i] != i) {
        return false;
      }
    }
  }

  return true;
}

/**
 * @brief Tests the shmem_p() function.
 *
 * This test verifies that the shmem_p() function correctly transfers a single data element from PE 0 to PE 1.
 *
 * @details PE 0 puts a single data element into PE 1.
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_p(void) {
  static long src, dest;
  int mype = p_shmem_my_pe();
  int npes = p_shmem_n_pes();

  src = mype;

  p_shmem_barrier_all();

  if (mype == 0) {
    p_shmem_long_p(&dest, src, 1);
  }

  p_shmem_barrier_all();

  if (mype == 1) {
    if (dest != 0) {
      return false;
    }
  }

  return true;
}

/**
 * @brief Tests the shmem_iput() function.
 *
 * This test verifies that the shmem_iput() function correctly transfers data from PE 0 to PE 1 using an indirect stride.
 *
 * @details PE 0 puts data into an array on PE 1 using an indirect stride.
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_iput(void) {
  static long src[10], dest[10];
  int mype = p_shmem_my_pe();
  int npes = p_shmem_n_pes();

  for (int i = 0; i < 10; i++) {
    src[i] = i + mype;
  }

  p_shmem_barrier_all();

  if (mype == 0) {
    p_shmem_long_iput(dest, src, 2, 2, 5, 1);
  }

  p_shmem_barrier_all();

  if (mype == 1) {
    for (int i = 0; i < 10; i += 2) {
      if (dest[i] != i / 2) {
        return false;
      }
    }
  }

  return true;
}

/**
 * @brief Tests the shmem_get() function.
 *
 * This test verifies that the shmem_get() function correctly retrieves data from PE 0 to PE 1.
 *
 * @details PE 1 gets data from an array on PE 0.
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_get(void) {
  static long src[10], dest[10];
  int mype = p_shmem_my_pe();
  int npes = p_shmem_n_pes();

  if (mype == 0) {
    for (int i = 0; i < 10; i++) {
      src[i] = i;
    }
  }

  p_shmem_barrier_all();

  if (mype == 1) {
    p_shmem_long_get(dest, src, 10, 0);
  }

  p_shmem_barrier_all();

  if (mype == 1) {
    for (int i = 0; i < 10; i++) {
      if (dest[i] != i) {
        return false;
      }
    }
  }

  return true;
}

/**
 * @brief Tests the shmem_g() function.
 *
 * This test verifies that the shmem_g() function correctly retrieves a single data element from PE 0 to PE 1.
 *
 * @details PE 1 gets a single data element from PE 0.
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_g(void) {
  static long src, dest;
  int mype = p_shmem_my_pe();
  int npes = p_shmem_n_pes();

  if (mype == 0) {
    src = 10;
  }

  p_shmem_barrier_all();

  if (mype == 1) {
    dest = p_shmem_long_g(&src, 0);
  }

  p_shmem_barrier_all();

  if (mype == 1) {
    if (dest != 10) {
      return false;
    }
  }

  return true;
}

/**
 * @brief Tests the shmem_iget() function.
 *
 * This test verifies that the shmem_iget() function correctly retrieves data from PE 0 to PE 1 using an indirect stride.
 *
 * @details PE 1 gets data from an array on PE 0 using an indirect stride.
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_iget(void) {
  static long src[10], dest[10];
  int mype = p_shmem_my_pe();
  int npes = p_shmem_n_pes();

  if (mype == 0) {
    for (int i = 0; i < 10; i++) {
      src[i] = i;
    }
  }

  p_shmem_barrier_all();

  if (mype == 1) {
    p_shmem_long_iget(dest, src, 2, 2, 5, 0);
  }

  p_shmem_barrier_all();

  if (mype == 1) {
    for (int i = 0; i < 10; i += 2) {
      if (dest[i] != i / 2) {
        return false;
      }
    }
  }

  return true;
}

/**
 * @brief Tests the shmem_put_nbi() function.
 *
 * This test verifies that the shmem_put_nbi() function correctly transfers data from PE 0 to PE 1 using non-blocking operations.
 *
 * @details PE 0 puts data into an array on PE 1 using non-blocking operations.
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_put_nbi(void) {
  static long src[10], dest[10];
  int mype = p_shmem_my_pe();
  int npes = p_shmem_n_pes();

  for (int i = 0; i < 10; i++) {
    src[i] = i + mype;
  }

  p_shmem_barrier_all();

  if (mype == 0) {
    p_shmem_long_put_nbi(dest, src, 10, 1);
    p_shmem_quiet();
  }

  p_shmem_barrier_all();

  if (mype == 1) {
    for (int i = 0; i < 10; i++) {
      if (dest[i] != i) {
        return false;
      }
    }
  }

  return true;
}

/**
 * @brief Tests the shmem_get_nbi() function.
 *
 * This test verifies that the shmem_get_nbi() function correctly retrieves data from PE 0 to PE 1 using non-blocking operations.
 *
 * @details PE 1 gets data from an array on PE 0 using non-blocking operations.
 * @return True if the test is successful, false otherwise.
 */
bool test_cxx_shmem_get_nbi(void) {
  static long src[10], dest[10];
  int mype = p_shmem_my_pe();
  int npes = p_shmem_n_pes();

  if (mype == 0) {
    for (int i = 0; i < 10; i++) {
      src[i] = i;
    }
  }

  p_shmem_barrier_all();

  if (mype == 1) {
    p_shmem_long_get_nbi(dest, src, 10, 0);
    p_shmem_quiet();
  }

  p_shmem_barrier_all();

  if (mype == 1) {
    for (int i = 0; i < 10; i++) {
      if (dest[i] != i) {
        return false;
      }
    }
  }

  return true;
}

/**
 * TODO: write docs
 * 
 */
void run_cxx_remote_tests(int mype, int npes) {
  shmem_barrier_all();
  if (mype == 0) {
    display_test_header("REMOTE MEMORY ACCESS"); 
  }

  /* Check to make sure there are at least 2 PEs */
  if ( !(npes > 1) ) {
    if (mype == 0) {
      display_not_enough_pes("REMOTE MEMORY ACCESS"); 
    }
  }
  else {
    /* Run shmem_put() test */
    shmem_barrier_all();
    if (!check_if_exists("shmem_long_put")) {
      if (mype == 0) {
        display_not_found_warning("shmem_long_put()", false);
      }
    }
    else {
      bool result_shmem_put = test_cxx_shmem_put();
      shmem_barrier_all();
      if (mype == 0) {
        display_test_result("CXX shmem_put()", result_shmem_put, false);
      }
    }

    /* Run shmem_p() test */
    shmem_barrier_all();
    if (!check_if_exists("shmem_long_p")) {
      if (mype == 0) {
        display_not_found_warning("shmem_long_p()", false);
      }
    }
    else {
      bool result_shmem_p = test_cxx_shmem_p();
      shmem_barrier_all();
      if (mype == 0) {
        display_test_result("CXX shmem_p()", result_shmem_p, false);
      }
    }

    /* Run shmem_iput() test */
    shmem_barrier_all();
    if (!check_if_exists("shmem_long_iput")) {
      if (mype == 0) {
        display_not_found_warning("shmem_long_iput()", false);
      }
    }
    else {
      bool result_shmem_iput = test_cxx_shmem_iput();
      shmem_barrier_all();
      if (mype == 0) {
        display_test_result("CXX shmem_iput()", result_shmem_iput, false);
      }
    }

    /* Run shmem_get() test */
    shmem_barrier_all();
    if (!check_if_exists("shmem_long_get")) {
      if (mype == 0) {
        display_not_found_warning("shmem_long_get()", false);
      }
    }
    else {
      bool result_shmem_get = test_cxx_shmem_get();
      shmem_barrier_all();
      if (mype == 0) {
        display_test_result("CXX shmem_get()", result_shmem_get, false);
      }
    }

    /* Run shmem_g() test */
    shmem_barrier_all();
    if (!check_if_exists("shmem_long_g")) {
      if (mype == 0) {
        display_not_found_warning("shmem_long_g()", false);
      }
    }
    else {
      bool result_shmem_g = test_cxx_shmem_g();
      shmem_barrier_all();
      if (mype == 0) {
        display_test_result("CXX shmem_g()", result_shmem_g, false);
      }
    }

    /* Run shmem_iget() test */
    shmem_barrier_all();
    if (!check_if_exists("shmem_long_iget")) {
      if (mype == 0) {
        display_not_found_warning("shmem_long_iget()", false);
      }
    }
    else {
      bool result_shmem_iget = test_cxx_shmem_iget();
      shmem_barrier_all();
      if (mype == 0) {
        display_test_result("CXX shmem_iget()", result_shmem_iget, false);
      }
    }

    /* Run shmem_put_nbi() test */
    shmem_barrier_all();
    if (!check_if_exists("shmem_long_put_nbi")) {
      if (mype == 0) {
        display_not_found_warning("shmem_long_put_nbi()", false);
      }
    }
    else {
      bool result_shmem_put_nbi = test_cxx_shmem_put_nbi();
      shmem_barrier_all();
      if (mype == 0) {
        display_test_result("CXX shmem_put_nbi()", result_shmem_put_nbi, false);
      }
    }

    /* Run shmem_get_nbi() test */
    shmem_barrier_all();
    if (!check_if_exists("shmem_long_get_nbi")) {
      if (mype == 0) {
        display_not_found_warning("shmem_long_get_nbi()", false);
      }
    }
    else {
      bool result_shmem_get_nbi = test_cxx_shmem_get_nbi();
      shmem_barrier_all();
      if (mype == 0) {
        display_test_result("CXX shmem_get_nbi()", result_shmem_get_nbi, false);
      }
    }
  }
}

