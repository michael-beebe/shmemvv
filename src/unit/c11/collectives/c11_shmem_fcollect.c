/**
 * @file c11_shmem_fcollect.c
 * @brief Unit test for shmem_fcollect().
 */

#include "shmemvv.h"

#define TEST_C11_SHMEM_FCOLLECT(TYPE) \
  ({                                                                           \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(sizeof(TYPE));                            \
    TYPE *dest = (TYPE *)shmem_malloc(npes * sizeof(TYPE));                    \
                                                                               \
    src[0] = mype;                                                             \
                                                                               \
    shmem_fcollect(SHMEM_TEAM_WORLD, dest, src, 1);               \
                                                                               \
    bool success = true;                                                       \
    for (int i = 0; i < npes; ++i) {                                           \
      if (dest[i] != i) {                                                      \
        success = false;                                                       \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
                                                                               \
    shmem_free(src);                                                           \
    shmem_free(dest);                                                          \
                                                                               \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C11_SHMEM_FCOLLECT(float);
  result &= TEST_C11_SHMEM_FCOLLECT(double);
  result &= TEST_C11_SHMEM_FCOLLECT(long double);
  result &= TEST_C11_SHMEM_FCOLLECT(char);
  result &= TEST_C11_SHMEM_FCOLLECT(signed char);
  result &= TEST_C11_SHMEM_FCOLLECT(short);
  result &= TEST_C11_SHMEM_FCOLLECT(int);
  result &= TEST_C11_SHMEM_FCOLLECT(long);
  result &= TEST_C11_SHMEM_FCOLLECT(long long);
  result &= TEST_C11_SHMEM_FCOLLECT(unsigned char);
  result &= TEST_C11_SHMEM_FCOLLECT(unsigned short);
  result &= TEST_C11_SHMEM_FCOLLECT(unsigned int);
  result &= TEST_C11_SHMEM_FCOLLECT(unsigned long);
  result &= TEST_C11_SHMEM_FCOLLECT(unsigned long long);
  result &= TEST_C11_SHMEM_FCOLLECT(int8_t);
  result &= TEST_C11_SHMEM_FCOLLECT(int16_t);
  result &= TEST_C11_SHMEM_FCOLLECT(int32_t);
  result &= TEST_C11_SHMEM_FCOLLECT(int64_t);
  result &= TEST_C11_SHMEM_FCOLLECT(uint8_t);
  result &= TEST_C11_SHMEM_FCOLLECT(uint16_t);
  result &= TEST_C11_SHMEM_FCOLLECT(uint32_t);
  result &= TEST_C11_SHMEM_FCOLLECT(uint64_t);
  result &= TEST_C11_SHMEM_FCOLLECT(size_t);
  result &= TEST_C11_SHMEM_FCOLLECT(ptrdiff_t);

  shmem_barrier_all();

  if (result) {
    if (shmem_my_pe() == 0) {
      display_test_result("C11 shmem_fcollect()", result, false);
    }
  } else {
    if (shmem_my_pe() == 0) {
      display_test_result("C11 shmem_fcollect()", result, false);
      rc = EXIT_FAILURE;
    }
  }

  shmem_finalize();
  return rc;
}

