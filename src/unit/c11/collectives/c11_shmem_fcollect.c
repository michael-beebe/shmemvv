/**
 * @file c11_shmem_fcollect.c
 * @brief Unit test for shmem_fcollect().
 */

#include "log.h"
#include "shmemvv.h"

#define TEST_C11_SHMEM_FCOLLECT(TYPE)                                          \
  ({                                                                           \
    log_routine("shmem_fcollect(" #TYPE ")");                                  \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(sizeof(TYPE));                            \
    TYPE *dest = (TYPE *)shmem_malloc(npes * sizeof(TYPE));                    \
    log_info("shmem_malloc'd %d bytes @ &src = %p, %d bytes @ &dest = %p",     \
             sizeof(TYPE), (void *)src, npes * sizeof(TYPE), (void *)dest);    \
                                                                               \
    /* Initialize dest array to detect unwritten elements */                   \
    for (int i = 0; i < npes; ++i) {                                           \
      dest[i] = (TYPE) - 1;                                                    \
    }                                                                          \
                                                                               \
    src[0] = mype;                                                             \
    log_info("set %p (src[0]) to %d", (void *)src, mype);                      \
                                                                               \
    shmem_barrier_all(); /* Ensure all PEs are ready */                        \
                                                                               \
    log_info("executing shmem_fcollect: dest = %p, src = %p", (void *)dest,    \
             (void *)src);                                                     \
    shmem_fcollect(SHMEM_TEAM_WORLD, dest, src, 1);                            \
                                                                               \
    shmem_barrier_all(); /* Ensure all PEs complete fcollect before validation \
                          */                                                   \
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
      log_info("shmem_fcollect on " #TYPE " produced expected result.");       \
    else                                                                       \
      log_fail(                                                                \
          "at least one value was unexpected in result of shmem_fcollect");    \
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

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_fcollect", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
