/**
 * @file c11_shmem_reduce.c
 * @brief Unified unit test for all shmem reduction operations (C11 version).
 *
 * This test combines max, min, sum, prod, and, or, xor reduction operations
 * into a single file with separate test functions for each operation.
 */

#include "log.h"
#include "shmemvv.h"
#include <math.h>
#include <complex.h>

const double FLOATING_POINT_TOLERANCE = 1e-6;

#define is_floating_point(X)                                                   \
  _Generic((X), float: true, double: true, long double: true, default: false)

#define is_complex(X)                                                          \
  _Generic((X), float _Complex: true, double _Complex: true, default: false)

#define TEST_C11_SHMEM_MAX_REDUCE(TYPE, TYPENAME)                              \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_max_reduce");                             \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
    const bool is_fp = is_floating_point((TYPE)0);                             \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(sizeof(TYPE));                            \
    TYPE *dest = (TYPE *)shmem_malloc(sizeof(TYPE));                           \
    log_info("shmem_malloc'd %d bytes @ &src = %p, %d bytes @ &dest = %p",     \
             sizeof(TYPE), (void *)src, sizeof(TYPE), (void *)dest);           \
                                                                               \
    /* Use safe value range to avoid overflow for 8-bit types */               \
    TYPE src_val;                                                              \
    if (sizeof(TYPE) == 1) {                                                   \
      /* For 8-bit types, use PE % 100 to stay within safe range */            \
      src_val = (TYPE)(mype % 100);                                            \
    } else {                                                                   \
      src_val = (TYPE)mype;                                                    \
    }                                                                          \
    *src = src_val;                                                            \
    if (is_complex((TYPE)0)) {                                                 \
      log_info("set %p (src) to %g+%gi", (void *)src, (double)creall(*src),    \
               (double)cimagl(*src));                                          \
    } else {                                                                   \
      log_info("set %p (src) to %g", (void *)src, (double)*src);               \
    }                                                                          \
                                                                               \
    log_info("executing shmem_max_reduce: dest = %p, src = %p", (void *)dest,  \
             (void *)src);                                                     \
    shmem_max_reduce(SHMEM_TEAM_WORLD, dest, src, 1);                          \
                                                                               \
    log_info("validating result...");                                          \
    TYPE expected;                                                             \
    if (sizeof(TYPE) == 1) {                                                   \
      /* For 8-bit types, max of (PE % 100) values is min(99, npes-1) */       \
      expected = (TYPE)(npes >= 100 ? 99 : npes - 1);                          \
    } else {                                                                   \
      expected = (TYPE)(npes - 1);                                             \
    }                                                                          \
    bool success;                                                              \
    if (is_complex((TYPE)0)) {                                                 \
      /* Complex validation: check real and imaginary parts separately */      \
      long double real_diff = fabsl(creall(*dest) - creall(expected));         \
      long double imag_diff = fabsl(cimagl(*dest) - cimagl(expected));         \
      success = (real_diff <= FLOATING_POINT_TOLERANCE) &&                     \
                (imag_diff <= FLOATING_POINT_TOLERANCE);                       \
    } else if (is_fp) {                                                        \
      success = (fabsl((long double)(*dest - expected)) <=                     \
                 FLOATING_POINT_TOLERANCE);                                    \
    } else {                                                                   \
      success = (*dest == expected);                                           \
    }                                                                          \
                                                                               \
    if (success)                                                               \
      log_info("shmem_" #TYPENAME "_max_reduce produced expected result.");    \
    else {                                                                     \
      log_fail("shmem_" #TYPENAME "_max_reduce failed: got %g, expected %g",   \
               (double)*dest, (double)expected);                               \
    }                                                                          \
                                                                               \
    shmem_free(src);                                                           \
    shmem_free(dest);                                                          \
                                                                               \
    success;                                                                   \
  })

