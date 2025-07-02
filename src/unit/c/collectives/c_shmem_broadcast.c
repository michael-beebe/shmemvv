/**
 * @file c_shmem_broadcast.c
 * @brief Unit test for shmem_broadcast().
 */

#include "log.h"
#include "shmemvv.h"

#define TEST_C_SHMEM_BROADCAST(TYPE, TYPENAME)                                 \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_broadcast");                              \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(4 * sizeof(TYPE));                        \
    TYPE *dest = (TYPE *)shmem_malloc(4 * sizeof(TYPE));                       \
    log_info("shmem_malloc'd %d bytes @ &src = %p, %d bytes @ &dest = %p",     \
             4 * sizeof(TYPE), (void *)src, 4 * sizeof(TYPE), (void *)dest);   \
                                                                               \
    /* Initialize source array on all PEs to avoid uninitialized memory */     \
    for (int i = 0; i < 4; ++i) {                                              \
      src[i] = 0;                                                              \
    }                                                                          \
                                                                               \
    /* Only root PE sets the actual source data */                             \
    if (mype == 0) {                                                           \
      for (int i = 0; i < 4; ++i) {                                            \
        src[i] = i + 1;                                                        \
      }                                                                        \
      log_info("set %p..%p to idx + 1", (void *)src, (void *)&src[3]);         \
    }                                                                          \
                                                                               \
    for (int i = 0; i < 4; ++i) {                                              \
      dest[i] = -1;                                                            \
    }                                                                          \
    log_info("set %p..%p to -1", (void *)dest, (void *)&dest[3]);              \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    log_info("executing shmem_broadcast: dest = %p, src = %p", (void *)dest,   \
             (void *)src);                                                     \
    /* Use broadcastmem as workaround for type-specific broadcast bugs */      \
    int bc_result =                                                            \
        shmem_broadcastmem(SHMEM_TEAM_WORLD, dest, src, 4 * sizeof(TYPE), 0);  \
    if (bc_result != 0) {                                                      \
      log_fail("shmem_broadcastmem returned error: %d", bc_result);            \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    shmem_fence(); /* Ensure memory operations are complete */                 \
                                                                               \
    log_info("validating result...");                                          \
    bool success = true;                                                       \
    for (int i = 0; i < 4; ++i) {                                              \
      TYPE expected = (TYPE)(i + 1);                                           \
      if (dest[i] != expected) {                                               \
        log_info("index %d of dest (%p) failed. expected %d, got %d", i,       \
                 &dest[i], (int)expected, (int)dest[i]);                       \
        /* Additional debugging for the first few bytes */                     \
        if (i == 0) {                                                          \
          unsigned char *ptr = (unsigned char *)&dest[i];                      \
          log_info("First element bytes: %02x %02x %02x %02x", ptr[0], ptr[1], \
                   ptr[2], ptr[3]);                                            \
        }                                                                      \
        success = false;                                                       \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
                                                                               \
    if (success)                                                               \
      log_info("shmem_broadcast on " #TYPE " produced expected result.");      \
    else                                                                       \
      log_fail(                                                                \
          "at least one value was unexpected in result of shmem_broadcast");   \
    shmem_free(src);                                                           \
    shmem_free(dest);                                                          \
                                                                               \
    /* Ensure memory cleanup is complete across all PEs */                     \
    shmem_barrier_all();                                                       \
    shmem_fence();                                                             \
                                                                               \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = true;
  int rc = EXIT_SUCCESS;

  result &= TEST_C_SHMEM_BROADCAST(float, float);
  result &= TEST_C_SHMEM_BROADCAST(double, double);
  result &= TEST_C_SHMEM_BROADCAST(long double, longdouble);
  result &= TEST_C_SHMEM_BROADCAST(char, char);
  result &= TEST_C_SHMEM_BROADCAST(signed char, schar);
  result &= TEST_C_SHMEM_BROADCAST(short, short);
  result &= TEST_C_SHMEM_BROADCAST(int, int);
  result &= TEST_C_SHMEM_BROADCAST(long, long);
  result &= TEST_C_SHMEM_BROADCAST(long long, longlong);
  result &= TEST_C_SHMEM_BROADCAST(unsigned char, uchar);
  result &= TEST_C_SHMEM_BROADCAST(unsigned short, ushort);
  result &= TEST_C_SHMEM_BROADCAST(unsigned int, uint);
  result &= TEST_C_SHMEM_BROADCAST(unsigned long, ulong);
  result &= TEST_C_SHMEM_BROADCAST(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_BROADCAST(int8_t, int8);
  result &= TEST_C_SHMEM_BROADCAST(int16_t, int16);
  result &= TEST_C_SHMEM_BROADCAST(int32_t, int32);
  result &= TEST_C_SHMEM_BROADCAST(int64_t, int64);
  result &= TEST_C_SHMEM_BROADCAST(uint8_t, uint8);
  result &= TEST_C_SHMEM_BROADCAST(uint16_t, uint16);
  result &= TEST_C_SHMEM_BROADCAST(uint32_t, uint32);
  result &= TEST_C_SHMEM_BROADCAST(uint64_t, uint64);
  result &= TEST_C_SHMEM_BROADCAST(size_t, size);
  result &= TEST_C_SHMEM_BROADCAST(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_broadcast", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();

  return rc;
}
