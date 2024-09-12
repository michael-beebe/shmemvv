/**
 * @file cxx_mem_tests.cpp
 * @brief Contains OpenSHMEM memory management tests.
 */

#include "mem_tests.h"

/****************************************************************/
/**
 * @brief Tests the shmem_malloc() and shmem_free() functions.
 *
 * This test verifies that the shmem_malloc() function allocates memory
 * correctly and that the shmem_free() function deallocates the memory
 * correctly.
 *
 * @return True if the tests are successful, false otherwise.
 */
/****************************************************************/
bool test_shmem_malloc_free(void) {
  size_t size = 1024;
  void *ptr = shmem_malloc(size);
  if (ptr == NULL) {
    return false;
  }
  shmem_free(ptr);
  return true;
}

/****************************************************************/
/**
 * @brief Tests the shmem_ptr() routine.
 *
 * This test verifies that the shmem_ptr() function correctly provides access to
 * the memory of another PE and that the memory content is accessible and
 * correct.
 *
 * @return True if the pointer is accessible, false otherwise.
 */
/****************************************************************/
bool test_shmem_ptr() {
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();
  int *ptr = (int *)shmem_malloc(sizeof(int));

  if (ptr == NULL) {
    return false;
  }

  *ptr = mype;

  shmem_barrier_all();

  bool test_passed = true;

  for (int pe = 0; pe < npes; ++pe) {
    int *remote_ptr = (int *)shmem_ptr(ptr, pe);

    if (remote_ptr != NULL) {
      int remote_val = *remote_ptr;
      if (remote_val != pe) {
        test_passed = false;
      }
    } else if (pe == mype) {
      test_passed = false;
    }
  }

  shmem_free(ptr);
  return test_passed;
}

/****************************************************************/
/**
 * @brief Tests the shmem_addr_accessible() routine.
 *
 * This test verifies that the shmem_addr_accessible() function correctly checks
 * whether a memory address is accessible from all PEs.
 *
 * @return True if the address is accessible from all PEs, false otherwise.
 */
/****************************************************************/
bool test_shmem_addr_accessible() {
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();
  int *ptr = (int *)shmem_malloc(sizeof(int));

  if (ptr == NULL) {
    return false;
  }

  *ptr = mype;

  shmem_barrier_all();

  bool test_passed = true;

  for (int pe = 0; pe < npes; ++pe) {
    if (shmem_addr_accessible(ptr, pe) != 1) {
      test_passed = false;
    }
  }

  shmem_free(ptr);
  return test_passed;
}

/****************************************************************/
/**
 * @brief Tests the shmem_realloc() routine.
 *
 * This test verifies that the shmem_realloc() function correctly reallocates
 * memory and that the new memory block is usable.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
bool test_shmem_realloc(void) {
  size_t size = 1024;
  void *ptr = shmem_malloc(size);
  if (ptr == NULL) {
    return false;
  }
  size_t new_size = 2048;
  void *new_ptr = shmem_realloc(ptr, new_size);
  if (new_ptr == NULL) {
    shmem_free(ptr);
    return false;
  }
  shmem_free(new_ptr);
  return true;
}

/****************************************************************/
/**
 * @brief Tests the shmem_align() routine.
 *
 * This test verifies that the shmem_align() function correctly allocates memory
 * with the specified alignment.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
bool test_shmem_align(void) {
  size_t alignment = 64;
  size_t size = 1024;
  void *ptr = shmem_align(alignment, size);
  if (ptr == NULL) {
    return false;
  }
  shmem_free(ptr);
  return true;
}

/****************************************************************/
/**
 * @brief Tests the shmem_calloc() routine.
 *
 * This test verifies that the shmem_calloc() function correctly allocates and
 * initializes memory to zero.
 *
 * @return True if the test is successful, false otherwise.
 */
/****************************************************************/
bool test_shmem_calloc(void) {
  size_t count = 256;
  size_t size = sizeof(int);
  int *ptr = (int *)shmem_calloc(count, size);
  if (ptr == NULL) {
    return false;
  }
  for (size_t i = 0; i < count; ++i) {
    if (ptr[i] != 0) {
      shmem_free(ptr);
      return false;
    }
  }
  shmem_free(ptr);
  return true;
}

/****************************************************************/
/**
 * @brief Run all memory tests
 */
/****************************************************************/
void run_mem_tests(int mype, int npes) {
  /* Test shmem_malloc() and shmem_free() */
  bool result_shmem_malloc_free = test_shmem_malloc_free();
  shmem_barrier_all();
  if (mype == 0) {
    display_test_result("shmem_malloc()", result_shmem_malloc_free, false);
    display_test_result("shmem_free()", result_shmem_malloc_free, false);
  }

  /* Test shmem_ptr() */
  shmem_barrier_all();
  bool result_shmem_ptr = test_shmem_ptr();
  shmem_barrier_all();
  if (mype == 0) {
    display_test_result("shmem_ptr()", result_shmem_ptr, false);
  }

  /* Test shmem_addr_accessible() */
  shmem_barrier_all();
  bool result_shmem_addr_accessible = test_shmem_addr_accessible();
  shmem_barrier_all();
  if (mype == 0) {
    display_test_result("shmem_addr_accessible()", result_shmem_addr_accessible,
                        false);
  }

  /* Test shmem_realloc() */
  shmem_barrier_all();
  bool result_shmem_realloc = test_shmem_realloc();
  shmem_barrier_all();
  if (mype == 0) {
    display_test_result("shmem_realloc()", result_shmem_realloc, false);
  }

  /* Test shmem_align() */
  shmem_barrier_all();
  bool result_shmem_align = test_shmem_align();
  shmem_barrier_all();
  if (mype == 0) {
    display_test_result("shmem_align()", result_shmem_align, false);
  }

  /* Test shmem_calloc() */
  shmem_barrier_all();
  bool result_shmem_calloc = test_shmem_calloc();
  shmem_barrier_all();
  if (mype == 0) {
    display_test_result("shmem_calloc()", result_shmem_calloc, false);
  }
}