#define TEST_C11_SHMEM_MIN_REDUCE(TYPE, TYPENAME)                              \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_min_reduce");                             \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
    const bool is_fp = is_floating_point((TYPE)0);                             \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(sizeof(TYPE));                            \
    TYPE *dest = (TYPE *)shmem_malloc(sizeof(TYPE));                           \
    log_info("shmem_malloc'd %d bytes @ &src = %p, %d bytes @ &dest = %p",     \
             sizeof(TYPE), (void *)src, sizeof(TYPE), (void *)dest);           \
                                                                               \
    /* Use safe value range to avoid overflow for 8-bit types */               \
    TYPE src_val;                                                              \
    if (sizeof(TYPE) == 1) {                                                   \
      /* For 8-bit types, use PE % 100 to stay within safe range */            \
      src_val = (TYPE)(mype % 100);                                            \
    } else {                                                                   \
      src_val = (TYPE)mype;                                                    \
    }                                                                          \
    *src = src_val;                                                            \
    if (is_complex((TYPE)0)) {                                                 \
      log_info("set %p (src) to %g+%gi", (void *)src, (double)creall(*src),    \
               (double)cimagl(*src));                                          \
    } else {                                                                   \
      log_info("set %p (src) to %g", (void *)src, (double)*src);               \
    }                                                                          \
                                                                               \
    log_info("executing shmem_min_reduce: dest = %p, src = %p", (void *)dest,  \
             (void *)src);                                                     \
    shmem_min_reduce(SHMEM_TEAM_WORLD, dest, src, 1);                          \
                                                                               \
    log_info("validating result...");                                          \
    TYPE expected = (TYPE)0; /* Minimum will always be 0 with safe range */    \
    bool success;                                                              \
    if (is_complex((TYPE)0)) {                                                 \
      /* Complex validation: check real and imaginary parts separately */      \
      long double real_diff = fabsl(creall(*dest) - creall(expected));         \
      long double imag_diff = fabsl(cimagl(*dest) - cimagl(expected));         \
      success = (real_diff <= FLOATING_POINT_TOLERANCE) &&                     \
                (imag_diff <= FLOATING_POINT_TOLERANCE);                       \
    } else if (is_fp) {                                                        \
      success = (fabsl((long double)(*dest - expected)) <=                     \
                 FLOATING_POINT_TOLERANCE);                                    \
    } else {                                                                   \
      success = (*dest == expected);                                           \
    }                                                                          \
                                                                               \
    if (success)                                                               \
      log_info("shmem_" #TYPENAME "_min_reduce produced expected result.");    \
    else                                                                       \
      log_fail("unexpected result from shmem_min_reduce");                     \
                                                                               \
    shmem_free(src);                                                           \
    shmem_free(dest);                                                          \
                                                                               \
    success;                                                                   \
  })

#define TEST_C11_SHMEM_SUM_REDUCE(TYPE, TYPENAME)                              \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_sum_reduce");                             \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
    const bool is_fp = is_floating_point((TYPE)0);                             \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(sizeof(TYPE));                            \
    TYPE *dest = (TYPE *)shmem_malloc(sizeof(TYPE));                           \
    log_info("shmem_malloc'd %d bytes @ &src = %p, %d bytes @ &dest = %p",     \
             sizeof(TYPE), (void *)src, sizeof(TYPE), (void *)dest);           \
                                                                               \
    *src = (TYPE)mype;                                                         \
    if (is_complex((TYPE)0)) {                                                 \
      log_info("set %p (src) to %g+%gi", (void *)src, (double)creall(*src),    \
               (double)cimagl(*src));                                          \
    } else {                                                                   \
      log_info("set %p (src) to %g", (void *)src, (double)*src);               \
    }                                                                          \
                                                                               \
    log_info("executing shmem_sum_reduce: dest = %p, src = %p", (void *)dest,  \
             (void *)src);                                                     \
    shmem_sum_reduce(SHMEM_TEAM_WORLD, dest, src, 1);                          \
                                                                               \
    log_info("validating result...");                                          \
    TYPE expected = (TYPE)(npes * (npes - 1) / 2);                             \
    bool success;                                                              \
    if (is_complex((TYPE)0)) {                                                 \
      /* Complex validation: check real and imaginary parts separately */      \
      long double real_diff = fabsl(creall(*dest) - creall(expected));         \
      long double imag_diff = fabsl(cimagl(*dest) - cimagl(expected));         \
      success = (real_diff <= FLOATING_POINT_TOLERANCE) &&                     \
                (imag_diff <= FLOATING_POINT_TOLERANCE);                       \
    } else if (is_fp) {                                                        \
      success = (fabsl((long double)(*dest - expected)) <=                     \
                 FLOATING_POINT_TOLERANCE);                                    \
    } else {                                                                   \
      success = (*dest == expected);                                           \
    }                                                                          \
                                                                               \
    if (success)                                                               \
      log_info("shmem_" #TYPENAME "_sum_reduce produced expected result.");    \
    else                                                                       \
      log_fail("unexpected result from shmem_sum_reduce");                     \
                                                                               \
    shmem_free(src);                                                           \
    shmem_free(dest);                                                          \
                                                                               \
    success;                                                                   \
  })

