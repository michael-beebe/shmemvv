/**
 * @file c_shmem_p.c
 * @brief Unit test for the shmem_p() routine.
 */

#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "shmemvv.h"

#define TEST_C_SHMEM_P(TYPE, TYPENAME)                                         \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_p()");                                    \
    bool success = true;                                                       \
    static TYPE src, dest;                                                     \
    log_info("Allocated static variables: src at %p, dest at %p",              \
             (void *)&src, (void *)&dest);                                     \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    log_info("Running on PE %d of %d total PEs", mype, npes);                  \
                                                                               \
    src = mype;                                                                \
    log_info("PE %d: Initialized src with value %d", mype, mype);              \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 0) {                                                           \
      log_info("PE 0: Starting put operation to PE 1");                        \
      log_info("PE 0: dest=%p, src=%d", (void *)&dest, src);                   \
      shmem_##TYPENAME##_p(&dest, src, 1);                                     \
      log_info("PE 0: Completed put operation");                               \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Beginning validation of received data");                 \
      if (dest != 0) {                                                         \
        log_fail("PE 1: Validation failed - dest = %d, expected 0",            \
                 (int)dest);                                                   \
        success = false;                                                       \
      } else {                                                                 \
        log_info("PE 1: Validation successful - dest matches expected value"); \
      }                                                                        \
    } else {                                                                   \
      log_info("PE 0: Waiting for PE 1 to complete validation");               \
    }                                                                          \
                                                                               \
    success;                                                                   \
  })

#define TEST_C_CTX_SHMEM_P(TYPE, TYPENAME)                                      \
  ({                                                                           \
    log_routine("shmem_ctx_" #TYPENAME "_p()");                                \
    bool success = true;                                                       \
    static TYPE src, dest;                                                     \
    log_info("Allocated static variables: src at %p, dest at %p",              \
             (void *)&src, (void *)&dest);                                     \
    int mype = shmem_my_pe();                                                  \
    int npes = shmem_n_pes();                                                  \
    log_info("Running on PE %d of %d total PEs", mype, npes);                  \
                                                                               \
    /* Create a context for the operation */                                   \
    shmem_ctx_t ctx;                                                           \
    int ctx_create_status = shmem_ctx_create(0, &ctx);                         \
                                                                               \
    if (ctx_create_status != 0) {                                              \
      log_fail("Failed to create context");                                    \
      return false;                                                            \
    }                                                                          \
    log_info("Successfully created context");                                  \
                                                                               \
    src = mype + 20; /* Different value for context test */                    \
    log_info("PE %d: Initialized src with value %d", mype, (int)(mype + 20));  \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 0) {                                                           \
      log_info("PE 0: Starting context-based put operation to PE 1");          \
      log_info("PE 0: dest=%p, src=%d", (void *)&dest, (int)src);              \
      shmem_ctx_##TYPENAME##_p(ctx, &dest, src, 1);                            \
      log_info("PE 0: Completed context-based put operation");                 \
    }                                                                          \
                                                                               \
    shmem_barrier_all();                                                       \
    log_info("Completed barrier synchronization");                             \
                                                                               \
    if (mype == 1) {                                                           \
      log_info("PE 1: Beginning validation of received data");                 \
      if (dest != 20) { /* PE 0's value is 0+20=20 */                          \
        log_fail("PE 1: Validation failed - dest = %d, expected 20",           \
                 (int)dest);                                                   \
        success = false;                                                       \
      } else {                                                                 \
        log_info("PE 1: Validation successful - dest matches expected value"); \
      }                                                                        \
    } else {                                                                   \
      log_info("PE 0: Waiting for PE 1 to complete validation");               \
    }                                                                          \
                                                                               \
    /* Destroy the context */                                                  \
    shmem_ctx_destroy(ctx);                                                    \
    log_info("Context destroyed");                                             \
                                                                               \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  if (!(shmem_n_pes() <= 2)) {
    log_warn("Not enough PEs to run test (requires 2 PEs, have %d PEs)",
             shmem_n_pes());
    if (shmem_my_pe() == 0) {
      display_not_enough_pes("RMA");
    }
    shmem_finalize();
    return EXIT_SUCCESS;
  }

  int result = true;
  int rc = EXIT_SUCCESS;

  /* Test standard shmem_p variants */
  result &= TEST_C_SHMEM_P(float, float);
  result &= TEST_C_SHMEM_P(double, double);
  result &= TEST_C_SHMEM_P(long double, longdouble);
  result &= TEST_C_SHMEM_P(char, char);
  result &= TEST_C_SHMEM_P(signed char, schar);
  result &= TEST_C_SHMEM_P(short, short);
  result &= TEST_C_SHMEM_P(int, int);
  result &= TEST_C_SHMEM_P(long, long);
  result &= TEST_C_SHMEM_P(long long, longlong);
  result &= TEST_C_SHMEM_P(unsigned char, uchar);
  result &= TEST_C_SHMEM_P(unsigned short, ushort);
  result &= TEST_C_SHMEM_P(unsigned int, uint);
  result &= TEST_C_SHMEM_P(unsigned long, ulong);
  result &= TEST_C_SHMEM_P(unsigned long long, ulonglong);
  result &= TEST_C_SHMEM_P(int8_t, int8);
  result &= TEST_C_SHMEM_P(int16_t, int16);
  result &= TEST_C_SHMEM_P(int32_t, int32);
  result &= TEST_C_SHMEM_P(int64_t, int64);
  result &= TEST_C_SHMEM_P(uint8_t, uint8);
  result &= TEST_C_SHMEM_P(uint16_t, uint16);
  result &= TEST_C_SHMEM_P(uint32_t, uint32);
  result &= TEST_C_SHMEM_P(uint64_t, uint64);
  result &= TEST_C_SHMEM_P(size_t, size);
  result &= TEST_C_SHMEM_P(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (!result) {
    rc = EXIT_FAILURE;
  }

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_p", result, false);
  }

  /* Test context-specific shmem_p variants */
  int result_ctx = true;

  result_ctx &= TEST_C_CTX_SHMEM_P(float, float);
  result_ctx &= TEST_C_CTX_SHMEM_P(double, double);
  result_ctx &= TEST_C_CTX_SHMEM_P(long double, longdouble);
  result_ctx &= TEST_C_CTX_SHMEM_P(char, char);
  result_ctx &= TEST_C_CTX_SHMEM_P(signed char, schar);
  result_ctx &= TEST_C_CTX_SHMEM_P(short, short);
  result_ctx &= TEST_C_CTX_SHMEM_P(int, int);
  result_ctx &= TEST_C_CTX_SHMEM_P(long, long);
  result_ctx &= TEST_C_CTX_SHMEM_P(long long, longlong);
  result_ctx &= TEST_C_CTX_SHMEM_P(unsigned char, uchar);
  result_ctx &= TEST_C_CTX_SHMEM_P(unsigned short, ushort);
  result_ctx &= TEST_C_CTX_SHMEM_P(unsigned int, uint);
  result_ctx &= TEST_C_CTX_SHMEM_P(unsigned long, ulong);
  result_ctx &= TEST_C_CTX_SHMEM_P(unsigned long long, ulonglong);
  result_ctx &= TEST_C_CTX_SHMEM_P(int8_t, int8);
  result_ctx &= TEST_C_CTX_SHMEM_P(int16_t, int16);
  result_ctx &= TEST_C_CTX_SHMEM_P(int32_t, int32);
  result_ctx &= TEST_C_CTX_SHMEM_P(int64_t, int64);
  result_ctx &= TEST_C_CTX_SHMEM_P(uint8_t, uint8);
  result_ctx &= TEST_C_CTX_SHMEM_P(uint16_t, uint16);
  result_ctx &= TEST_C_CTX_SHMEM_P(uint32_t, uint32);
  result_ctx &= TEST_C_CTX_SHMEM_P(uint64_t, uint64);
  result_ctx &= TEST_C_CTX_SHMEM_P(size_t, size);
  result_ctx &= TEST_C_CTX_SHMEM_P(ptrdiff_t, ptrdiff);

  shmem_barrier_all();

  if (!result_ctx) {
    rc = EXIT_FAILURE;
  }

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_ctx_p", result_ctx, false);
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
