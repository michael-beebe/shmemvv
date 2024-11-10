/**
 * @file c11_shmem_g.c
 * @brief Unit test for the shmem_g() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C11_SHMEM_G(TYPE)                                                 \
  ({                                                                           \
    log_routine("shmem_g(" #TYPE ")");                                         \
    bool success = true;                                                       \
    static TYPE src, dest;                                                     \
    log_info("&src = %p, &dest = %p", &src, &dest);                            \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
                                                                               \
    if (mype == 0) {                                                           \
      src = 10;                                                                \
      log_info("set src = 10");                                                \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("shmem_g'ing src into dest");                                   \
      dest = shmem_g(&src, 0);                                                 \
      log_info("dest = %d", (char)dest);                                       \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("validating...");                                               \
      if (dest != 10) {                                                        \
        log_fail("dest was unexpected value: expected 10, got ", (char)dest);  \
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

  result &= TEST_C11_SHMEM_G(long);
  result &= TEST_C11_SHMEM_G(double);
  result &= TEST_C11_SHMEM_G(long double);
  result &= TEST_C11_SHMEM_G(char);
  result &= TEST_C11_SHMEM_G(signed char);
  result &= TEST_C11_SHMEM_G(short);
  result &= TEST_C11_SHMEM_G(int);
  result &= TEST_C11_SHMEM_G(long);
  result &= TEST_C11_SHMEM_G(long long);
  result &= TEST_C11_SHMEM_G(unsigned char);
  result &= TEST_C11_SHMEM_G(unsigned short);
  result &= TEST_C11_SHMEM_G(unsigned int);
  result &= TEST_C11_SHMEM_G(unsigned long);
  result &= TEST_C11_SHMEM_G(unsigned long long);
  result &= TEST_C11_SHMEM_G(int8_t);
  result &= TEST_C11_SHMEM_G(int16_t);
  result &= TEST_C11_SHMEM_G(int32_t);
  result &= TEST_C11_SHMEM_G(int64_t);
  result &= TEST_C11_SHMEM_G(uint8_t);
  result &= TEST_C11_SHMEM_G(uint16_t);
  result &= TEST_C11_SHMEM_G(uint32_t);
  result &= TEST_C11_SHMEM_G(uint64_t);
  result &= TEST_C11_SHMEM_G(size_t);
  result &= TEST_C11_SHMEM_G(ptrdiff_t);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_g()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
