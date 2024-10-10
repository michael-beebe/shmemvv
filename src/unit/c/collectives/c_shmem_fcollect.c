/**
 * @file c_shmem_fcollect.c
 * @brief Unit test for shmem_fcollect().
 */

#include "shmemvv.h"

#define TEST_C_SHMEM_FCOLLECT(TYPE, TYPENAME) \
  ({                                                                           \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(sizeof(TYPE));                            \
    TYPE *dest = (TYPE *)shmem_malloc(npes * sizeof(TYPE));                    \
                                                                               \
    src[0] = mype;                                                             \
                                                                               \
    shmem_##TYPENAME##_fcollect(SHMEM_TEAM_WORLD, dest, src, 1);               \
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

  result &= TEST_C_SHMEM_FCOLLECT(float, float);
  result &= TEST_C_SHMEM_FCOLLECT(double, double);
  result &= TEST_C_SHMEM_FCOLLECT(long double, longdouble);
  result &= TEST_C_SHMEM_FCOLLECT(char, char);
  result &= TEST_C_SHMEM_FCOLLECT(signed char, schar);
  result &= TEST_C_SHMEM_FCOLLECT(short, short);
  result &= TEST_C_SHMEM_FCOLLECT(int, int);
  result &= TEST_C_SHMEM_FCOLLECT(long, long);
  result &= TEST_C_SHMEM_FCOLLECT(long long, longlong);
  result &= TEST_C_SHMEM_FCOLLECT(unsigned char, uchar);
  result &= TEST_C_SHMEM_FCOLLECT(unsigned short, ushort);
  result &= TEST_C_SHMEM_FCOLLECT(unsigned int, uint);
  result &= TEST_C_SHMEM_FCOLLECT(unsigned long, ulong);
  result &= TEST_C_SHMEM_FCOLLECT(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_FCOLLECT(int8_t, int8);
  result &= TEST_C_SHMEM_FCOLLECT(int16_t, int16);
  result &= TEST_C_SHMEM_FCOLLECT(int32_t, int32);
  result &= TEST_C_SHMEM_FCOLLECT(int64_t, int64);
  result &= TEST_C_SHMEM_FCOLLECT(uint8_t, uint8);
  result &= TEST_C_SHMEM_FCOLLECT(uint16_t, uint16);
  result &= TEST_C_SHMEM_FCOLLECT(uint32_t, uint32);
  result &= TEST_C_SHMEM_FCOLLECT(uint64_t, uint64);
  result &= TEST_C_SHMEM_FCOLLECT(size_t, size);
  result &= TEST_C_SHMEM_FCOLLECT(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (result) {
    if (shmem_my_pe() == 0) {
      display_test_result("C shmem_fcollect()", result, false);
    }
  } else {
    if (shmem_my_pe() == 0) {
      display_test_result("C shmem_fcollect()", result, false);
      rc = EXIT_FAILURE;
    }
  }

  shmem_finalize();
  return rc;
}

