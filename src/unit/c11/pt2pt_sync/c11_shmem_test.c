/**
 * @file c11_shmem_test.c
 * @brief Unit test shmem_test() routine.
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "log.h"
#include "shmemvv.h"

#define TIMEOUT 2

#define TEST_C_SHMEM_TEST(TYPE, TYPENAME)                                      \
  ({                                                                           \
    log_routine("c11_shmem_test(" #TYPE ")");                                  \
    bool success = true;                                                       \
    TYPE *flag = (TYPE *)shmem_malloc(sizeof(TYPE));                           \
    log_info("shmem_malloc'd flag (%d bytes) at %p", sizeof(TYPE),             \
             (void *)flag);                                                    \
    if (flag == NULL) {                                                        \
      log_fail("shmem_malloc failed! got null ptr!");                          \
      success = false;                                                         \
    } else {                                                                   \
      *flag = 0;                                                               \
      int mype = shmem_my_pe();                                                \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 0) {                                                         \
        log_info("set flag to 1");                                             \
        *flag = 1;                                                             \
        shmem_quiet();                                                         \
      }                                                                        \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype != 0) {                                                         \
        time_t start_time = time(NULL);                                        \
        log_info("waiting for flag to be one for me (pe %d)", mype);           \
        while (!shmem_##TYPENAME##_test(flag, SHMEM_CMP_EQ, 1)) {              \
          if (time(NULL) - start_time > TIMEOUT) {                             \
            log_fail("waited too long. assuming failure");                     \
            break;                                                             \
          }                                                                    \
          usleep(1000);                                                        \
        }                                                                      \
        if (*flag != 1) {                                                      \
          log_fail("flag was not expected value, expected %d got %d", 1,       \
                   (char)*flag);                                               \
          success = false;                                                     \
        } else {                                                               \
          log_info("flag is valid");                                           \
        }                                                                      \
      }                                                                        \
      shmem_free(flag);                                                        \
    }                                                                          \
    success;                                                                   \
  })

int main(int argc, char **argv) {
  shmem_init();
  log_init(__FILE__);

  int result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C_SHMEM_TEST(short, short);
  result &= TEST_C_SHMEM_TEST(int, int);
  result &= TEST_C_SHMEM_TEST(long, long);
  result &= TEST_C_SHMEM_TEST(long long, longlong);
  result &= TEST_C_SHMEM_TEST(unsigned short, ushort);
  result &= TEST_C_SHMEM_TEST(unsigned int, uint);
  result &= TEST_C_SHMEM_TEST(unsigned long, ulong);
  result &= TEST_C_SHMEM_TEST(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_TEST(int32_t, int32);
  result &= TEST_C_SHMEM_TEST(int64_t, int64);
  result &= TEST_C_SHMEM_TEST(uint32_t, uint32);
  result &= TEST_C_SHMEM_TEST(uint64_t, uint64);
  result &= TEST_C_SHMEM_TEST(size_t, size);
  result &= TEST_C_SHMEM_TEST(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_test()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
