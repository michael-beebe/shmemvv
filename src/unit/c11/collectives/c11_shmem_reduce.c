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
#include "type_tables.h"

const double FLOATING_POINT_TOLERANCE = 1e-6;

#define ARR_LEN 10

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
    TYPE *src = (TYPE *)shmem_calloc(ARR_LEN, sizeof(TYPE));                   \
    TYPE *dest = (TYPE *)shmem_calloc(ARR_LEN, sizeof(TYPE));                  \
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
    for (int i = 0; i < ARR_LEN; i ++){                                        \
      src[i] = src_val + i;                                                    \
    }                                                                          \
    log_info("src array at %p set to i + %f", (void *)src, (double) src_val);  \
                                                                               \
    log_info("executing shmem_max_reduce: dest = %p, src = %p", (void *)dest,  \
             (void *)src);                                                     \
    shmem_max_reduce(SHMEM_TEAM_WORLD, dest, src, ARR_LEN);                    \
                                                                               \
    log_info("validating result...");                                          \
    TYPE expected;                                                             \
    if (sizeof(TYPE) == 1) {                                                   \
      /* For 8-bit types, max of (PE % 100) values is min(99, npes-1) */       \
      expected = (TYPE)(npes >= 100 ? 99 : npes - 1);                          \
    } else {                                                                   \
      expected = (TYPE)(npes - 1);                                             \
    }                                                                          \
    bool success = true;                                                       \
    for (int i = 0; i < 10; i++){                                              \
      if (is_fp) {                                                             \
        success = (fabsl((long double)(dest[i] - (expected + i))) <=           \
                  FLOATING_POINT_TOLERANCE);                                   \
      } else {                                                                 \
        success = (dest[i] == expected + i);                                   \
      }                                                                        \
      if (!success){                                                           \
        printf("shmem_" #TYPENAME "_max_reduce dest[%d] = %lf, expected %d",   \
            i, (double) dest[i], expected + i );                               \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
                                                                               \
    if (success)                                                               \
      log_info("shmem_" #TYPENAME "_max_reduce produced expected result.");    \
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
    TYPE *src = (TYPE *)shmem_calloc(ARR_LEN, sizeof(TYPE));                   \
    TYPE *dest = (TYPE *)shmem_calloc(ARR_LEN, sizeof(TYPE));                  \
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
    for (int i = 0; i < ARR_LEN; i ++){                                        \
      src[i] = src_val + i;                                                    \
    }                                                                          \
    log_info("src array at %p set to i + %f", (void *)src, (double) src_val);  \
                                                                               \
    log_info("executing shmem_min_reduce: dest = %p, src = %p", (void *)dest,  \
             (void *)src);                                                     \
    shmem_min_reduce(SHMEM_TEAM_WORLD, dest, src, ARR_LEN);                    \
                                                                               \
    log_info("validating result...");                                          \
    bool success = true;                                                       \
    for (int i = 0; i < ARR_LEN; i++){                                         \
      if (is_fp) {                                                             \
        success = (fabsl((long double)(dest[i] - (i))) <=                      \
                  FLOATING_POINT_TOLERANCE);                                   \
      } else {                                                                 \
        success = (dest[i] == (TYPE)(i));                                      \
      }                                                                        \
      if (!success){                                                           \
        log_fail("shmem_" #TYPENAME "_min_reduce dest[%d] = %lf, expected %d", \
            i, (double) dest[i], i);                                           \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
    if (success)                                                               \
      log_info("shmem_" #TYPENAME "_min_reduce produced expected result.");    \
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
    TYPE *src = (TYPE *)shmem_calloc(ARR_LEN, sizeof(TYPE));                   \
    TYPE *dest = (TYPE *)shmem_calloc(ARR_LEN, sizeof(TYPE));                  \
    log_info("shmem_malloc'd %d bytes @ &src = %p, %d bytes @ &dest = %p",     \
             sizeof(TYPE), (void *)src, sizeof(TYPE), (void *)dest);           \
                                                                               \
    TYPE src_val = (TYPE)((sizeof(TYPE) == 1)? mype % 100 : mype);             \
    for (int i = 0; i < ARR_LEN; i ++){                                        \
      src[i] = (TYPE) (src_val + i);                                           \
    }                                                                          \
    if (is_complex((TYPE)0)) {                                                 \
      log_info("set %p (src) to %g+%gi", (void *)src, (double)creall(*src),    \
               (double)cimagl(*src));                                          \
    } else {                                                                   \
      log_info("set %p (src) to %g", (void *)src, (double)*src);               \
    }                                                                          \
                                                                               \
    log_info("executing shmem_sum_reduce: dest = %p, src = %p", (void *)dest,  \
             (void *)src);                                                     \
    shmem_sum_reduce(SHMEM_TEAM_WORLD, dest, src, ARR_LEN);                    \
                                                                               \
    log_info("validating result...");                                          \
    TYPE exp_npes = (TYPE)((sizeof(TYPE) == 1)? npes % 100 : npes);            \
    bool success = true;                                                       \
    TYPE expected;                                                             \
    for (int i = 0; i < ARR_LEN; i++){                                         \
      expected = (TYPE) (exp_npes * (exp_npes - 1) / 2 + i * exp_npes);        \
      if (is_complex((TYPE)0)) {                                               \
        /* Complex validation: check real and imaginary parts separately */    \
        long double real_diff = fabsl(creall(dest[i]) - creall(expected));     \
        long double imag_diff = fabsl(cimagl(dest[i]) - cimagl(expected));     \
        success = (real_diff <= FLOATING_POINT_TOLERANCE) &&                   \
                  (imag_diff <= FLOATING_POINT_TOLERANCE);                     \
      } else if (is_fp) {                                                      \
        success = (fabsl((long double)(dest[i] - expected)) <=                 \
                  FLOATING_POINT_TOLERANCE);                                   \
      } else {                                                                 \
        success = (dest[i] == expected);                                       \
      }                                                                        \
      if (!success){                                                           \
        log_fail("shmem_" #TYPENAME "_sum_reduce dest[%d] = %lf, expected %lf",\
            i, (double) dest[i], (double) expected);                           \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
                                                                               \
    if (success)                                                               \
      log_info("shmem_" #TYPENAME "_sum_reduce produced expected result.");    \
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
    } else if (npes > 7) {                                                     \
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
      if (npes > 7) {                                                         \
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
    TYPE *src = (TYPE *)shmem_calloc(ARR_LEN, sizeof(TYPE));                   \
    TYPE *dest = (TYPE *)shmem_calloc(ARR_LEN, sizeof(TYPE));                  \
    log_info("shmem_malloc'd %d bytes @ &src = %p, %d bytes @ &dest = %p",     \
             sizeof(TYPE), (void *)src, sizeof(TYPE), (void *)dest);           \
                                                                               \
    TYPE my_pe_safe = (TYPE)(sizeof(TYPE) == 1? mype % 100: mype);             \
    for (int i = 0; i < ARR_LEN; i++){                                         \
      src[i] = (TYPE)(my_pe_safe + i);                                         \
    }                                                                          \
    log_info("set %p (src) to i + %lf", (void *)src, (double)my_pe_safe);      \
                                                                               \
    log_info("executing shmem_and_reduce: dest = %p, src = %p", (void *)dest,  \
             (void *)src);                                                     \
    shmem_and_reduce(SHMEM_TEAM_WORLD, dest, src, ARR_LEN);                    \
                                                                               \
    log_info("validating result...");                                          \
    bool success = true;                                                       \
    for (int i = 0; i < ARR_LEN; i++){                                         \
      /*find expected value*/                                                  \
      TYPE expected = (TYPE)(~0);                                              \
      for (int pe = 0; pe < npes; pe++) {                                      \
        TYPE pe_val = (TYPE)((sizeof(TYPE) == 1) ? pe % 100 : pe);             \
        TYPE src_val = (TYPE)(pe_val + i);                                     \
        expected &= src_val;                                                   \
      }                                                                        \
      success &= (dest[i] == expected);                                        \
      if (!success){                                                           \
        log_fail("shmem_" #TYPENAME "_and_reduce dest[%d] = %lf, expected %lf",\
            i, (double) dest[i], (double) expected);                           \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
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
    TYPE *src = (TYPE *)shmem_calloc(ARR_LEN, sizeof(TYPE));                   \
    TYPE *dest = (TYPE *)shmem_calloc(ARR_LEN, sizeof(TYPE));                  \
    log_info("shmem_malloc'd %d bytes @ &src = %p, %d bytes @ &dest = %p",     \
             sizeof(TYPE), (void *)src, sizeof(TYPE), (void *)dest);           \
                                                                               \
    TYPE my_pe_safe = (TYPE)(sizeof(TYPE) == 1? mype % 100: mype);             \
    for (int i = 0; i < ARR_LEN; i++){                                         \
      src[i] = (TYPE)(my_pe_safe + i);                                         \
    }                                                                          \
    log_info("set %p (src) to i + %lf", (void *)src, (double)my_pe_safe);      \
                                                                               \
    log_info("executing shmem_or_reduce: dest = %p, src = %p", (void *)dest,   \
             (void *)src);                                                     \
    shmem_or_reduce(SHMEM_TEAM_WORLD, dest, src, ARR_LEN);                     \
                                                                               \
    log_info("validating result...");                                          \
    bool success = true;                                                       \
    for (int i = 0; i < ARR_LEN; i++){                                         \
      /*find expected value*/                                                  \
      TYPE expected = 0;                                                       \
      for (int pe = 0; pe < npes; pe++) {                                      \
        TYPE pe_val = (TYPE)((sizeof(TYPE) == 1) ? pe % 100 : pe);             \
        TYPE src_val = (TYPE)(pe_val + i);                                     \
        expected |= src_val;                                                   \
      }                                                                        \
      success &= (dest[i] == expected);                                        \
      if (!success){                                                           \
        log_fail("shmem_" #TYPENAME "_or_reduce dest[%d] = %lf, expected %lf", \
            i, (double) dest[i], (double) expected);                           \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
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
    TYPE *src = (TYPE *)shmem_calloc(ARR_LEN, sizeof(TYPE));                   \
    TYPE *dest = (TYPE *)shmem_calloc(ARR_LEN, sizeof(TYPE));                  \
    log_info("shmem_malloc'd %d bytes @ &src = %p, %d bytes @ &dest = %p",     \
             sizeof(TYPE), (void *)src, sizeof(TYPE), (void *)dest);           \
                                                                               \
    TYPE my_pe_safe = (TYPE)(sizeof(TYPE) == 1? mype % 100: mype);             \
    for (int i = 0; i < ARR_LEN; i++){                                         \
      src[i] = (TYPE)(my_pe_safe + i);                                         \
    }                                                                          \
    log_info("set %p (src) to i + %lf", (void *)src, (double)my_pe_safe);      \
                                                                               \
    log_info("executing shmem_xor_reduce: dest = %p, src = %p", (void *)dest,  \
             (void *)src);                                                     \
    shmem_xor_reduce(SHMEM_TEAM_WORLD, dest, src, ARR_LEN);                    \
                                                                               \
    log_info("validating result...");                                          \
    bool success = true;                                                       \
    for (int i = 0; i < ARR_LEN; i++){                                         \
      /*find expected value*/                                                  \
      TYPE expected = 0;                                                       \
      for (int pe = 0; pe < npes; pe++) {                                      \
        TYPE pe_val = (TYPE)((sizeof(TYPE) == 1) ? pe % 100 : pe);             \
        TYPE src_val = (TYPE)(pe_val + i);                                     \
        expected ^= src_val;                                                   \
      }                                                                        \
      success &= (dest[i] == expected);                                        \
      if (!success){                                                           \
        log_fail("shmem_" #TYPENAME "_xor_reduce dest[%d] = %lf, expected %lf",\
            i, (double) dest[i], (double) expected);                           \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
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
  /*probably don't need, right?*/
  // if (!(npes >= 2)) {
  //   log_warn("Not enough PEs to run test (requires 2 PEs, have %d PEs)",
  //            shmem_n_pes());
  //   if (mype == 0) {
  //     display_not_enough_pes("atomic");
  //   }
  //   shmem_finalize();
  //   return EXIT_SUCCESS;
  // }

  /* Test MAX reduction - SHMEM_REDUCE_MINMAX_TYPE_TABLE */
  static int result_max = true;
  #define X(type, shmem_types) result_max &= TEST_C11_SHMEM_MAX_REDUCE(type, shmem_types);
    SHMEM_REDUCE_MINMAX_TYPE_TABLE(X)
  #undef X

  reduce_test_result("C11 shmem_max_reduce", &result_max, false);

  /* Test MIN reduction - SHMEM_REDUCE_MINMAX_TYPE_TABLE */
  static int result_min = true;
  #define X(type, shmem_types) result_min &= TEST_C11_SHMEM_MIN_REDUCE(type, shmem_types);
    SHMEM_REDUCE_MINMAX_TYPE_TABLE(X)
  #undef X

  reduce_test_result("C11 shmem_min_reduce", &result_min, false);

  /* Test SUM reduction - SHMEM_REDUCE_ARITH_TYPE_TABLE */
  static int result_sum = true;
  #define X(type, shmem_types) result_sum &= TEST_C11_SHMEM_SUM_REDUCE(type, shmem_types);
    SHMEM_REDUCE_ARITH_TYPE_TABLE(X)
  #undef X
  reduce_test_result("C11 shmem_sum_reduce", &result_sum, false);

  /* Test PROD reduction - SHMEM_REDUCE_ARITH_TYPE_TABLE */
  static int result_prod = true;
  #define X(type, shmem_types) result_prod &= TEST_C11_SHMEM_PROD_REDUCE(type, shmem_types);
    SHMEM_REDUCE_ARITH_TYPE_TABLE(X)
  #undef X

  reduce_test_result("C11 shmem_prod_reduce", &result_prod, false);

  /* Test AND reduction - unsigned integer types only */
  static int result_and = true;
  #define X(type, shmem_types) result_and &= TEST_C11_SHMEM_AND_REDUCE(type, shmem_types);
    SHMEM_REDUCE_BITWISE_TYPE_TABLE(X)
  #undef X

  reduce_test_result("C11 shmem_and_reduce", &result_and, false);

  /* Test OR reduction - unsigned integer types only */
  static int result_or = true;
  #define X(type, shmem_types) result_or &= TEST_C11_SHMEM_OR_REDUCE(type, shmem_types);
    SHMEM_REDUCE_BITWISE_TYPE_TABLE(X)
  #undef X
  reduce_test_result("C11 shmem_or_reduce", &result_or, false);

  /* Test XOR reduction - unsigned integer types only */
  static int result_xor = true;
  #define X(type, shmem_types) result_xor &= TEST_C11_SHMEM_XOR_REDUCE(type, shmem_types);
    SHMEM_REDUCE_BITWISE_TYPE_TABLE(X)
  #undef X

  reduce_test_result("C11 shmem_xor_reduce", &result_xor, false);

  /* Single barrier at the end */
  shmem_barrier_all();

  bool rc = result_max & result_min & result_and & result_or & result_xor 
    & result_prod & result_sum ? EXIT_SUCCESS : EXIT_FAILURE;
  log_close(rc);
  shmem_finalize();
  return rc;
}
