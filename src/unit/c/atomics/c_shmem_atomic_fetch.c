/**
 * @file c_shmem_atomic_fetch.c
 * @brief Unit test for shmem_atomic_fetch
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "shmemvv.h"

#define TEST_SHMEM_ATOMIC_FETCH(TYPE, TYPENAME)                                \
  do {                                                                         \
    static TYPE remote;                                                        \
    TYPE val;                                                                  \
    const int mype = shmem_my_pe();                                            \
    const int npes = shmem_n_pes();                                            \
    remote = (TYPE)mype;                                                       \
    shmem_barrier_all();                                                       \
    val = shmem_##TYPENAME##_atomic_fetch(&remote, (mype + 1) % npes);         \
    if (val != (TYPE)((mype + 1) % npes)) {                                    \
      printf("PE %i received incorrect value with "                            \
             "TEST_SHMEM_ATOMIC_FETCH(%s)\n",                                  \
             mype, #TYPE);                                                     \
      rc = EXIT_FAILURE;                                                       \
    }                                                                          \
  } while (false)

int main(int argc, char *argv[]) {
  shmem_init();

  bool result = true;

  result &= TEST_SHMEM_ATOMIC_FETCH(float, float);
  result &= TEST_SHMEM_ATOMIC_FETCH(double, double);
  result &= TEST_SHMEM_ATOMIC_FETCH(int, int);
  result &= TEST_SHMEM_ATOMIC_FETCH(long, long);
  result &= TEST_SHMEM_ATOMIC_FETCH(long long, longlong);
  result &= TEST_SHMEM_ATOMIC_FETCH(unsigned int, uint);
  result &= TEST_SHMEM_ATOMIC_FETCH(unsigned long, ulong);
  result &= TEST_SHMEM_ATOMIC_FETCH(unsigned long long, ulonglong);
  result &= TEST_SHMEM_ATOMIC_FETCH(int32_t, int32);
  result &= TEST_SHMEM_ATOMIC_FETCH(int64_t, int64);
  result &= TEST_SHMEM_ATOMIC_FETCH(uint32_t, uint32);
  result &= TEST_SHMEM_ATOMIC_FETCH(uint64_t, uint64);
  result &= TEST_SHMEM_ATOMIC_FETCH(size_t, size);
  result &= TEST_SHMEM_ATOMIC_FETCH(ptrdiff_t, ptrdiff);

  if (result) {
    if (shmem_my_pe() == 0) {
      printf("All tests passed.\n");
    }
  } else {
    if (shmem_my_pe() == 0) {
      printf("One or more tests failed.\n");
    }
  }

  shmem_finalize();
  return result ? EXIT_SUCCESS : EXIT_FAILURE;
}
