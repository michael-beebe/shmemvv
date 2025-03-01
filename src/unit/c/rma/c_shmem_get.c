/**
 * @file c_shmem_get.c
 * @brief Unit test for the shmem_get() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C_SHMEM_GET(TYPE, TYPENAME)                                       \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_get()");                                  \
    bool success = true;                                                       \
    static TYPE src[10], dest[10];                                             \
    log_info("Allocated static arrays: src at %p, dest at %p", (void *)&src,   \
             (void *)&dest);                                                   \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    log_info("Running on PE %d of %d total PEs", mype, npes);                  \
                                                                               \
    if (mype == 0) {                                                           \
      for (int i = 0; i < 10; i++) {                                           \
        src[i] = i;                                                            \
      }                                                                        \
      log_info("PE 0: Initialized src array with values [0..9]");              \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Starting get of 10 elements from PE 0");                 \
      shmem_##TYPENAME##_get(dest, src, 10, 0);                                \
      log_info("PE 1: Completed get operation");                               \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Beginning validation of received data");                 \
      for (int i = 0; i < 10; i++) {                                           \
        if (dest[i] != i) {                                                    \
          log_fail("PE 1: Validation failed - dest[%d] = %d, expected %d", i,  \
                   (int)dest[i], i);                                           \
          success = false;                                                     \
          break;                                                               \
        }                                                                      \
      }                                                                        \
      if (success) {                                                           \
        log_info("PE 1: Validation successful - all elements match expected "  \
                 "values");                                                    \
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

  result &= TEST_C_SHMEM_GET(long, long);
  result &= TEST_C_SHMEM_GET(double, double);
  result &= TEST_C_SHMEM_GET(long double, longdouble);
  result &= TEST_C_SHMEM_GET(char, char);
  result &= TEST_C_SHMEM_GET(signed char, schar);
  result &= TEST_C_SHMEM_GET(short, short);
  result &= TEST_C_SHMEM_GET(int, int);
  result &= TEST_C_SHMEM_GET(long, long);
  result &= TEST_C_SHMEM_GET(long long, longlong);
  result &= TEST_C_SHMEM_GET(unsigned char, uchar);
  result &= TEST_C_SHMEM_GET(unsigned short, ushort);
  result &= TEST_C_SHMEM_GET(unsigned int, uint);
  result &= TEST_C_SHMEM_GET(unsigned long, ulong);
  result &= TEST_C_SHMEM_GET(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_GET(int8_t, int8);
  result &= TEST_C_SHMEM_GET(int16_t, int16);
  result &= TEST_C_SHMEM_GET(int32_t, int32);
  result &= TEST_C_SHMEM_GET(int64_t, int64);
  result &= TEST_C_SHMEM_GET(uint8_t, uint8);
  result &= TEST_C_SHMEM_GET(uint16_t, uint16);
  result &= TEST_C_SHMEM_GET(uint32_t, uint32);
  result &= TEST_C_SHMEM_GET(uint64_t, uint64);
  result &= TEST_C_SHMEM_GET(size_t, size);
  result &= TEST_C_SHMEM_GET(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_get", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
