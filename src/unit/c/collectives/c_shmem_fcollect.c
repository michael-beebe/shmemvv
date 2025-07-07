/**
 * @file c_shmem_fcollect.c
 * @brief Unit test for shmem_fcollect().
 */

#include "log.h"
#include "shmemvv.h"

#define TEST_C_SHMEM_FCOLLECT(TYPE, TYPENAME)                                  \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_fcollect");                               \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_calloc(1, sizeof(TYPE));                         \
    TYPE *dest = (TYPE *)shmem_calloc(npes, sizeof(TYPE));                     \
    log_info("shmem_calloc'd %d bytes @ &src = %p, %d bytes @ &dest = %p",     \
             sizeof(TYPE), (void *)src, npes * sizeof(TYPE), (void *)dest);    \
                                                                               \
    if (!src || !dest) {                                                       \
      log_fail("Failed to allocate symmetric memory");                         \
      if (src) shmem_free(src);                                                \
      if (dest) shmem_free(dest);                                              \
      return false;                                                            \
    }                                                                          \
                                                                               \
    src[0] = (TYPE)mype;                                                       \
    log_info("set %p (src[0]) to %d", (void *)src, mype);                      \
                                                                               \
    shmem_barrier_all(); /* Ensure all PEs are ready */                        \
                                                                               \
    log_info("executing shmem_fcollect: dest = %p, src = %p", (void *)dest,    \
             (void *)src);                                                     \
    shmem_##TYPENAME##_fcollect(SHMEM_TEAM_WORLD, dest, src, 1);               \
                                                                               \
    shmem_barrier_all(); /* Ensure all PEs complete fcollect before validation \
                          */                                                   \
                                                                               \
    log_info("validating result...");                                          \
    bool success = true;                                                       \
    for (int i = 0; i < npes; ++i) {                                           \
      TYPE expected = (TYPE)i;                                                 \
      if (dest[i] != expected) {                                               \
        log_info("index %d of dest (%p) failed. expected %d, got %d", i,       \
                 &dest[i], (int)expected, (int)dest[i]);                       \
        success = false;                                                       \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
                                                                               \
    if (success)                                                               \
      log_info("shmem_" #TYPENAME "_fcollect produced expected result.");      \
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

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_fcollect", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}

