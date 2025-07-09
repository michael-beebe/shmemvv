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
    int root_pe = 1;                                                           \
                                                                               \
    /* Skip test if we don't have enough PEs */                                \
    if (npes < 2) {                                                            \
      log_info("Skipping broadcast test - requires at least 2 PEs, got %d",    \
               npes);                                                          \
      return true;                                                             \
    }                                                                          \
                                                                               \
    /* Use separate memory allocations to avoid overlap issues */              \
    TYPE *dest = (TYPE *)shmem_malloc(4 * sizeof(TYPE));                       \
    TYPE *src = (TYPE *)shmem_malloc(4 * sizeof(TYPE));                        \
    log_info("shmem_malloc'd dest = %p, src = %p", (void *)dest, (void *)src); \
                                                                               \
    /* Initialize destination to zeros */                                      \
    for (int i = 0; i < 4; ++i) {                                              \
      dest[i] = (TYPE)0;                                                       \
    }                                                                          \
                                                                               \
    /* Initialize source array: only root PE has data, others zero */          \
    for (int i = 0; i < 4; ++i) {                                              \
      if (mype == root_pe) {                                                   \
        src[i] = (TYPE)(i + 1);                                                \
      } else {                                                                 \
        src[i] = (TYPE)0;                                                      \
      }                                                                        \
    }                                                                          \
    if (mype == root_pe) {                                                     \
      log_info("set src[0..4] to [1,2,3,4] on root PE %d", root_pe);           \
    } else {                                                                   \
      log_info("set src[0..4] to [0,0,0,0] on non-root PE %d", mype);          \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    log_info("executing shmem_broadcast: dest = %p, src = %p, root = %d",      \
             (void *)dest, (void *)src, root_pe);                              \
    shmem_##TYPENAME##_broadcast(SHMEM_TEAM_WORLD, dest, src, 4, root_pe);     \
                                                                               \
    shmem_barrier_all();                                                       \
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
          log_info("Dest element bytes: %02x %02x %02x %02x", ptr[0], ptr[1],  \
                   ptr[2], ptr[3]);                                            \
          /* Also check what's in the source array */                          \
          unsigned char *src_ptr = (unsigned char *)&src[i];                   \
          log_info("Source element bytes: %02x %02x %02x %02x", src_ptr[0],    \
                   src_ptr[1], src_ptr[2], src_ptr[3]);                        \
        }                                                                      \
        success = false;                                                       \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
                                                                               \
    if (success)                                                               \
      log_info("shmem_broadcast on " #TYPE " produced expected result.");      \
    else                                                                       \
      log_fail(                                                                \
          "at least one value was unexpected in result of shmem_broadcast");   \
                                                                               \
    shmem_free(dest);                                                          \
    shmem_free(src);                                                           \
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
  shmem_barrier_all();
  result &= TEST_C_SHMEM_BROADCAST(double, double);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_BROADCAST(long double, longdouble);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_BROADCAST(char, char);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_BROADCAST(signed char, schar);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_BROADCAST(short, short);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_BROADCAST(int, int);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_BROADCAST(long, long);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_BROADCAST(long long, longlong);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_BROADCAST(unsigned char, uchar);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_BROADCAST(unsigned short, ushort);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_BROADCAST(unsigned int, uint);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_BROADCAST(unsigned long, ulong);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_BROADCAST(unsigned long long, ulonglong);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_BROADCAST(int8_t, int8);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_BROADCAST(int16_t, int16);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_BROADCAST(int32_t, int32);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_BROADCAST(int64_t, int64);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_BROADCAST(uint8_t, uint8);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_BROADCAST(uint16_t, uint16);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_BROADCAST(uint32_t, uint32);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_BROADCAST(uint64_t, uint64);
  shmem_barrier_all();
  result &= TEST_C_SHMEM_BROADCAST(size_t, size);
  shmem_barrier_all();
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