#define TEST_C11_SHMEM_PROD_REDUCE(TYPE, TYPENAME)                             \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_prod_reduce");                            \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
    const bool is_fp = is_floating_point((TYPE)0);                             \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(sizeof(TYPE));                            \
    TYPE *dest = (TYPE *)shmem_malloc(sizeof(TYPE));                           \
    log_info("shmem_malloc'd %d bytes @ &src = %p, %d bytes @ &dest = %p",     \
             sizeof(TYPE), (void *)src, sizeof(TYPE), (void *)dest);           \
                                                                               \
    /* Use smaller values for floating-point types to avoid precision issues   \
     */                                                                        \
    TYPE src_val;                                                              \
    if (is_complex((TYPE)0) && npes > 50) {                                    \
      /* Use 1+0i for large PE counts to avoid precision issues */             \
      src_val = (TYPE)(1.0 + 0.0 * I);                                         \
    } else if (is_complex((TYPE)0)) {                                          \
      /* Use 1.1+0i for smaller PE counts */                                   \
      src_val = (TYPE)(1.1 + 0.0 * I);                                         \
    } else if (is_fp && npes > 50) {                                           \
      /* Use 1 for large PE counts to avoid precision issues */                \
      src_val = (TYPE)1.0;                                                     \
    } else if (is_fp) {                                                        \
      /* Use 1.1 for smaller PE counts */                                      \
      src_val = (TYPE)1.1;                                                     \
    } else if (npes > 20) {                                                    \
      /* Use 1 for large PE counts to avoid integer overflow */                \
      src_val = (TYPE)1;                                                       \
    } else {                                                                   \
      src_val = (TYPE)2; /* Use 2 for all PEs to avoid overflow */             \
    }                                                                          \
    *src = src_val;                                                            \
    if (is_complex((TYPE)0)) {                                                 \
      log_info("set %p (src) to %g+%gi", (void *)src, (double)creall(*src),    \
               (double)cimagl(*src));                                          \
    } else {                                                                   \
      log_info("set %p (src) to %g", (void *)src, (double)*src);               \
    }                                                                          \
                                                                               \
    log_info("executing shmem_prod_reduce: dest = %p, src = %p", (void *)dest, \
             (void *)src);                                                     \
    shmem_prod_reduce(SHMEM_TEAM_WORLD, dest, src, 1);                         \
                                                                               \
    log_info("validating result...");                                          \
    TYPE expected;                                                             \
    if (is_complex((TYPE)0) && npes > 50) {                                    \
      /* For large PE counts: (1+0i)^npes = 1+0i */                            \
      expected = (TYPE)(1.0 + 0.0 * I);                                        \
    } else if (is_complex((TYPE)0)) {                                          \
      /* For smaller PE counts: (1.1+0i)^npes = 1.1^npes + 0i */               \
      long double real_part = powl(1.1L, (long double)npes);                   \
      expected = (TYPE)(real_part + 0.0 * I);                                  \
    } else if (is_fp && npes > 50) {                                           \
      /* For large PE counts: 1^npes = 1 */                                    \
      expected = (TYPE)1.0;                                                    \
    } else if (is_fp) {                                                        \
      /* For smaller PE counts: 1.1^npes */                                    \
      expected = (TYPE)powl(1.1L, (long double)npes);                          \
    } else {                                                                   \
      /* For integers: use 2^npes or 1^npes depending on PE count */           \
      if (npes > 20) {                                                         \
        /* For large PE counts: 1^npes = 1 */                                  \
        expected = (TYPE)1;                                                    \
      } else {                                                                 \
        /* For smaller PE counts: 2^npes */                                    \
        expected = (TYPE)1;                                                    \
        for (int i = 0; i < npes; i++) {                                       \
          expected *= (TYPE)2;                                                 \
        }                                                                      \
      }                                                                        \
    }                                                                          \
    bool success;                                                              \
    if (is_complex((TYPE)0)) {                                                 \
      /* Complex validation: check real and imaginary parts separately */      \
      long double real_diff = fabsl(creall(*dest) - creall(expected));         \
      long double imag_diff = fabsl(cimagl(*dest) - cimagl(expected));         \
      long double abs_expected = fabsl(creall(expected));                      \
      long double relative_tolerance = FLOATING_POINT_TOLERANCE;               \
      if (abs_expected > 1.0L) {                                               \
        relative_tolerance = abs_expected * FLOATING_POINT_TOLERANCE;          \
      }                                                                        \
      success = (real_diff <= relative_tolerance) &&                           \
                (imag_diff <= FLOATING_POINT_TOLERANCE);                       \
    } else if (is_fp) {                                                        \
      /* Use relative tolerance for floating-point values */                   \
      long double abs_diff = fabsl((long double)(*dest - expected));           \
      long double abs_expected = fabsl((long double)expected);                 \
      long double relative_tolerance = FLOATING_POINT_TOLERANCE;               \
      if (abs_expected > 1.0L) {                                               \
        relative_tolerance = abs_expected * FLOATING_POINT_TOLERANCE;          \
      }                                                                        \
      success = (abs_diff <= relative_tolerance);                              \
    } else {                                                                   \
      success = (*dest == expected);                                           \
    }                                                                          \
                                                                               \
    if (success)                                                               \
      log_info("shmem_" #TYPENAME "_prod_reduce produced expected result.");   \
    else {                                                                     \
      log_fail("shmem_" #TYPENAME "_prod_reduce failed: got %g, expected %g",  \
               (double)*dest, (double)expected);                               \
    }                                                                          \
                                                                               \
    shmem_free(src);                                                           \
    shmem_free(dest);                                                          \
                                                                               \
    success;                                                                   \
  })

