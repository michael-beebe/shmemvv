/**
 * @file cxx_shmem_iput.cpp
 * @brief Unit test for the shmem_iput() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_CXX_SHMEM_IPUT(TYPE, TYPENAME)                                    \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_iput()");                                 \
    bool success = true;                                                       \
    static TYPE src[10], dest[10];                                             \
    log_info("Allocated static arrays: src at %p, dest at %p",                 \
             (void *)src, (void *)dest);                                       \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    log_info("Running on PE %d of %d total PEs", mype, npes);                  \
                                                                               \
    for (int i = 0; i < 10; i++) {                                             \
      src[i] = i + mype;                                                       \
    }                                                                          \
    log_info("PE %d: Initialized src array with values %d-%d",                 \
             mype, mype, 9 + mype);                                            \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 0) {                                                           \
      log_info("PE 0: Starting strided put to PE 1");                          \
      shmem_##TYPENAME##_iput(dest, src, 2, 2, 5, 1);                          \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Beginning validation");                                  \
      for (int i = 0; i < 10; i += 2) {                                        \
        if (dest[i] != i / 2) {                                                \
          log_fail("PE 1: Validation failed - expected dest[%d]=%d, got %d",   \
                   i, i/2, (int)dest[i]);                                      \
          success = false;                                                     \
          break;                                                               \
        }                                                                      \
      }                                                                        \
      if (success) {                                                           \
        log_info("PE 1: Validation successful - all values match");            \
      }                                                                        \
    } else {                                                                   \
      log_info("PE 0: Waiting for PE 1 to complete validation");               \
    }                                                                          \
                                                                               \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  if (!(shmem_n_pes() <= 2)) {
    log_warn("Not enough PEs to run test (requires 2 PEs, have %d PEs)",
             shmem_n_pes());
    if (shmem_my_pe() == 0) {
      display_not_enough_pes("RMA");
    }
    shmem_finalize();
    return EXIT_SUCCESS;
  }

  int result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_CXX_SHMEM_IPUT(long, long);
  result &= TEST_CXX_SHMEM_IPUT(double, double);
  result &= TEST_CXX_SHMEM_IPUT(long double, longdouble);
  result &= TEST_CXX_SHMEM_IPUT(char, char);
  result &= TEST_CXX_SHMEM_IPUT(signed char, schar);
  result &= TEST_CXX_SHMEM_IPUT(short, short);
  result &= TEST_CXX_SHMEM_IPUT(int, int);
  result &= TEST_CXX_SHMEM_IPUT(long, long);
  result &= TEST_CXX_SHMEM_IPUT(long long, longlong);
  result &= TEST_CXX_SHMEM_IPUT(unsigned char, uchar);
  result &= TEST_CXX_SHMEM_IPUT(unsigned short, ushort);
  result &= TEST_CXX_SHMEM_IPUT(unsigned int, uint);
  result &= TEST_CXX_SHMEM_IPUT(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_IPUT(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_IPUT(int8_t, int8);
  result &= TEST_CXX_SHMEM_IPUT(int16_t, int16);
  result &= TEST_CXX_SHMEM_IPUT(int32_t, int32);
  result &= TEST_CXX_SHMEM_IPUT(int64_t, int64);
  result &= TEST_CXX_SHMEM_IPUT(uint8_t, uint8);
  result &= TEST_CXX_SHMEM_IPUT(uint16_t, uint16);
  result &= TEST_CXX_SHMEM_IPUT(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_IPUT(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_IPUT(size_t, size);
  result &= TEST_CXX_SHMEM_IPUT(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_iput()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
