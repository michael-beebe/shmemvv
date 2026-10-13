/**
 * @file c_shmem_test_masks.c
 * @brief Scalar test mask, empty-set, and no-match contracts for every
 * comparison.
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
#define SCALAR_CALL(NAME, OP, ...) shmem_##OP(__VA_ARGS__)
#else
#define SCALAR_CALL(NAME, OP, ...) shmem_##NAME##_##OP(__VA_ARGS__)
#endif

static bool valid_indices(size_t count, const size_t *indices,
                          const int *status) {
  bool seen[4] = {false, false, false, false};
  if (count > 4)
    return false;
  for (size_t i = 0; i < count; ++i) {
    if (indices[i] >= 4 || seen[indices[i]] || status[indices[i]] != 0)
      return false;
    seen[indices[i]] = true;
  }
  return true;
}

#define TEST_C_SHMEM_TEST_MASKS(TYPE, NAME)                                    \
  ({                                                                           \
    log_routine("shmem_" #NAME "_test_{all,any,some} mask contracts");         \
    static TYPE ivars[4];                                                      \
    const int selected[4] = {0, 1, 0, 7};                                      \
    const int excluded[4] = {1, 2, 3, 4};                                      \
    const int operators[] = {SHMEM_CMP_EQ, SHMEM_CMP_NE, SHMEM_CMP_GT,         \
                             SHMEM_CMP_LE, SHMEM_CMP_LT, SHMEM_CMP_GE};        \
    const int matching[] = {2, 3, 3, 1, 1, 3};                                 \
    const int failing[] = {3, 2, 1, 3, 3, 1};                                  \
    bool passed = true;                                                        \
    size_t indices[4];                                                         \
    for (size_t op = 0; op < 6; ++op) {                                        \
      int cmp = operators[op];                                                 \
      TYPE value = (TYPE)2;                                                    \
      for (size_t i = 0; i < 4; ++i)                                           \
        ivars[i] = (TYPE)(selected[i] ? failing[op] : matching[op]);           \
      int all = SCALAR_CALL(NAME, test_all, ivars, 4, selected, cmp, value);   \
      size_t any =                                                             \
          SCALAR_CALL(NAME, test_any, ivars, 4, selected, cmp, value);         \
      size_t some = SCALAR_CALL(NAME, test_some, ivars, 4, indices, selected,  \
                                cmp, value);                                   \
      bool ok = all == 1 && any < 4 && (any >= 4 || selected[any] == 0) &&     \
                some == 2 && valid_indices(some, indices, selected);           \
      SCALAR_CALL(NAME, wait_until_all, ivars, 4, selected, cmp, value);       \
      any = SCALAR_CALL(NAME, wait_until_any, ivars, 4, selected, cmp, value); \
      some = SCALAR_CALL(NAME, wait_until_some, ivars, 4, indices, selected,   \
                         cmp, value);                                          \
      ok &= any < 4 && (any >= 4 || selected[any] == 0) && some > 0 &&         \
            valid_indices(some, indices, selected);                            \
      all = SCALAR_CALL(NAME, test_all, ivars, 4, excluded, cmp, value);       \
      any = SCALAR_CALL(NAME, test_any, ivars, 4, excluded, cmp, value);       \
      some = SCALAR_CALL(NAME, test_some, ivars, 4, indices, excluded, cmp,    \
                         value);                                               \
      ok &= all == 1 && any == SIZE_MAX && some == 0;                          \
      SCALAR_CALL(NAME, wait_until_all, ivars, 4, excluded, cmp, value);       \
      all = SCALAR_CALL(NAME, test_all, (TYPE *)NULL, 0, NULL, cmp, value);    \
      any = SCALAR_CALL(NAME, test_any, (TYPE *)NULL, 0, NULL, cmp, value);    \
      some = SCALAR_CALL(NAME, test_some, (TYPE *)NULL, 0, NULL, NULL, cmp,    \
                         value);                                               \
      ok &= all == 1 && any == SIZE_MAX && some == 0;                          \
      SCALAR_CALL(NAME, wait_until_all, (TYPE *)NULL, 0, NULL, cmp, value);    \
      for (size_t i = 0; i < 4; ++i)                                           \
        ivars[i] = (TYPE)(selected[i] ? matching[op] : failing[op]);           \
      all = SCALAR_CALL(NAME, test_all, ivars, 4, selected, cmp, value);       \
      any = SCALAR_CALL(NAME, test_any, ivars, 4, selected, cmp, value);       \
      some = SCALAR_CALL(NAME, test_some, ivars, 4, indices, selected, cmp,    \
                         value);                                               \
      ok &= all == 0 && any == SIZE_MAX && some == 0;                          \
      for (size_t i = 0; i < 4; ++i)                                           \
        ivars[i] = (TYPE)failing[op];                                          \
      ok &= SCALAR_CALL(NAME, test_all, ivars, 4, NULL, cmp, value) == 0;      \
      ok &=                                                                    \
          SCALAR_CALL(NAME, test_any, ivars, 4, NULL, cmp, value) == SIZE_MAX; \
      if (!ok)                                                                 \
        log_fail(#NAME " scalar mask/empty/no-match contract, cmp %d", cmp);   \
      passed &= ok;                                                            \
    }                                                                          \
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
  result = true;
#define TEST_TYPE(TYPE, NAME) result &= TEST_C_SHMEM_TEST_MASKS(TYPE, NAME);
  SHMEM_STANDARD_AMO_TYPE_TABLE(TEST_TYPE)
#undef TEST_TYPE
  shmem_barrier_all();
  reduce_test_result("scalar masked/empty/no-match test semantics", &result,
                     false);
  shmem_barrier_all();
  int rc = result ? EXIT_SUCCESS : EXIT_FAILURE;
  log_close(rc);
  shmem_finalize();
  return rc;
}
