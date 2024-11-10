/**
 * @file c11_shmem_p.c
 * @brief Unit test for the shmem_p() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C11_SHMEM_P(TYPE)                                                 \
  ({                                                                           \
    log_routine("shmem_2(" #TYPE ")"); \
    bool success = true;                                                       \
    static TYPE src, dest;                                                     \
    log_info("&src = %p, &dest = %p", src, dest);                              \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
                                                                               \
    src = mype;                                                                \
    log_info("set src = %d", mype);                                                \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    if (mype == 0) {                                                           \
      log_info("shmem_p'ing %d into dest", src); \
      shmem_p(&dest, src, 1);                                                  \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("validating...");                                               \
      if (dest != 0) {                                                         \
        log_fail("dest was unexpected value: expected 0, got ", (char)dest);  \
        success = false;                                                       \
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
    log_warn("not enough pes for test (need 2, have %d)", shmem_n_pes());
    if (shmem_my_pe() == 0) {
      display_not_enough_pes("RMA");
    }
    shmem_finalize();
    return EXIT_SUCCESS;
  }

  int result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C11_SHMEM_P(long);
  result &= TEST_C11_SHMEM_P(double);
  result &= TEST_C11_SHMEM_P(long double);
  result &= TEST_C11_SHMEM_P(char);
  result &= TEST_C11_SHMEM_P(signed char);
  result &= TEST_C11_SHMEM_P(short);
  result &= TEST_C11_SHMEM_P(int);
  result &= TEST_C11_SHMEM_P(long);
  result &= TEST_C11_SHMEM_P(long long);
  result &= TEST_C11_SHMEM_P(unsigned char);
  result &= TEST_C11_SHMEM_P(unsigned short);
  result &= TEST_C11_SHMEM_P(unsigned int);
  result &= TEST_C11_SHMEM_P(unsigned long);
  result &= TEST_C11_SHMEM_P(unsigned long long);
  result &= TEST_C11_SHMEM_P(int8_t);
  result &= TEST_C11_SHMEM_P(int16_t);
  result &= TEST_C11_SHMEM_P(int32_t);
  result &= TEST_C11_SHMEM_P(int64_t);
  result &= TEST_C11_SHMEM_P(uint8_t);
  result &= TEST_C11_SHMEM_P(uint16_t);
  result &= TEST_C11_SHMEM_P(uint32_t);
  result &= TEST_C11_SHMEM_P(uint64_t);
  result &= TEST_C11_SHMEM_P(size_t);
  result &= TEST_C11_SHMEM_P(ptrdiff_t);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_p()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
