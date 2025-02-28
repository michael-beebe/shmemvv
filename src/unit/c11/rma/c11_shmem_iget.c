/**
 * @file c11_shmem_iget.c
 * @brief Unit test for the shmem_iget() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C11_SHMEM_IGET(TYPE)                                              \
  ({                                                                           \
    log_routine("shmem_iget(" #TYPE ")");                                      \
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
      log_info("PE 1: Starting strided get from PE 0");                        \
      log_info("PE 1: dest=%p, src=%p, dest_stride=2, src_stride=2, nelems=5", \
               (void *)dest, (void *)src);                                     \
      shmem_iget(dest, src, 2, 2, 5, 0);                                       \
      log_info("PE 1: Completed strided get operation");                       \
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

  result &= TEST_C11_SHMEM_IGET(long);
  result &= TEST_C11_SHMEM_IGET(double);
  result &= TEST_C11_SHMEM_IGET(long double);
  result &= TEST_C11_SHMEM_IGET(char);
  result &= TEST_C11_SHMEM_IGET(signed char);
  result &= TEST_C11_SHMEM_IGET(short);
  result &= TEST_C11_SHMEM_IGET(int);
  result &= TEST_C11_SHMEM_IGET(long);
  result &= TEST_C11_SHMEM_IGET(long long);
  result &= TEST_C11_SHMEM_IGET(unsigned char);
  result &= TEST_C11_SHMEM_IGET(unsigned short);
  result &= TEST_C11_SHMEM_IGET(unsigned int);
  result &= TEST_C11_SHMEM_IGET(unsigned long);
  result &= TEST_C11_SHMEM_IGET(unsigned long long);
  result &= TEST_C11_SHMEM_IGET(int8_t);
  result &= TEST_C11_SHMEM_IGET(int16_t);
  result &= TEST_C11_SHMEM_IGET(int32_t);
  result &= TEST_C11_SHMEM_IGET(int64_t);
  result &= TEST_C11_SHMEM_IGET(uint8_t);
  result &= TEST_C11_SHMEM_IGET(uint16_t);
  result &= TEST_C11_SHMEM_IGET(uint32_t);
  result &= TEST_C11_SHMEM_IGET(uint64_t);
  result &= TEST_C11_SHMEM_IGET(size_t);
  result &= TEST_C11_SHMEM_IGET(ptrdiff_t);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_iget()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
