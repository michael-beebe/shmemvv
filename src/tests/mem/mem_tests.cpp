/**
 * @file mem_tests.cpp
 * @brief Contains OpenSHMEM memory management tests.
 */

#include "mem_tests.hpp"

/**
 * @brief Tests the shmem_malloc() and shmem_free() functions.
 *
 * This test verifies that the shmem_malloc() function allocates memory correctly and that
 * the shmem_free() function deallocates the memory correctly.
 *
 * @return True if the tests are successful, false otherwise.
 */
bool test_shmem_malloc_free(void) {
  size_t size = 1024;
  void *ptr = p_shmem_malloc(size);
  if (ptr == nullptr) {
    return false;
  }
  p_shmem_free(ptr);
  return true;
}

/**
 * @brief Tests the shmem_ptr() routine.
 *
 * This test verifies that the shmem_ptr() function correctly provides access to the memory
 * of another PE and that the memory content is accessible and correct.
 *
 * @return True if the pointer is accessible, false otherwise.
 */
bool test_shmem_ptr() {
  int mype = p_shmem_my_pe();
  int npes = p_shmem_n_pes();
  int *ptr = (int *)p_shmem_malloc(sizeof(int));

  if (ptr == nullptr) {
    return false;
  }

  *ptr = mype;

  p_shmem_barrier_all();

  bool test_passed = true;

  for (int pe = 0; pe < npes; ++pe) {
    int *remote_ptr = (int *)p_shmem_ptr(ptr, pe);

    if (remote_ptr != nullptr) {
      int remote_val = *remote_ptr;
      if (remote_val != pe) {
        test_passed = false;
      }
    } else if (pe == mype) {
      test_passed = false;
    }
  }

  p_shmem_free(ptr);
  return test_passed;
}

/**
 * @brief Tests the shmem_addr_accessible() routine.
 *
 * This test verifies that the shmem_addr_accessible() function correctly checks whether
 * a memory address is accessible from all PEs.
 *
 * @return True if the address is accessible from all PEs, false otherwise.
 */
bool test_shmem_addr_accessible() {
  int mype = p_shmem_my_pe();
  int npes = p_shmem_n_pes();
  int *ptr = (int *)p_shmem_malloc(sizeof(int));

  if (ptr == nullptr) {
    return false;
  }

  *ptr = mype;

  p_shmem_barrier_all();

  bool test_passed = true;

  for (int pe = 0; pe < npes; ++pe) {
    if (p_shmem_addr_accessible(ptr, pe) != 1) {
      test_passed = false;
    }
  }

  p_shmem_free(ptr);
  return test_passed;
}

/**
 * @brief Tests the shmem_realloc() routine.
 *
 * This test verifies that the shmem_realloc() function correctly reallocates memory
 * and that the new memory block is usable.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_realloc(void) {
  size_t size = 1024;
  void *ptr = p_shmem_malloc(size);
  if (ptr == nullptr) {
    return false;
  }
  size_t new_size = 2048;
  void *new_ptr = p_shmem_realloc(ptr, new_size);
  if (new_ptr == nullptr) {
    p_shmem_free(ptr);
    return false;
  }
  p_shmem_free(new_ptr);
  return true;
}

/**
 * @brief Tests the shmem_align() routine.
 *
 * This test verifies that the shmem_align() function correctly allocates memory with
 * the specified alignment.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_align(void) {
  size_t alignment = 64;
  size_t size = 1024;
  void *ptr = p_shmem_align(alignment, size);
  if (ptr == nullptr) {
    return false;
  }
  p_shmem_free(ptr);
  return true;
}

/**
 * @brief Tests the shmem_malloc_with_hints() routine.
 *
 * This test verifies that the shmem_malloc_with_hints() function correctly allocates memory
 * with the specified hints.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_malloc_with_hints(void) {
  size_t size = 1024;
  long hints = SHMEM_MALLOC_ATOMICS_REMOTE;
  void *ptr = p_shmem_malloc_with_hints(size, hints);
  if (ptr == nullptr) {
    return false;
  }
  p_shmem_free(ptr);
  return true;
}

/**
 * @brief Tests the shmem_calloc() routine.
 *
 * This test verifies that the shmem_calloc() function correctly allocates and initializes
 * memory to zero.
 *
 * @return True if the test is successful, false otherwise.
 */
bool test_shmem_calloc(void) {
  size_t count = 256;
  size_t size = sizeof(int);
  int *ptr = (int *)p_shmem_calloc(count, size);
  if (ptr == nullptr) {
    return false;
  }
  for (size_t i = 0; i < count; ++i) {
    if (ptr[i] != 0) {
      p_shmem_free(ptr);
      return false;
    }
  }
  p_shmem_free(ptr);
  return true;
}
