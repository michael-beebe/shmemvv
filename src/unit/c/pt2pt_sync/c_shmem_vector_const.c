/**
 * @file c_shmem_vector_const.c
 * @brief All six vector interfaces accept immutable comparison arrays.
 * Also checks masked and empty test vectors without potentially blocking waits.
 */
#include <shmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "shmemvv.h"
#include "type_tables.h"

#ifdef API_USE_GENERICS
#define VECTOR_CALL(NAME, OP, ...) shmem_##OP##_vector(__VA_ARGS__)
#else
#define VECTOR_CALL(NAME, OP, ...) shmem_##NAME##_##OP##_vector(__VA_ARGS__)
#endif

static bool const_result;

static bool valid_indices(size_t count, const size_t *indices,
                          const int *status) {
  bool seen[4] = {false, false, false, false};
  if (count > 4)
    return false;
  for (size_t i = 0; i < count; ++i) {
    if (indices[i] >= 4 || seen[indices[i]] || status[indices[i]])
      return false;
    seen[indices[i]] = true;
  }
  return true;
}

#define TEST_C_SHMEM_VECTOR_CONST(TYPE, NAME)                                  \
  ({                                                                           \
    log_routine("shmem_" #NAME "_*_vector const comparisons");                 \
    static TYPE ivar[4];                                                       \
    const TYPE comparisons[4] = {(TYPE)1, (TYPE)2, (TYPE)3, (TYPE)4};          \
    const TYPE saved[4] = {(TYPE)1, (TYPE)2, (TYPE)3, (TYPE)4};                \
    const int active[4] = {0, 0, 0, 0};                                        \
    const int masked[4] = {0, 1, 0, 1};                                        \
    const int excluded[4] = {1, 1, 1, 1};                                      \
    size_t indices[4];                                                         \
    bool passed = true;                                                        \
    for (size_t i = 0; i < 4; ++i)                                             \
      ivar[i] = comparisons[i];                                                \
    int all = VECTOR_CALL(NAME, test_all, ivar, 4, active, SHMEM_CMP_EQ,       \
                          comparisons);                                        \
    size_t any = VECTOR_CALL(NAME, test_any, ivar, 4, active, SHMEM_CMP_EQ,    \
                             comparisons);                                     \
    size_t some = VECTOR_CALL(NAME, test_some, ivar, 4, indices, active,       \
                              SHMEM_CMP_EQ, comparisons);                      \
    passed &= all == 1 && any < 4 && some == 4 &&                              \
              valid_indices(some, indices, active);                            \
    VECTOR_CALL(NAME, wait_until_all, ivar, 4, active, SHMEM_CMP_EQ,           \
                comparisons);                                                  \
    any = VECTOR_CALL(NAME, wait_until_any, ivar, 4, active, SHMEM_CMP_EQ,     \
                      comparisons);                                            \
    some = VECTOR_CALL(NAME, wait_until_some, ivar, 4, indices, active,        \
                       SHMEM_CMP_EQ, comparisons);                             \
    /* wait_until_some_vector promises at least one index, not a particular    \
       count or order; accept any nonempty set of unique matching indices. */  \
    passed &= any < 4 && some > 0 && some <= 4 &&                              \
              valid_indices(some, indices, active);                            \
    for (size_t i = 0; i < 4; ++i)                                             \
      passed &= comparisons[i] == saved[i];                                    \
    const_result &= passed;                                                    \
    if (!passed)                                                               \
      log_fail(#NAME " const vector call failed");                             \
    passed = true;                                                             \
    for (size_t i = 0; i < 4; ++i)                                             \
      if (masked[i])                                                           \
        ivar[i] = (TYPE)0;                                                     \
    all = VECTOR_CALL(NAME, test_all, ivar, 4, masked, SHMEM_CMP_EQ,           \
                      comparisons);                                            \
    any = VECTOR_CALL(NAME, test_any, ivar, 4, masked, SHMEM_CMP_EQ,           \
                      comparisons);                                            \
    some = VECTOR_CALL(NAME, test_some, ivar, 4, indices, masked,              \
                       SHMEM_CMP_EQ, comparisons);                             \
    if (all != 1 || any >= 4 || (any < 4 && masked[any]) || some != 2 ||       \
        !valid_indices(some, indices, masked)) {                               \
      log_fail(#NAME " masked vector: all %d, any %zu, some %zu", all, any,    \
               some);                                                          \
      passed = false;                                                          \
    }                                                                          \
    all = VECTOR_CALL(NAME, test_all, ivar, 4, excluded, SHMEM_CMP_EQ,         \
                      comparisons);                                            \
    any = VECTOR_CALL(NAME, test_any, ivar, 4, excluded, SHMEM_CMP_EQ,         \
                      comparisons);                                            \
    some = VECTOR_CALL(NAME, test_some, ivar, 4, indices, excluded,            \
                       SHMEM_CMP_EQ, comparisons);                             \
    if (all != 1 || any != SIZE_MAX || some != 0) {                            \
      log_fail(#NAME " excluded vector: all %d, any %zu, some %zu", all, any,  \
               some);                                                          \
      passed = false;                                                          \
    }                                                                          \
    all =                                                                      \
        VECTOR_CALL(NAME, test_all, ivar, 0, NULL, SHMEM_CMP_EQ, comparisons); \
    any =                                                                      \
        VECTOR_CALL(NAME, test_any, ivar, 0, NULL, SHMEM_CMP_EQ, comparisons); \
    some = VECTOR_CALL(NAME, test_some, ivar, 0, indices, NULL, SHMEM_CMP_EQ,  \
                       comparisons);                                           \
    if (all != 1 || any != SIZE_MAX || some != 0) {                            \
      log_fail(#NAME " zero-length vector: all %d, any %zu, some %zu", all,    \
               any, some);                                                     \
      passed = false;                                                          \
    }                                                                          \
    for (size_t i = 0; i < 4; ++i)                                             \
      ivar[i] = (TYPE)0;                                                       \
    all =                                                                      \
        VECTOR_CALL(NAME, test_all, ivar, 4, NULL, SHMEM_CMP_EQ, comparisons); \
    any =                                                                      \
        VECTOR_CALL(NAME, test_any, ivar, 4, NULL, SHMEM_CMP_EQ, comparisons); \
    some = VECTOR_CALL(NAME, test_some, ivar, 4, indices, NULL, SHMEM_CMP_EQ,  \
                       comparisons);                                           \
    if (all != 0 || any != SIZE_MAX || some != 0) {                            \
      log_fail(#NAME " no-match vector: all %d, any %zu, some %zu", all, any,  \
               some);                                                          \
      passed = false;                                                          \
    }                                                                          \
    for (size_t i = 0; i < 4; ++i)                                             \
      passed &= comparisons[i] == saved[i];                                    \
    if (!passed)                                                               \
      log_fail(#NAME " const vector validation failed");                       \
    passed;                                                                    \
  })

int main(int argc, char *argv[]) {
  if (argc > 1 && strcmp(argv[1], "--init-thread") == 0) {
    int provided;
    if (shmem_init_thread(SHMEM_THREAD_MULTIPLE, &provided) != 0)
      return EXIT_FAILURE;
  } else {
    shmem_init();
  }
  log_init(__FILE__);
  static bool result;
  result = const_result = true;
#define TEST_TYPE(TYPE, NAME) result &= TEST_C_SHMEM_VECTOR_CONST(TYPE, NAME);
  SHMEM_STANDARD_AMO_TYPE_TABLE(TEST_TYPE)
#undef TEST_TYPE
  shmem_barrier_all();
  reduce_test_result("const vector test/wait interfaces", &const_result, false);
  reduce_test_result("vector masked/empty test semantics", &result, false);
  shmem_barrier_all();
  int rc = result && const_result ? EXIT_SUCCESS : EXIT_FAILURE;
  log_close(rc);
  shmem_finalize();
  return rc;
}
