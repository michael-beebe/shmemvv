/**
 * @file c_shmem_iget.c
 * @brief Unit test for the shmem_iget() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C_SHMEM_IGET(TYPE, TYPENAME)                                      \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_iget()");                                 \
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
        log_info("PE %d: Initialized src[%d] = %d", mype, i, i);               \
      }                                                                        \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Starting iget operation from PE 0");                     \
      log_info("PE 1: dest=%p, src=%p, dst_stride=2, src_stride=2, nelems=5",  \
               (void *)dest, (void *)src);                                     \
      shmem_##TYPENAME##_iget(dest, src, 2, 2, 5, 0);                          \
      log_info("PE 1: Completed iget operation");                              \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Beginning validation of received data");                 \
      for (int i = 0; i < 10; i += 2) {                                        \
        if (dest[i] != i / 2) {                                                \
          log_fail("PE 1: Validation failed - dest[%d] = %d, expected %d", i,  \
                   (int)dest[i], i / 2);                                       \
          success = false;                                                     \
          break;                                                               \
        }                                                                      \
        log_info("PE 1: dest[%d] = %d (valid)", i, (int)dest[i]);              \
      }                                                                        \
      if (success) {                                                           \
        log_info("PE 1: All elements validated successfully");                 \
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

  result &= TEST_C_SHMEM_IGET(long, long);
  result &= TEST_C_SHMEM_IGET(double, double);
  result &= TEST_C_SHMEM_IGET(long double, longdouble);
  result &= TEST_C_SHMEM_IGET(char, char);
  result &= TEST_C_SHMEM_IGET(signed char, schar);
  result &= TEST_C_SHMEM_IGET(short, short);
  result &= TEST_C_SHMEM_IGET(int, int);
  result &= TEST_C_SHMEM_IGET(long, long);
  result &= TEST_C_SHMEM_IGET(long long, longlong);
  result &= TEST_C_SHMEM_IGET(unsigned char, uchar);
  result &= TEST_C_SHMEM_IGET(unsigned short, ushort);
  result &= TEST_C_SHMEM_IGET(unsigned int, uint);
  result &= TEST_C_SHMEM_IGET(unsigned long, ulong);
  result &= TEST_C_SHMEM_IGET(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_IGET(int8_t, int8);
  result &= TEST_C_SHMEM_IGET(int16_t, int16);
  result &= TEST_C_SHMEM_IGET(int32_t, int32);
  result &= TEST_C_SHMEM_IGET(int64_t, int64);
  result &= TEST_C_SHMEM_IGET(uint8_t, uint8);
  result &= TEST_C_SHMEM_IGET(uint16_t, uint16);
  result &= TEST_C_SHMEM_IGET(uint32_t, uint32);
  result &= TEST_C_SHMEM_IGET(uint64_t, uint64);
  result &= TEST_C_SHMEM_IGET(size_t, size);
  result &= TEST_C_SHMEM_IGET(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_iget", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
