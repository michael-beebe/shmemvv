/**
  @file mem_tests.cpp
  @brief Contains OpenSHMEM memory management tests.
 */

#include "mem_tests.hpp"

/**
  @brief Tests if a pointer is accessible from a remote PE.
  @return True if the pointer is accessible, false otherwise.
 */
bool test_shmem_ptr() {
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();
  int* ptr = (int*)shmem_malloc(sizeof(int));

  if (ptr == nullptr) {
    return false;
  }

  *ptr = mype;

  shmem_barrier_all();

  bool test_passed = true;

  for (int pe = 0; pe < npes; ++pe) {
    if (shmem_addr_accessible(ptr, pe) != 1) {
      test_passed = false;
    }
    else {
      int* remote_ptr = (int*)shmem_ptr(ptr, pe);
      if (pe == mype) {
        if (remote_ptr != ptr) {
          test_passed = false;
        }
      }
      else {
        if (remote_ptr == nullptr) {
          test_passed = false;
        }
      }
    }
  }

  shmem_free(ptr);
  return test_passed;
}

/**
  @brief Tests the shmem_malloc() and shmem_free() functions.
  @return True if the tests are successful, false otherwise.
 */
bool test_shmem_malloc_free(void) {
  size_t size = 1024;
  void *ptr = shmem_malloc(size);
  if (ptr == nullptr) {
    std::cerr << "shmem_malloc() failed!" << std::endl;
    return false;
  }
  shmem_free(ptr);
  return true;
}

/**
  @brief Tests the shmem_realloc() function.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_realloc(void) {
  size_t size = 1024;
  void *ptr = shmem_malloc(size);
  if (ptr == nullptr) {
    std::cerr << "shmem_malloc() failed!" << std::endl;
    return false;
  }
  size_t new_size = 2048;
  void *new_ptr = shmem_realloc(ptr, new_size);
  if (new_ptr == nullptr) {
    std::cerr << "shmem_realloc() failed!" << std::endl;
    shmem_free(ptr);
    return false;
  }
  shmem_free(new_ptr);
  return true;
}

/**
  @brief Tests the shmem_align() function.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_align(void) {
  size_t alignment = 64;
  size_t size = 1024;
  void *ptr = shmem_align(alignment, size);
  if (ptr == nullptr) {
    std::cerr << "shmem_align() failed!" << std::endl;
    return false;
  }
  shmem_free(ptr);
  return true;
}

/**
  @brief Tests the shmem_malloc_with_hints() function.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_malloc_with_hints(void) {
  size_t size = 1024;
  long hints = SHMEM_MALLOC_ATOMICS_REMOTE;
  void *ptr = shmem_malloc_with_hints(size, hints);
  if (ptr == nullptr) {
    std::cerr << "shmem_malloc_with_hints() failed!" << std::endl;
    return false;
  }
  shmem_free(ptr);
  return true;
}

/**
  @brief Tests the shmem_calloc() function.
  @return True if the test is successful, false otherwise.
 */
bool test_shmem_calloc(void) {
  size_t count = 256;
  size_t size = sizeof(int);
  int *ptr = (int*)shmem_calloc(count, size);
  if (ptr == nullptr) {
    std::cerr << "shmem_calloc() failed!" << std::endl;
    return false;
  }
  for (size_t i = 0; i < count; ++i) {
    if (ptr[i] != 0) {
      std::cerr << "shmem_calloc() did not initialize memory to zero!" << std::endl;
      shmem_free(ptr);
      return false;
    }
  }
  shmem_free(ptr);
  return true;
}
