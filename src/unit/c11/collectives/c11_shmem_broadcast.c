/**
 * @file c11_shmem_broadcast.c
 * @brief Unit test for shmem_broadcast().
 */

#include "shmemvv.h"

#define TEST_C11_SHMEM_BROADCAST(TYPE) \
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
    shmem_broadcast(SHMEM_TEAM_WORLD, dest, src, 4, 0);           \
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

  result &= TEST_C11_SHMEM_BROADCAST(float);
  result &= TEST_C11_SHMEM_BROADCAST(double);
  result &= TEST_C11_SHMEM_BROADCAST(long double);
  result &= TEST_C11_SHMEM_BROADCAST(char);
  result &= TEST_C11_SHMEM_BROADCAST(signed char);
  result &= TEST_C11_SHMEM_BROADCAST(short);
  result &= TEST_C11_SHMEM_BROADCAST(int);
  result &= TEST_C11_SHMEM_BROADCAST(long);
  result &= TEST_C11_SHMEM_BROADCAST(long long);
  result &= TEST_C11_SHMEM_BROADCAST(unsigned char);
  result &= TEST_C11_SHMEM_BROADCAST(unsigned short);
  result &= TEST_C11_SHMEM_BROADCAST(unsigned int);
  result &= TEST_C11_SHMEM_BROADCAST(unsigned long);
  result &= TEST_C11_SHMEM_BROADCAST(unsigned long long);
  result &= TEST_C11_SHMEM_BROADCAST(int8_t);
  result &= TEST_C11_SHMEM_BROADCAST(int16_t);
  result &= TEST_C11_SHMEM_BROADCAST(int32_t);
  result &= TEST_C11_SHMEM_BROADCAST(int64_t);
  result &= TEST_C11_SHMEM_BROADCAST(uint8_t);
  result &= TEST_C11_SHMEM_BROADCAST(uint16_t);
  result &= TEST_C11_SHMEM_BROADCAST(uint32_t);
  result &= TEST_C11_SHMEM_BROADCAST(uint64_t);
  result &= TEST_C11_SHMEM_BROADCAST(size_t);
  result &= TEST_C11_SHMEM_BROADCAST(ptrdiff_t);

  shmem_barrier_all();

  if (result) {
    if (shmem_my_pe() == 0) {
      display_test_result("C11 shmem_broadcast()", result, false);
    }
  } else {
    if (shmem_my_pe() == 0) {
      display_test_result("C11 shmem_broadcast()", result, false);
      rc = EXIT_FAILURE;
    }
  }

  shmem_finalize();

  return rc;
}
