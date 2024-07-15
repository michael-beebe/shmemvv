/**
  @file remote_tests.cpp
  @brief Contains OpenSHMEM remote memory access tests.
 */

#include "remote_tests.hpp"

/**
  @brief Tests the shmem_put() function.
  @details PE 0 puts data into an array on PE 1.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_put(void) {
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
  @brief Tests the shmem_p() function.
  @details PE 0 puts a single data element into PE 1.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_p(void) {
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
  @brief Tests the shmem_iput() function.
  @details PE 0 puts data into an array on PE 1 using an indirect stride.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_iput(void) {
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
  @brief Tests the shmem_get() function.
  @details PE 1 gets data from an array on PE 0.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_get(void) {
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
  @brief Tests the shmem_g() function.
  @details PE 1 gets a single data element from PE 0.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_g(void) {
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
  @brief Tests the shmem_iget() function.
  @details PE 1 gets data from an array on PE 0 using an indirect stride.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_iget(void) {
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
  @brief Tests the shmem_put_nbi() function.
  @details PE 0 puts data into an array on PE 1 using non-blocking operations.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_put_nbi(void) {
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
  @brief Tests the shmem_get_nbi() function.
  @details PE 1 gets data from an array on PE 0 using non-blocking operations.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_get_nbi(void) {
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
