/**
 * @file c11_shmem_collect.c
 * @brief Unit test for shmem_collect().
 */

#include "log.h"
#include "shmemvv.h"

#define TEST_C11_SHMEM_COLLECT(TYPE)                                           \
  ({                                                                           \
    log_routine("shmem_collect(" #TYPE ")");                                   \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(sizeof(TYPE));                            \
    TYPE *dest = (TYPE *)shmem_malloc(npes * sizeof(TYPE));                    \
    log_info("shmem_malloc'd %d bytes @ &src = %p, %d bytes @ &dest = %p",     \
             sizeof(TYPE), (void *)src, npes * sizeof(TYPE), (void *)dest);    \
                                                                               \
    src[0] = mype;                                                             \
    log_info("set %p (src[0]) to %d", (void *)src, mype);                      \
                                                                               \
    log_info("executing shmem_collect: dest = %p, src = %p", (void *)dest,     \
             (void *)src);                                                     \
    shmem_collect(SHMEM_TEAM_WORLD, dest, src, 1);                             \
                                                                               \
    log_info("validating result...");                                          \
    bool success = true;                                                       \
    for (int i = 0; i < npes; ++i) {                                           \
      if (dest[i] != i) {                                                      \
        log_info("index %d of dest (%p) failed. expected %d, got %d", i,       \
                 &dest[i], i, (char)dest[i]);                                  \
        success = false;                                                       \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
                                                                               \
    if (success)                                                               \
      log_info("shmem_collect on " #TYPE " produced expected result.");        \
    else                                                                       \
      log_fail(                                                                \
          "at least one value was unexpected in result of shmem_collect");     \
    shmem_free(src);                                                           \
    shmem_free(dest);                                                          \
                                                                               \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

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

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_collect", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
