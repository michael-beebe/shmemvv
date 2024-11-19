/**
 * @file c11_shmem_test_all.c
 * @brief Tests the shmem_test_all() routine.
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "log.h"
#include "shmemvv.h"

#define TIMEOUT 2

#define TEST_C_SHMEM_TEST_ALL(TYPE, TYPENAME)                                  \
  ({                                                                           \
    log_routine("c11_shmem_test_all(" #TYPE ")");                              \
    bool success = true;                                                       \
    TYPE *flags = (TYPE *)shmem_malloc(4 * sizeof(TYPE));                      \
    log_info("shmem_malloc'd flag (%d bytes) at %p", 4 * sizeof(TYPE),         \
             (void *)flags);                                                   \
    if (flags == NULL) {                                                       \
      log_fail("shmem_malloc failed! got null ptr!");                          \
      success = false;                                                         \
    } else {                                                                   \
      for (int i = 0; i < 4; ++i) {                                            \
        flags[i] = 0;                                                          \
      }                                                                        \
      log_info("set flags to 0..=3");                                          \
      int mype = shmem_my_pe();                                                \
      int npes = shmem_n_pes();                                                \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 0) {                                                         \
        for (int pe = 1; pe < npes; ++pe) {                                    \
          log_info("copying flags to pe %d...", pe);                           \
          for (int i = 0; i < 4; ++i) {                                        \
            shmem_##TYPENAME##_p(&flags[i], 1, pe);                            \
          }                                                                    \
        }                                                                      \
        shmem_quiet();                                                         \
      }                                                                        \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype != 0) {                                                         \
        TYPE cmp_value = 1;                                                    \
        time_t start_time = time(NULL);                                        \
        log_info("waiting for flags to be sent (pe %d)", mype);                \
        while (!shmem_##TYPENAME##_test_all(flags, 4, NULL, SHMEM_CMP_EQ,      \
                                            cmp_value)) {                      \
          if (time(NULL) - start_time > TIMEOUT) {                             \
            log_info("waited too long. assuming failure");                     \
            break;                                                             \
          }                                                                    \
          usleep(500);                                                         \
        }                                                                      \
        for (int i = 0; i < 4; ++i) {                                          \
          if (flags[i] != 1) {                                                 \
            log_fail("flags[%d] was not expected value, expected %d got %d",   \
                     1, (char)flags[i]);                                       \
            success = false;                                                   \
            break;                                                             \
          }                                                                    \
        }                                                                      \
        if (success) {                                                         \
          log_info("flags are valid");                                         \
        }                                                                      \
      }                                                                        \
      shmem_free(flags);                                                       \
    }                                                                          \
    success;                                                                   \
  })

int main(int argc, char **argv) {
  shmem_init();
  log_init(__FILE__);

  int result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C_SHMEM_TEST_ALL(short, short);
  result &= TEST_C_SHMEM_TEST_ALL(int, int);
  result &= TEST_C_SHMEM_TEST_ALL(long, long);
  result &= TEST_C_SHMEM_TEST_ALL(long long, longlong);
  result &= TEST_C_SHMEM_TEST_ALL(unsigned short, ushort);
  result &= TEST_C_SHMEM_TEST_ALL(unsigned int, uint);
  result &= TEST_C_SHMEM_TEST_ALL(unsigned long, ulong);
  result &= TEST_C_SHMEM_TEST_ALL(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_TEST_ALL(int32_t, int32);
  result &= TEST_C_SHMEM_TEST_ALL(int64_t, int64);
  result &= TEST_C_SHMEM_TEST_ALL(uint32_t, uint32);
  result &= TEST_C_SHMEM_TEST_ALL(uint64_t, uint64);
  result &= TEST_C_SHMEM_TEST_ALL(size_t, size);
  result &= TEST_C_SHMEM_TEST_ALL(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_test_all()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
