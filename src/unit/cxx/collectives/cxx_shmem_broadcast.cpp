/**
 * @file c_shmem_broadcast.cpp
 * @brief Unit test for shmem_broadcast().
 */

#include "shmemvv.h"

#define TEST_CXX_SHMEM_BROADCAST(TYPE, TYPENAME) \
  ({                                                                           \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(4 * sizeof(TYPE));                        \
    TYPE *dest = (TYPE *)shmem_malloc(4 * sizeof(TYPE));                       \
                                                                               \
    if (mype == 0) {                                                           \
      for (int i = 0; i < 4; ++i) {                                            \
        src[i] = i + 1;                                                        \
      }                                                                        \
    }                                                                          \
                                                                               \
    for (int i = 0; i < 4; ++i) {                                              \
      dest[i] = -1;                                                            \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    shmem_##TYPENAME##_broadcast(SHMEM_TEAM_WORLD, dest, src, 4, 0);           \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    bool success = true;                                                       \
    for (int i = 0; i < 4; ++i) {                                              \
      if (dest[i] != i + 1) {                                                  \
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

  result &= TEST_CXX_SHMEM_BROADCAST(float, float);
  result &= TEST_CXX_SHMEM_BROADCAST(double, double);
  result &= TEST_CXX_SHMEM_BROADCAST(long double, longdouble);
  result &= TEST_CXX_SHMEM_BROADCAST(char, char);
  result &= TEST_CXX_SHMEM_BROADCAST(signed char, schar);
  result &= TEST_CXX_SHMEM_BROADCAST(short, short);
  result &= TEST_CXX_SHMEM_BROADCAST(int, int);
  result &= TEST_CXX_SHMEM_BROADCAST(long, long);
  result &= TEST_CXX_SHMEM_BROADCAST(long long, longlong);
  result &= TEST_CXX_SHMEM_BROADCAST(unsigned char, uchar);
  result &= TEST_CXX_SHMEM_BROADCAST(unsigned short, ushort);
  result &= TEST_CXX_SHMEM_BROADCAST(unsigned int, uint);
  result &= TEST_CXX_SHMEM_BROADCAST(unsigned long, ulong);
  result &= TEST_CXX_SHMEM_BROADCAST(unsigned long long, ulonglong);
  result &= TEST_CXX_SHMEM_BROADCAST(int8_t, int8);
  result &= TEST_CXX_SHMEM_BROADCAST(int16_t, int16);
  result &= TEST_CXX_SHMEM_BROADCAST(int32_t, int32);
  result &= TEST_CXX_SHMEM_BROADCAST(int64_t, int64);
  result &= TEST_CXX_SHMEM_BROADCAST(uint8_t, uint8);
  result &= TEST_CXX_SHMEM_BROADCAST(uint16_t, uint16);
  result &= TEST_CXX_SHMEM_BROADCAST(uint32_t, uint32);
  result &= TEST_CXX_SHMEM_BROADCAST(uint64_t, uint64);
  result &= TEST_CXX_SHMEM_BROADCAST(size_t, size);
  result &= TEST_CXX_SHMEM_BROADCAST(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("CXX shmem_broadcast()", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  shmem_finalize();

  return rc;
}