#define TEST_C11_SHMEM_AND_REDUCE(TYPE, TYPENAME)                              \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_and_reduce");                             \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(sizeof(TYPE));                            \
    TYPE *dest = (TYPE *)shmem_malloc(sizeof(TYPE));                           \
    log_info("shmem_malloc'd %d bytes @ &src = %p, %d bytes @ &dest = %p",     \
             sizeof(TYPE), (void *)src, sizeof(TYPE), (void *)dest);           \
                                                                               \
    *src = (TYPE)1;                                                            \
    log_info("set %p (src) to %g", (void *)src, (double)*src);                 \
                                                                               \
    log_info("executing shmem_and_reduce: dest = %p, src = %p", (void *)dest,  \
             (void *)src);                                                     \
    shmem_and_reduce(SHMEM_TEAM_WORLD, dest, src, 1);                          \
                                                                               \
    log_info("validating result...");                                          \
    TYPE expected = (TYPE)1;                                                   \
    bool success = (*dest == expected);                                        \
                                                                               \
    if (success)                                                               \
      log_info("shmem_" #TYPENAME "_and_reduce produced expected result.");    \
    else                                                                       \
      log_fail("unexpected result from shmem_and_reduce");                     \
                                                                               \
    shmem_free(src);                                                           \
    shmem_free(dest);                                                          \
                                                                               \
    success;                                                                   \
  })

#define TEST_C11_SHMEM_OR_REDUCE(TYPE, TYPENAME)                               \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_or_reduce");                              \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(sizeof(TYPE));                            \
    TYPE *dest = (TYPE *)shmem_malloc(sizeof(TYPE));                           \
    log_info("shmem_malloc'd %d bytes @ &src = %p, %d bytes @ &dest = %p",     \
             sizeof(TYPE), (void *)src, sizeof(TYPE), (void *)dest);           \
                                                                               \
    *src = (TYPE)1;                                                            \
    log_info("set %p (src) to %g", (void *)src, (double)*src);                 \
                                                                               \
    log_info("executing shmem_or_reduce: dest = %p, src = %p", (void *)dest,   \
             (void *)src);                                                     \
    shmem_or_reduce(SHMEM_TEAM_WORLD, dest, src, 1);                           \
                                                                               \
    log_info("validating result...");                                          \
    TYPE expected = (TYPE)1;                                                   \
    bool success = (*dest == expected);                                        \
                                                                               \
    if (success)                                                               \
      log_info("shmem_" #TYPENAME "_or_reduce produced expected result.");     \
    else                                                                       \
      log_fail("unexpected result from shmem_or_reduce");                      \
                                                                               \
    shmem_free(src);                                                           \
    shmem_free(dest);                                                          \
                                                                               \
    success;                                                                   \
  })

