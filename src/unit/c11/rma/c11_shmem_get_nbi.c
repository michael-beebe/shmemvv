/**
 * @file c11_shmem_get_nbi.c
 * @brief Unit test for the shmem_get_nbi() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C11_SHMEM_GET_NBI(TYPE)                                           \
  ({                                                                           \
    log_routine("shmem_get_nbi(" #TYPE ")");                                       \
    bool success = true;                                                       \
    static TYPE src[10], dest[10];                                             \
    log_info("&src = %p, &dest = %p", &src, &dest);                            \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
                                                                               \
    if (mype == 0) {                                                           \
      for (int i = 0; i < 10; i++) {                                           \
        src[i] = i;                                                            \
      }                                                                        \
      log_info("set src to [0..=9]");                                          \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("getting 10 elements from src into dest on pe 0");                      \
      shmem_get_nbi(dest, src, 10, 0);                                         \
      shmem_quiet();                                                           \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("validating...");                                               \
      for (int i = 0; i < 10; i++) {                                           \
        if (dest[i] != i) {                                                    \
          log_fail("dest[%d] failed: expected %d, got %d", i, i,               \
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

  result &= TEST_C11_SHMEM_GET_NBI(long);
  result &= TEST_C11_SHMEM_GET_NBI(double);
  result &= TEST_C11_SHMEM_GET_NBI(long double);
  result &= TEST_C11_SHMEM_GET_NBI(char);
  result &= TEST_C11_SHMEM_GET_NBI(signed char);
  result &= TEST_C11_SHMEM_GET_NBI(short);
  result &= TEST_C11_SHMEM_GET_NBI(int);
  result &= TEST_C11_SHMEM_GET_NBI(long);
  result &= TEST_C11_SHMEM_GET_NBI(long long);
  result &= TEST_C11_SHMEM_GET_NBI(unsigned char);
  result &= TEST_C11_SHMEM_GET_NBI(unsigned short);
  result &= TEST_C11_SHMEM_GET_NBI(unsigned int);
  result &= TEST_C11_SHMEM_GET_NBI(unsigned long);
  result &= TEST_C11_SHMEM_GET_NBI(unsigned long long);
  result &= TEST_C11_SHMEM_GET_NBI(int8_t);
  result &= TEST_C11_SHMEM_GET_NBI(int16_t);
  result &= TEST_C11_SHMEM_GET_NBI(int32_t);
  result &= TEST_C11_SHMEM_GET_NBI(int64_t);
  result &= TEST_C11_SHMEM_GET_NBI(uint8_t);
  result &= TEST_C11_SHMEM_GET_NBI(uint16_t);
  result &= TEST_C11_SHMEM_GET_NBI(uint32_t);
  result &= TEST_C11_SHMEM_GET_NBI(uint64_t);
  result &= TEST_C11_SHMEM_GET_NBI(size_t);
  result &= TEST_C11_SHMEM_GET_NBI(ptrdiff_t);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_get_nbi()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
