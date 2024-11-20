/**
 * @file c11_shmem_iput.c
 * @brief Unit test for the shmem_iput() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C11_SHMEM_IPUT(TYPE)                                              \
  ({                                                                           \
    log_routine("shmem_iput(" #TYPE ")");                                      \
    bool success = true;                                                       \
    static TYPE src[10], dest[10];                                             \
    log_info("&src = %p, &dest = %p", &src, &dest);                            \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
                                                                               \
    for (int i = 0; i < 10; i++) {                                             \
      src[i] = i + mype;                                                       \
    }                                                                          \
    log_info("set src to [%d..=%d]", mype, mype + 9);                          \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    if (mype == 0) {                                                           \
      log_info("shmem_iput(dest = %p, src = %p, dest_stride = 2, src_stride "  \
               "= 2, n = 5, pe = 0)",                                          \
               &src, &dest);                                                   \
      shmem_iput(dest, src, 2, 2, 5, 1);                                       \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("validating...");                                               \
      for (int i = 0; i < 10; i += 2) {                                        \
        if (dest[i] != i / 2) {                                                \
          log_fail("dest[%d] failed: expected %d, got %d", i, i / 2,           \
                   (char)dest[i]);                                             \
          success = false;                                                     \
          break;                                                               \
        }                                                                      \
      }                                                                        \
      log_info("result is valid");                                             \
    } else {                                                                   \
      log_info("waiting for pe 1 to verify");                                  \
    }                                                                          \
                                                                               \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  if (!(shmem_n_pes() <= 2)) {
    if (shmem_my_pe() == 0) {
      display_not_enough_pes("RMA");
    }
    shmem_finalize();
    return EXIT_SUCCESS;
  }

  int result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C11_SHMEM_IPUT(long);
  result &= TEST_C11_SHMEM_IPUT(double);
  result &= TEST_C11_SHMEM_IPUT(long double);
  result &= TEST_C11_SHMEM_IPUT(char);
  result &= TEST_C11_SHMEM_IPUT(signed char);
  result &= TEST_C11_SHMEM_IPUT(short);
  result &= TEST_C11_SHMEM_IPUT(int);
  result &= TEST_C11_SHMEM_IPUT(long);
  result &= TEST_C11_SHMEM_IPUT(long long);
  result &= TEST_C11_SHMEM_IPUT(unsigned char);
  result &= TEST_C11_SHMEM_IPUT(unsigned short);
  result &= TEST_C11_SHMEM_IPUT(unsigned int);
  result &= TEST_C11_SHMEM_IPUT(unsigned long);
  result &= TEST_C11_SHMEM_IPUT(unsigned long long);
  result &= TEST_C11_SHMEM_IPUT(int8_t);
  result &= TEST_C11_SHMEM_IPUT(int16_t);
  result &= TEST_C11_SHMEM_IPUT(int32_t);
  result &= TEST_C11_SHMEM_IPUT(int64_t);
  result &= TEST_C11_SHMEM_IPUT(uint8_t);
  result &= TEST_C11_SHMEM_IPUT(uint16_t);
  result &= TEST_C11_SHMEM_IPUT(uint32_t);
  result &= TEST_C11_SHMEM_IPUT(uint64_t);
  result &= TEST_C11_SHMEM_IPUT(size_t);
  result &= TEST_C11_SHMEM_IPUT(ptrdiff_t);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_iput()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