#define TEST_C11_SHMEM_XOR_REDUCE(TYPE, TYPENAME)                              \
  ({                                                                           \
    log_routine("shmem_" #TYPENAME "_xor_reduce");                             \
    int npes = shmem_n_pes();                                                  \
    int mype = shmem_my_pe();                                                  \
                                                                               \
    TYPE *src = (TYPE *)shmem_malloc(sizeof(TYPE));                            \
    TYPE *dest = (TYPE *)shmem_malloc(sizeof(TYPE));                           \
    log_info("shmem_malloc'd %d bytes @ &src = %p, %d bytes @ &dest = %p",     \
             sizeof(TYPE), (void *)src, sizeof(TYPE), (void *)dest);           \
                                                                               \
    *src = (TYPE)1;                                                            \
    log_info("set %p (src) to %g", (void *)src, (double)*src);                 \
                                                                               \
    log_info("executing shmem_xor_reduce: dest = %p, src = %p", (void *)dest,  \
             (void *)src);                                                     \
    shmem_xor_reduce(SHMEM_TEAM_WORLD, dest, src, 1);                          \
                                                                               \
    log_info("validating result...");                                          \
    TYPE expected = (TYPE)(npes % 2 ? 1 : 0);                                  \
    bool success = (*dest == expected);                                        \
                                                                               \
    if (success)                                                               \
      log_info("shmem_" #TYPENAME "_xor_reduce produced expected result.");    \
    else                                                                       \
      log_fail("unexpected result from shmem_xor_reduce");                     \
                                                                               \
    shmem_free(src);                                                           \
    shmem_free(dest);                                                          \
                                                                               \
    success;                                                                   \
  })

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  const int npes = shmem_n_pes();
  const int mype = shmem_my_pe();

  int rc = EXIT_SUCCESS;

  /* Test MAX reduction - SHMEM_REDUCE_MINMAX_TYPE_TABLE */
  bool result_max = true;
  result_max &= TEST_C11_SHMEM_MAX_REDUCE(char, char);
  result_max &= TEST_C11_SHMEM_MAX_REDUCE(signed char, schar);
  result_max &= TEST_C11_SHMEM_MAX_REDUCE(short, short);
  result_max &= TEST_C11_SHMEM_MAX_REDUCE(int, int);
  result_max &= TEST_C11_SHMEM_MAX_REDUCE(long, long);
  result_max &= TEST_C11_SHMEM_MAX_REDUCE(long long, longlong);
  result_max &= TEST_C11_SHMEM_MAX_REDUCE(ptrdiff_t, ptrdiff);
  result_max &= TEST_C11_SHMEM_MAX_REDUCE(unsigned char, uchar);
  result_max &= TEST_C11_SHMEM_MAX_REDUCE(unsigned short, ushort);
  result_max &= TEST_C11_SHMEM_MAX_REDUCE(unsigned int, uint);
  result_max &= TEST_C11_SHMEM_MAX_REDUCE(unsigned long, ulong);
  result_max &= TEST_C11_SHMEM_MAX_REDUCE(unsigned long long, ulonglong);
  result_max &= TEST_C11_SHMEM_MAX_REDUCE(int8_t, int8);
  result_max &= TEST_C11_SHMEM_MAX_REDUCE(int16_t, int16);
  result_max &= TEST_C11_SHMEM_MAX_REDUCE(int32_t, int32);
  result_max &= TEST_C11_SHMEM_MAX_REDUCE(int64_t, int64);
  result_max &= TEST_C11_SHMEM_MAX_REDUCE(uint8_t, uint8);
  result_max &= TEST_C11_SHMEM_MAX_REDUCE(uint16_t, uint16);
  result_max &= TEST_C11_SHMEM_MAX_REDUCE(uint32_t, uint32);
  result_max &= TEST_C11_SHMEM_MAX_REDUCE(uint64_t, uint64);
  result_max &= TEST_C11_SHMEM_MAX_REDUCE(size_t, size);
  result_max &= TEST_C11_SHMEM_MAX_REDUCE(float, float);
  result_max &= TEST_C11_SHMEM_MAX_REDUCE(double, double);
  result_max &= TEST_C11_SHMEM_MAX_REDUCE(long double, longdouble);

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_max_reduce", result_max, false);
  }

  if (!result_max) {
    rc = EXIT_FAILURE;
  }

  /* Test MIN reduction - SHMEM_REDUCE_MINMAX_TYPE_TABLE */
  bool result_min = true;
  result_min &= TEST_C11_SHMEM_MIN_REDUCE(char, char);
  result_min &= TEST_C11_SHMEM_MIN_REDUCE(signed char, schar);
  result_min &= TEST_C11_SHMEM_MIN_REDUCE(short, short);
  result_min &= TEST_C11_SHMEM_MIN_REDUCE(int, int);
  result_min &= TEST_C11_SHMEM_MIN_REDUCE(long, long);
  result_min &= TEST_C11_SHMEM_MIN_REDUCE(long long, longlong);
  result_min &= TEST_C11_SHMEM_MIN_REDUCE(ptrdiff_t, ptrdiff);
  result_min &= TEST_C11_SHMEM_MIN_REDUCE(unsigned char, uchar);
  result_min &= TEST_C11_SHMEM_MIN_REDUCE(unsigned short, ushort);
  result_min &= TEST_C11_SHMEM_MIN_REDUCE(unsigned int, uint);
  result_min &= TEST_C11_SHMEM_MIN_REDUCE(unsigned long, ulong);
  result_min &= TEST_C11_SHMEM_MIN_REDUCE(unsigned long long, ulonglong);
  result_min &= TEST_C11_SHMEM_MIN_REDUCE(int8_t, int8);
  result_min &= TEST_C11_SHMEM_MIN_REDUCE(int16_t, int16);
  result_min &= TEST_C11_SHMEM_MIN_REDUCE(int32_t, int32);
  result_min &= TEST_C11_SHMEM_MIN_REDUCE(int64_t, int64);
  result_min &= TEST_C11_SHMEM_MIN_REDUCE(uint8_t, uint8);
  result_min &= TEST_C11_SHMEM_MIN_REDUCE(uint16_t, uint16);
  result_min &= TEST_C11_SHMEM_MIN_REDUCE(uint32_t, uint32);
  result_min &= TEST_C11_SHMEM_MIN_REDUCE(uint64_t, uint64);
  result_min &= TEST_C11_SHMEM_MIN_REDUCE(size_t, size);
  result_min &= TEST_C11_SHMEM_MIN_REDUCE(float, float);
  result_min &= TEST_C11_SHMEM_MIN_REDUCE(double, double);
  result_min &= TEST_C11_SHMEM_MIN_REDUCE(long double, longdouble);

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_min_reduce", result_min, false);
  }

  if (!result_min) {
    rc = EXIT_FAILURE;
  }

  /* Test SUM reduction - SHMEM_REDUCE_ARITH_TYPE_TABLE */
  bool result_sum = true;
  result_sum &= TEST_C11_SHMEM_SUM_REDUCE(char, char);
  result_sum &= TEST_C11_SHMEM_SUM_REDUCE(signed char, schar);
  result_sum &= TEST_C11_SHMEM_SUM_REDUCE(short, short);
  result_sum &= TEST_C11_SHMEM_SUM_REDUCE(int, int);
  result_sum &= TEST_C11_SHMEM_SUM_REDUCE(long, long);
  result_sum &= TEST_C11_SHMEM_SUM_REDUCE(long long, longlong);
  result_sum &= TEST_C11_SHMEM_SUM_REDUCE(ptrdiff_t, ptrdiff);
  result_sum &= TEST_C11_SHMEM_SUM_REDUCE(unsigned char, uchar);
  result_sum &= TEST_C11_SHMEM_SUM_REDUCE(unsigned short, ushort);
  result_sum &= TEST_C11_SHMEM_SUM_REDUCE(unsigned int, uint);
  result_sum &= TEST_C11_SHMEM_SUM_REDUCE(unsigned long, ulong);
  result_sum &= TEST_C11_SHMEM_SUM_REDUCE(unsigned long long, ulonglong);
  result_sum &= TEST_C11_SHMEM_SUM_REDUCE(int8_t, int8);
  result_sum &= TEST_C11_SHMEM_SUM_REDUCE(int16_t, int16);
  result_sum &= TEST_C11_SHMEM_SUM_REDUCE(int32_t, int32);
  result_sum &= TEST_C11_SHMEM_SUM_REDUCE(int64_t, int64);
  result_sum &= TEST_C11_SHMEM_SUM_REDUCE(uint8_t, uint8);
  result_sum &= TEST_C11_SHMEM_SUM_REDUCE(uint16_t, uint16);
  result_sum &= TEST_C11_SHMEM_SUM_REDUCE(uint32_t, uint32);
  result_sum &= TEST_C11_SHMEM_SUM_REDUCE(uint64_t, uint64);
  result_sum &= TEST_C11_SHMEM_SUM_REDUCE(size_t, size);
  result_sum &= TEST_C11_SHMEM_SUM_REDUCE(float, float);
  result_sum &= TEST_C11_SHMEM_SUM_REDUCE(double, double);
  result_sum &= TEST_C11_SHMEM_SUM_REDUCE(long double, longdouble);
  result_sum &= TEST_C11_SHMEM_SUM_REDUCE(float _Complex, complexf);
  result_sum &= TEST_C11_SHMEM_SUM_REDUCE(double _Complex, complexd);

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_sum_reduce", result_sum, false);
  }

  if (!result_sum) {
    rc = EXIT_FAILURE;
  }

  /* Test PROD reduction - SHMEM_REDUCE_ARITH_TYPE_TABLE */
  bool result_prod = true;
  result_prod &= TEST_C11_SHMEM_PROD_REDUCE(char, char);
  result_prod &= TEST_C11_SHMEM_PROD_REDUCE(signed char, schar);
  result_prod &= TEST_C11_SHMEM_PROD_REDUCE(short, short);
  result_prod &= TEST_C11_SHMEM_PROD_REDUCE(int, int);
  result_prod &= TEST_C11_SHMEM_PROD_REDUCE(long, long);
  result_prod &= TEST_C11_SHMEM_PROD_REDUCE(long long, longlong);
  result_prod &= TEST_C11_SHMEM_PROD_REDUCE(ptrdiff_t, ptrdiff);
  result_prod &= TEST_C11_SHMEM_PROD_REDUCE(unsigned char, uchar);
  result_prod &= TEST_C11_SHMEM_PROD_REDUCE(unsigned short, ushort);
  result_prod &= TEST_C11_SHMEM_PROD_REDUCE(unsigned int, uint);
  result_prod &= TEST_C11_SHMEM_PROD_REDUCE(unsigned long, ulong);
  result_prod &= TEST_C11_SHMEM_PROD_REDUCE(unsigned long long, ulonglong);
  result_prod &= TEST_C11_SHMEM_PROD_REDUCE(int8_t, int8);
  result_prod &= TEST_C11_SHMEM_PROD_REDUCE(int16_t, int16);
  result_prod &= TEST_C11_SHMEM_PROD_REDUCE(int32_t, int32);
  result_prod &= TEST_C11_SHMEM_PROD_REDUCE(int64_t, int64);
  result_prod &= TEST_C11_SHMEM_PROD_REDUCE(uint8_t, uint8);
  result_prod &= TEST_C11_SHMEM_PROD_REDUCE(uint16_t, uint16);
  result_prod &= TEST_C11_SHMEM_PROD_REDUCE(uint32_t, uint32);
  result_prod &= TEST_C11_SHMEM_PROD_REDUCE(uint64_t, uint64);
  result_prod &= TEST_C11_SHMEM_PROD_REDUCE(size_t, size);
  result_prod &= TEST_C11_SHMEM_PROD_REDUCE(float, float);
  result_prod &= TEST_C11_SHMEM_PROD_REDUCE(double, double);
  result_prod &= TEST_C11_SHMEM_PROD_REDUCE(long double, longdouble);
  result_prod &= TEST_C11_SHMEM_PROD_REDUCE(float _Complex, complexf);
  result_prod &= TEST_C11_SHMEM_PROD_REDUCE(double _Complex, complexd);

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_prod_reduce", result_prod, false);
  }

  if (!result_prod) {
    rc = EXIT_FAILURE;
  }

  /* Test AND reduction - unsigned integer types only */
  bool result_and = true;
  result_and &= TEST_C11_SHMEM_AND_REDUCE(unsigned char, uchar);
  result_and &= TEST_C11_SHMEM_AND_REDUCE(unsigned short, ushort);
  result_and &= TEST_C11_SHMEM_AND_REDUCE(unsigned int, uint);
  result_and &= TEST_C11_SHMEM_AND_REDUCE(unsigned long, ulong);
  result_and &= TEST_C11_SHMEM_AND_REDUCE(unsigned long long, ulonglong);
  result_and &= TEST_C11_SHMEM_AND_REDUCE(int8_t, int8);
  result_and &= TEST_C11_SHMEM_AND_REDUCE(int16_t, int16);
  result_and &= TEST_C11_SHMEM_AND_REDUCE(int32_t, int32);
  result_and &= TEST_C11_SHMEM_AND_REDUCE(int64_t, int64);
  result_and &= TEST_C11_SHMEM_AND_REDUCE(uint8_t, uint8);
  result_and &= TEST_C11_SHMEM_AND_REDUCE(uint16_t, uint16);
  result_and &= TEST_C11_SHMEM_AND_REDUCE(uint32_t, uint32);
  result_and &= TEST_C11_SHMEM_AND_REDUCE(uint64_t, uint64);
  result_and &= TEST_C11_SHMEM_AND_REDUCE(size_t, size);

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_and_reduce", result_and, false);
  }

  if (!result_and) {
    rc = EXIT_FAILURE;
  }

  /* Test OR reduction - unsigned integer types only */
  bool result_or = true;
  result_or &= TEST_C11_SHMEM_OR_REDUCE(unsigned char, uchar);
  result_or &= TEST_C11_SHMEM_OR_REDUCE(unsigned short, ushort);
  result_or &= TEST_C11_SHMEM_OR_REDUCE(unsigned int, uint);
  result_or &= TEST_C11_SHMEM_OR_REDUCE(unsigned long, ulong);
  result_or &= TEST_C11_SHMEM_OR_REDUCE(unsigned long long, ulonglong);
  result_or &= TEST_C11_SHMEM_OR_REDUCE(int8_t, int8);
  result_or &= TEST_C11_SHMEM_OR_REDUCE(int16_t, int16);
  result_or &= TEST_C11_SHMEM_OR_REDUCE(int32_t, int32);
  result_or &= TEST_C11_SHMEM_OR_REDUCE(int64_t, int64);
  result_or &= TEST_C11_SHMEM_OR_REDUCE(uint8_t, uint8);
  result_or &= TEST_C11_SHMEM_OR_REDUCE(uint16_t, uint16);
  result_or &= TEST_C11_SHMEM_OR_REDUCE(uint32_t, uint32);
  result_or &= TEST_C11_SHMEM_OR_REDUCE(uint64_t, uint64);
  result_or &= TEST_C11_SHMEM_OR_REDUCE(size_t, size);

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_or_reduce", result_or, false);
  }

  if (!result_or) {
    rc = EXIT_FAILURE;
  }

  /* Test XOR reduction - unsigned integer types only */
  bool result_xor = true;
  result_xor &= TEST_C11_SHMEM_XOR_REDUCE(unsigned char, uchar);
  result_xor &= TEST_C11_SHMEM_XOR_REDUCE(unsigned short, ushort);
  result_xor &= TEST_C11_SHMEM_XOR_REDUCE(unsigned int, uint);
  result_xor &= TEST_C11_SHMEM_XOR_REDUCE(unsigned long, ulong);
  result_xor &= TEST_C11_SHMEM_XOR_REDUCE(unsigned long long, ulonglong);
  result_xor &= TEST_C11_SHMEM_XOR_REDUCE(int8_t, int8);
  result_xor &= TEST_C11_SHMEM_XOR_REDUCE(int16_t, int16);
  result_xor &= TEST_C11_SHMEM_XOR_REDUCE(int32_t, int32);
  result_xor &= TEST_C11_SHMEM_XOR_REDUCE(int64_t, int64);
  result_xor &= TEST_C11_SHMEM_XOR_REDUCE(uint8_t, uint8);
  result_xor &= TEST_C11_SHMEM_XOR_REDUCE(uint16_t, uint16);
  result_xor &= TEST_C11_SHMEM_XOR_REDUCE(uint32_t, uint32);
  result_xor &= TEST_C11_SHMEM_XOR_REDUCE(uint64_t, uint64);
  result_xor &= TEST_C11_SHMEM_XOR_REDUCE(size_t, size);

  if (shmem_my_pe() == 0) {
    display_test_result("C11 shmem_xor_reduce", result_xor, false);
  }

  if (!result_xor) {
    rc = EXIT_FAILURE;
  }

  /* Single barrier at the end */
  shmem_barrier_all();

  log_close(rc);
  shmem_finalize();
  return rc;
}
