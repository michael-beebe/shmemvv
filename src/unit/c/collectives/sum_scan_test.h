/**
 * @file sum_scan_test.h
 * @brief Common inclusive/exclusive scan test body.
 */
#ifndef SHMEMVV_SUM_SCAN_TEST_H
#define SHMEMVV_SUM_SCAN_TEST_H

#define TEST_C_SHMEM_SUM_SCAN(TYPE, NAME, TEAM)                                \
  ({                                                                           \
    log_routine("shmem_" #NAME "_sum_scan");                                   \
    static TYPE source[5], dest[5];                                            \
    int rank = shmem_team_my_pe(TEAM), size = shmem_team_n_pes(TEAM);          \
    TYPE unit = (TYPE)1;                                                       \
    if (strcmp(#NAME, "complexf") == 0 || strcmp(#NAME, "complexd") == 0)      \
      unit = (TYPE)(1 + I);                                                    \
    bool passed = true;                                                        \
    for (int inplace = 0; inplace < 2; ++inplace) {                            \
      for (int repeat = 0; repeat < 2; ++repeat) {                             \
        log_info("Team rank %d/%d, in place %d, repeat %d", rank, size,        \
                 inplace, repeat);                                             \
        for (size_t j = 0; j < 5; ++j)                                         \
          dest[j] = (TYPE)99;                                                  \
        /* Make every destination ready before entering any scan. */           \
        shmem_team_sync(TEAM);                                                 \
        if (!inplace && rank == size - 1) {                                    \
          const struct timespec delay = {.tv_sec = 0, .tv_nsec = 10000000};    \
          nanosleep(&delay, NULL);                                             \
        }                                                                      \
        for (size_t j = 0; j < 3; ++j) {                                       \
          TYPE value = unit * (TYPE)((rank % 2 + 1) * (int)(j + 1) + repeat);  \
          if (inplace)                                                         \
            dest[j] = value;                                                   \
          else                                                                 \
            source[j] = value;                                                 \
        }                                                                      \
        /* In-place buffers must also be ready to receive remote writes.       \
           Out-of-place sources deliberately have no readiness barrier. */     \
        if (inplace)                                                           \
          shmem_team_sync(TEAM);                                               \
        fflush(NULL);                                                          \
        int ret = SCAN(TYPE, NAME, TEAM, dest, inplace ? dest : source, 3);    \
        if (ret != 0) {                                                        \
          log_fail(#NAME " scan returned %d", ret);                            \
          passed = false;                                                      \
        }                                                                      \
        for (size_t j = 0; j < 3; ++j) {                                       \
          TYPE expected = (TYPE)0;                                             \
          int count = SCAN_EXCLUSIVE ? rank : rank + 1;                        \
          for (int k = 0; k < count; ++k)                                      \
            expected += unit * (TYPE)((k % 2 + 1) * (int)(j + 1) + repeat);    \
          if (dest[j] != expected) {                                           \
            log_fail(#NAME " element %zu, in place %d, repeat %d mismatch "    \
                           "(real part expected %g, got %g)",                  \
                     j, inplace, repeat, (double)expected, (double)dest[j]);   \
            passed = false;                                                    \
          }                                                                    \
        }                                                                      \
        if (dest[3] != (TYPE)99 || dest[4] != (TYPE)99) {                      \
          log_fail(#NAME " scan overwrote elements beyond nelems");            \
          passed = false;                                                      \
        }                                                                      \
        /* Source reuse is allowed immediately on local return. */             \
        if (!inplace)                                                          \
          for (size_t j = 0; j < 3; ++j)                                       \
            source[j] = (TYPE)0;                                               \
        shmem_team_sync(TEAM);                                                 \
      }                                                                        \
    }                                                                          \
    for (size_t j = 0; j < 5; ++j)                                             \
      dest[j] = (TYPE)99;                                                      \
    shmem_team_sync(TEAM);                                                     \
    values_result &= passed;                                                   \
    passed = true;                                                             \
    fflush(NULL);                                                              \
    int zero_ret = SCAN(TYPE, NAME, TEAM, dest, source, 0);                    \
    /* shmem_scan.tex allows a nonzero error return; it supplies no buffer     \
       guarantees on failure. Do not promote that return into a normative      \
       mismatch or inspect failure-path output. */                             \
    if (zero_ret != 0)                                                         \
      log_warn(#NAME " empty scan returned permitted error %d; successful "    \
                     "zero-element completion was not validated",              \
               zero_ret);                                                      \
    else                                                                       \
      for (size_t j = 0; j < 5; ++j)                                           \
        if (dest[j] != (TYPE)99) {                                             \
          log_fail(#NAME " successful empty scan changed destination");        \
          passed = false;                                                      \
        }                                                                      \
    shmem_team_sync(TEAM);                                                     \
    passed;                                                                    \
  })
#endif
