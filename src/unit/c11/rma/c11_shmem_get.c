/**
 * @file c_shmem_get.c
 * @brief Unit test for the shmem_get() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C11_SHMEM_GET(TYPE)                                               \
  ({                                                                           \
    log_routine("shmem_get(" #TYPE ")");                                       \
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
      log_info("getting 10 elements from src into dest on pe 0");              \
      shmem_get(dest, src, 10, 0);                                             \
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

  result &= TEST_C11_SHMEM_GET(long);
  result &= TEST_C11_SHMEM_GET(double);
  result &= TEST_C11_SHMEM_GET(long double);
  result &= TEST_C11_SHMEM_GET(char);
  result &= TEST_C11_SHMEM_GET(signed char);
  result &= TEST_C11_SHMEM_GET(short);
  result &= TEST_C11_SHMEM_GET(int);
  result &= TEST_C11_SHMEM_GET(long);
  result &= TEST_C11_SHMEM_GET(long long);
  result &= TEST_C11_SHMEM_GET(unsigned char);
  result &= TEST_C11_SHMEM_GET(unsigned short);
  result &= TEST_C11_SHMEM_GET(unsigned int);
  result &= TEST_C11_SHMEM_GET(unsigned long);
  result &= TEST_C11_SHMEM_GET(unsigned long long);
  result &= TEST_C11_SHMEM_GET(int8_t);
  result &= TEST_C11_SHMEM_GET(int16_t);
  result &= TEST_C11_SHMEM_GET(int32_t);
  result &= TEST_C11_SHMEM_GET(int64_t);
  result &= TEST_C11_SHMEM_GET(uint8_t);
  result &= TEST_C11_SHMEM_GET(uint16_t);
  result &= TEST_C11_SHMEM_GET(uint32_t);
  result &= TEST_C11_SHMEM_GET(uint64_t);
  result &= TEST_C11_SHMEM_GET(size_t);
  result &= TEST_C11_SHMEM_GET(ptrdiff_t);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_get()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
