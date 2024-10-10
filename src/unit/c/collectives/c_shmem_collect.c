/**
 * @file c_shmem_collect.c
 * @brief Unit test for shmem_collect().
 */

#include "shmemvv.h"

#define TEST_C_SHMEM_COLLECT(TYPE, TYPENAME) \
  ({                                                                           \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(sizeof(TYPE));                            \
    TYPE *dest = (TYPE *)shmem_malloc(npes * sizeof(TYPE));                    \
                                                                               \
    src[0] = mype;                                                             \
                                                                               \
    shmem_##TYPENAME##_collect(SHMEM_TEAM_WORLD, dest, src, 1);                \
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

  result &= TEST_C_SHMEM_COLLECT(float, float);
  result &= TEST_C_SHMEM_COLLECT(double, double);
  result &= TEST_C_SHMEM_COLLECT(long double, longdouble);
  result &= TEST_C_SHMEM_COLLECT(char, char);
  result &= TEST_C_SHMEM_COLLECT(signed char, schar);
  result &= TEST_C_SHMEM_COLLECT(short, short);
  result &= TEST_C_SHMEM_COLLECT(int, int);
  result &= TEST_C_SHMEM_COLLECT(long, long);
  result &= TEST_C_SHMEM_COLLECT(long long, longlong);
  result &= TEST_C_SHMEM_COLLECT(unsigned char, uchar);
  result &= TEST_C_SHMEM_COLLECT(unsigned short, ushort);
  result &= TEST_C_SHMEM_COLLECT(unsigned int, uint);
  result &= TEST_C_SHMEM_COLLECT(unsigned long, ulong);
  result &= TEST_C_SHMEM_COLLECT(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_COLLECT(int8_t, int8);
  result &= TEST_C_SHMEM_COLLECT(int16_t, int16);
  result &= TEST_C_SHMEM_COLLECT(int32_t, int32);
  result &= TEST_C_SHMEM_COLLECT(int64_t, int64);
  result &= TEST_C_SHMEM_COLLECT(uint8_t, uint8);
  result &= TEST_C_SHMEM_COLLECT(uint16_t, uint16);
  result &= TEST_C_SHMEM_COLLECT(uint32_t, uint32);
  result &= TEST_C_SHMEM_COLLECT(uint64_t, uint64);
  result &= TEST_C_SHMEM_COLLECT(size_t, size);
  result &= TEST_C_SHMEM_COLLECT(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (result) {
    if (shmem_my_pe() == 0) {
      display_test_result("C shmem_collect()", result, false);
    }
  } else {
    if (shmem_my_pe() == 0) {
      display_test_result("C shmem_collect()", result, false);
      rc = EXIT_FAILURE;
    }
  }

  shmem_finalize();
  return rc;
}
