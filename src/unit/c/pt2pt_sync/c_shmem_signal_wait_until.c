/**
 * @file c_shmem_signal_wait_until.c
 * @brief Unit test shmem_signal_wait_until() routine.
 */

#include <ctime>
#include <iostream>
#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "shmemvv.h"

#define TIMEOUT 2

#define TEST_CXX_SHMEM_SIGNAL_WAIT_UNTIL()                                     \
  ({                                                                           \
    bool success = true;                                                       \
    uint64_t *flag = (uint64_t *)shmem_malloc(sizeof(uint64_t));               \
    if (flag == NULL) {                                                        \
      success = false;                                                         \
    } else {                                                                   \
      *flag = 0;                                                               \
      int mype = shmem_my_pe();                                                \
      int npes = shmem_n_pes();                                                \
      uint64_t value = 1;                                                      \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype == 0) {                                                         \
        for (int pe = 1; pe < npes; ++pe) {                                    \
          shmem_uint64_p(flag, value, pe);                                     \
        }                                                                      \
        shmem_quiet();                                                         \
      }                                                                        \
                                                                               \
      shmem_barrier_all();                                                     \
                                                                               \
      if (mype != 0) {                                                         \
        time_t start_time = time(NULL);                                        \
        while (!shmem_test(flag, SHMEM_CMP_EQ, value) &&                       \
               time(NULL) - start_time < TIMEOUT) {                            \
          shmem_signal_wait_until(flag, SHMEM_CMP_EQ, value);                  \
        }                                                                      \
        if (*flag != value) {                                                  \
          success = false;                                                     \
        }                                                                      \
      }                                                                        \
      shmem_free(flag);                                                        \
    }                                                                          \
    success;                                                                   \
  })

int main(int argc, char **argv) {
  shmem_init();

  int result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_CXX_SHMEM_SIGNAL_WAIT_UNTIL();

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_signal_wait_until()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();
  return rc;
}
