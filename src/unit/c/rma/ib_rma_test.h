/**
 * @file ib_rma_test.h
 * @brief Shared test bodies for ibput and ibget, following the typed RMA tests.
 */
#ifndef SHMEMVV_IB_RMA_TEST_H
#define SHMEMVV_IB_RMA_TEST_H

/* bsize, destination stride, source stride, nblocks. Include unequal strides,
   single-element and contiguous blocks, and both zero-length forms. */
static const size_t block_cases[][4] = {
    {2, 5, 4, 3}, {1, 3, 2, 4}, {3, 3, 3, 2}, {0, 0, 0, 4}, {2, 5, 4, 0}};

#define TEST_C_SHMEM_IB_TRANSFER(TYPE, NAME, CTX, EXPLICIT)                    \
  ({                                                                           \
    log_routine("interleaved block " #NAME);                                   \
    static TYPE source[40], dest[40];                                          \
    bool passed = true;                                                        \
    int me = shmem_my_pe(), npes = shmem_n_pes();                              \
    int next = (me + 1) % npes;                                                \
    int source_rank = IB_IS_GET ? next : (me + npes - 1) % npes;               \
    for (size_t c = 0; c < sizeof(block_cases) / sizeof(block_cases[0]);       \
         ++c) {                                                                \
      size_t bsize = block_cases[c][0], dst = block_cases[c][1];               \
      size_t sst = block_cases[c][2], nblocks = block_cases[c][3];             \
      log_info("Case %zu: block %zu, strides %zu/%zu, blocks %zu, ctx %d", c,  \
               bsize, dst, sst, nblocks, EXPLICIT);                            \
      for (size_t i = 0; i < 40; ++i) {                                        \
        source[i] = (TYPE)((i + (size_t)me * 3) % 31 + 1);                     \
        dest[i] = (TYPE)99;                                                    \
      }                                                                        \
      shmem_barrier_all();                                                     \
      if (EXPLICIT)                                                            \
        CTX_TRANSFER(TYPE, NAME, CTX, dest + 1, source + 1, dst, sst, bsize,   \
                     nblocks, next);                                           \
      else                                                                     \
        TRANSFER(TYPE, NAME, dest + 1, source + 1, dst, sst, bsize, nblocks,   \
                 next);                                                        \
      if (!IB_IS_GET) {                                                        \
        /* Blocking puts must release the source on return. */                 \
        for (size_t i = 0; i < 40; ++i)                                        \
          source[i] = (TYPE)0;                                                 \
        if (EXPLICIT)                                                          \
          shmem_ctx_quiet(CTX);                                                \
        else                                                                   \
          shmem_quiet();                                                       \
        shmem_sync_all();                                                      \
      }                                                                        \
      /* Blocking gets must update local dest before any quiet or barrier. */  \
      for (size_t i = 0; i < 40; ++i) {                                        \
        TYPE expected = (TYPE)99;                                              \
        for (size_t b = 0; b < nblocks; ++b)                                   \
          if (i >= 1 + b * dst && i < 1 + b * dst + bsize)                     \
            expected = (TYPE)((1 + b * sst + i - (1 + b * dst) +               \
                               (size_t)source_rank * 3) %                      \
                                  31 +                                         \
                              1);                                              \
        if (dest[i] != expected) {                                             \
          log_fail(#NAME " case %zu element %zu: expected %g, got %g", c, i,   \
                   (double)expected, (double)dest[i]);                         \
          passed = false;                                                      \
        }                                                                      \
      }                                                                        \
      shmem_barrier_all();                                                     \
    }                                                                          \
    passed;                                                                    \
  })

/* Byte patterns also cover 128-bit elements without requiring a compiler
   integer extension. The sized API scales each stride by SIZE/8 bytes. */
#define TEST_C_SHMEM_IB_SIZE(SIZE, CTX, EXPLICIT)                              \
  ({                                                                           \
    log_routine("interleaved block " #SIZE);                                   \
    static union {                                                             \
      long double align;                                                       \
      unsigned char bytes[640];                                                \
    } src, dstbuf;                                                             \
    unsigned char *source = src.bytes, *dest = dstbuf.bytes;                   \
    const size_t width = SIZE / 8;                                             \
    bool passed = true;                                                        \
    int me = shmem_my_pe(), npes = shmem_n_pes();                              \
    int next = (me + 1) % npes;                                                \
    int source_rank = IB_IS_GET ? next : (me + npes - 1) % npes;               \
    for (size_t c = 0; c < sizeof(block_cases) / sizeof(block_cases[0]);       \
         ++c) {                                                                \
      size_t bsize = block_cases[c][0], dst = block_cases[c][1];               \
      size_t sst = block_cases[c][2], nblocks = block_cases[c][3];             \
      for (size_t i = 0; i < 40 * width; ++i)                                  \
        source[i] = (unsigned char)((i + (size_t)me * 17) % 251 + 1);          \
      memset(dest, 0, 40 * width);                                             \
      shmem_barrier_all();                                                     \
      if (EXPLICIT)                                                            \
        CTX_SIZE_TRANSFER(SIZE, CTX, dest + width, source + width, dst, sst,   \
                          bsize, nblocks, next);                               \
      else                                                                     \
        SIZE_TRANSFER(SIZE, dest + width, source + width, dst, sst, bsize,     \
                      nblocks, next);                                          \
      if (!IB_IS_GET) {                                                        \
        memset(source, 0, 40 * width);                                         \
        if (EXPLICIT)                                                          \
          shmem_ctx_quiet(CTX);                                                \
        else                                                                   \
          shmem_quiet();                                                       \
        shmem_sync_all();                                                      \
      }                                                                        \
      for (size_t i = 0; i < 40 * width; ++i) {                                \
        unsigned char expected = 0;                                            \
        for (size_t b = 0; b < nblocks; ++b)                                   \
          if (i >= (1 + b * dst) * width && i < (1 + b * dst + bsize) * width) \
            expected = (unsigned char)(((1 + b * sst) * width + i -            \
                                        (1 + b * dst) * width +                \
                                        (size_t)source_rank * 17) %            \
                                           251 +                               \
                                       1);                                     \
        if (dest[i] != expected) {                                             \
          log_fail(#SIZE " case %zu byte %zu: expected %u, got %u", c, i,      \
                   (unsigned)expected, (unsigned)dest[i]);                     \
          passed = false;                                                      \
        }                                                                      \
      }                                                                        \
      shmem_barrier_all();                                                     \
    }                                                                          \
    passed;                                                                    \
  })
#endif
