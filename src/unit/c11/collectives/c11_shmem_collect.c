/**
 * @file c11_shmem_collect.c
 * @brief Unit test for shmem_collect().
 */

#include "shmemvv.h"

#define TEST_C11_SHMEM_COLLECT(TYPE) \
  ({                                                                           \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(sizeof(TYPE));                            \
    TYPE *dest = (TYPE *)shmem_malloc(npes * sizeof(TYPE));                    \
                                                                               \
    src[0] = mype;                                                             \
                                                                               \
    shmem_collect(SHMEM_TEAM_WORLD, dest, src, 1);                \
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

  result &= TEST_C11_SHMEM_COLLECT(float);
  result &= TEST_C11_SHMEM_COLLECT(double);
  result &= TEST_C11_SHMEM_COLLECT(long double);
  result &= TEST_C11_SHMEM_COLLECT(char);
  result &= TEST_C11_SHMEM_COLLECT(signed char);
  result &= TEST_C11_SHMEM_COLLECT(short);
  result &= TEST_C11_SHMEM_COLLECT(int);
  result &= TEST_C11_SHMEM_COLLECT(long);
  result &= TEST_C11_SHMEM_COLLECT(long long);
  result &= TEST_C11_SHMEM_COLLECT(unsigned char);
  result &= TEST_C11_SHMEM_COLLECT(unsigned short);
  result &= TEST_C11_SHMEM_COLLECT(unsigned int);
  result &= TEST_C11_SHMEM_COLLECT(unsigned long);
  result &= TEST_C11_SHMEM_COLLECT(unsigned long long);
  result &= TEST_C11_SHMEM_COLLECT(int8_t);
  result &= TEST_C11_SHMEM_COLLECT(int16_t);
  result &= TEST_C11_SHMEM_COLLECT(int32_t);
  result &= TEST_C11_SHMEM_COLLECT(int64_t);
  result &= TEST_C11_SHMEM_COLLECT(uint8_t);
  result &= TEST_C11_SHMEM_COLLECT(uint16_t);
  result &= TEST_C11_SHMEM_COLLECT(uint32_t);
  result &= TEST_C11_SHMEM_COLLECT(uint64_t);
  result &= TEST_C11_SHMEM_COLLECT(size_t);
  result &= TEST_C11_SHMEM_COLLECT(ptrdiff_t);

  shmem_barrier_all();

  if (result) {
    if (shmem_my_pe() == 0) {
      display_test_result("C11 shmem_collect()", result, false);
    }
  } else {
    if (shmem_my_pe() == 0) {
      display_test_result("C11 shmem_collect()", result, false);
      rc = EXIT_FAILURE;
    }
  }

  shmem_finalize();
  return rc;
}
